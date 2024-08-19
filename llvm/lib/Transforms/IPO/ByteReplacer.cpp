//===- DeadArgumentElimination.cpp - Eliminate dead arguments -------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This pass deletes dead arguments from internal functions.  Dead argument
// elimination removes arguments which are directly dead, as well as arguments
// only passed into function calls as dead arguments of other functions.  This
// pass also deletes dead return values in a similar way.
//
// This pass is often useful as a cleanup pass to run after aggressive
// interprocedural passes, which add possibly-dead arguments or return values.
//
//===----------------------------------------------------------------------===//

#include "llvm/Transforms/IPO/ByteReplacer.h"
#include "llvm/Analysis/AliasAnalysis.h"
#include "llvm/Analysis/AssumptionCache.h"
#include "llvm/Analysis/InlineAdvisor.h"
#include "llvm/Analysis/InlineCost.h"
#include "llvm/Analysis/OptimizationRemarkEmitter.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Module.h"
#include "llvm/Transforms/Utils/Cloning.h"
#include "llvm/Transforms/Utils/ModuleUtils.h"
#include <llvm/ADT/STLExtras.h>
#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/StringRef.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Instruction.h>
#include <llvm/IR/IntrinsicInst.h>
#include <llvm/IR/Intrinsics.h>
#include <llvm/IR/IntrinsicsRISCV.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/Casting.h>
#include <llvm/Support/Compiler.h>

using namespace llvm;

#define DEBUG_TYPE "byte-replacer"

bool HasByteArgsOrReturn(FunctionType *FTy) {
  if (FTy->getReturnType()->isByteOrByteVectorTy())
    return true;
  for (Type *ArgTy : FTy->params())
    if (ArgTy->isByteOrByteVectorTy())
      return true;
  return false;
}

bool AreIsomorphicTypes(Type *Ty1, Type *Ty2) {
  if (!Ty1->isVectorTy() && Ty2->isVectorTy())
    return false;
  if (Ty1->isVectorTy() && !Ty2->isVectorTy())
    return false;
  if (!Ty1->isVectorTy() || !Ty2->isVectorTy())
    return true;
  assert(Ty1->isVectorTy() && Ty2->isVectorTy() && "Expected vector types");
  VectorType *VecTy1 = cast<VectorType>(Ty1);
  VectorType *VecTy2 = cast<VectorType>(Ty2);

  if (VecTy1->isScalableTy() != VecTy2->isScalableTy())
    return false;

  if (VecTy1->getElementCount() != VecTy2->getElementCount())
    return false;

  return VecTy1->getElementType()->getScalarSizeInBits() ==
         VecTy2->getElementType()->getScalarSizeInBits();
}

Type *GetIsomorphicType(Type *Ty) {
  if (auto *STy = dyn_cast<StructType>(Ty)) {
    SmallVector<Type *, 8> NewEls;
    for (unsigned i = 0; i < STy->getNumElements(); ++i)
      NewEls.push_back(GetIsomorphicType(STy->getElementType(i)));
    return StructType::get(STy->getContext(), NewEls);
  }

  if (auto *ATy = dyn_cast<ArrayType>(Ty)) {
    Type *ElTy = GetIsomorphicType(ATy->getElementType());
    return ArrayType::get(ElTy, ATy->getNumElements());
  }
  if (auto *FTy = dyn_cast<FunctionType>(Ty)) {
    if (HasByteArgsOrReturn(FTy)) {
      SmallVector<Type *, 8> NewArgTys;
      for (unsigned i = 0; i < FTy->getNumParams(); ++i)
        NewArgTys.push_back(GetIsomorphicType(FTy->getParamType(i)));
      Type *RetTy = GetIsomorphicType(FTy->getReturnType());
      FunctionType *NewFuncTy =
          FunctionType::get(RetTy, NewArgTys, FTy->isVarArg());
      return NewFuncTy;
    }
  }
  if (!Ty->isByteOrByteVectorTy())
    return Ty;

  if (Ty->isVectorTy()) {
    VectorType *VecTy = cast<VectorType>(Ty);
    bool Scalable = VecTy->isScalableTy();
    unsigned NumEls = VecTy->getElementCount().getKnownMinValue();
    unsigned BitWidth = VecTy->getElementType()->getPrimitiveSizeInBits();
    Type *DstElTy = Type::getIntNTy(Ty->getContext(), BitWidth);
    return VectorType::get(DstElTy, NumEls, Scalable);
  }
  return Type::getIntNTy(Ty->getContext(), Ty->getPrimitiveSizeInBits());
}

void ReplaceInstructions(Function &F) {
  for (BasicBlock &BB : F)
    for (Instruction &I : make_early_inc_range(BB))
      if (auto *BC = dyn_cast<BitCastInst>(&I)) {
        if (BC->getDestTy()->isByteOrByteVectorTy()) {
          if (!AreIsomorphicTypes(BC->getSrcTy(), BC->getDestTy())) {
            BitCastInst *NewBC = new BitCastInst(
                BC->getOperand(0), GetIsomorphicType(BC->getDestTy()), "", BC);
            BC->takeName(NewBC);
            BC->mutateType(GetIsomorphicType(BC->getDestTy()));
            BC->replaceAllUsesWith(NewBC);
            BC->eraseFromParent();
          } else {
            BC->mutateType(GetIsomorphicType(BC->getDestTy()));
            BC->getOperand(0)->mutateType(GetIsomorphicType(BC->getDestTy()));
            BC->replaceAllUsesWith(BC->getOperand(0));
            BC->eraseFromParent();
          }
        }
      } else if (auto *BC = dyn_cast<ByteCastInst>(&I)) {
        if (!AreIsomorphicTypes(BC->getSrcTy(), BC->getDestTy())) {
          if (BC->getSrcTy()->isByteOrByteVectorTy())
            BC->getOperand(0)->mutateType(GetIsomorphicType(BC->getSrcTy()));
          BitCastInst *NewBC =
              new BitCastInst(BC->getOperand(0), BC->getDestTy(), "", BC);
          BC->takeName(NewBC);
          BC->replaceAllUsesWith(NewBC);
          BC->eraseFromParent();

        } else {
          BC->mutateType(BC->getDestTy());
          BC->getOperand(0)->mutateType(BC->getDestTy());
          BC->replaceAllUsesWith(BC->getOperand(0));
          BC->eraseFromParent();
        }
      } else if (auto *II = dyn_cast<IntrinsicInst>(&I)) {
        Intrinsic::ID ID = II->getIntrinsicID();
        Function *IF = II->getCalledFunction();
        Function *NewIF;
        if (!HasByteArgsOrReturn(IF->getFunctionType()))
          continue;
        SmallVector<Value *, 4> Args;
        for (unsigned i = 0; i < II->arg_size(); ++i) {
          Args.push_back(II->getArgOperand(i));
          if (Args[i]->getType()->isByteOrByteVectorTy())
            Args[i]->mutateType(GetIsomorphicType(Args[i]->getType()));
        }

        // TODO: provide Tys which are the overloaded types
        if (Intrinsic::isOverloaded(ID)) {
          FunctionType *NewFTy =
              cast<FunctionType>(GetIsomorphicType(IF->getFunctionType()));
          SmallVector<Type *, 4> ArgTys;
          Intrinsic::getIntrinsicSignature(ID, NewFTy, ArgTys);
          NewIF = Intrinsic::getDeclaration(IF->getParent(), ID, ArgTys);
        } else {
          NewIF = Intrinsic::getDeclaration(IF->getParent(), ID);
        }
        if (IF != NewIF) {
          IF->replaceAllUsesWith(NewIF);
        }
        NewIF->updateAfterNameChange();

        CallInst *NewCI = CallInst::Create(NewIF->getFunctionType(), NewIF,
                                           Args, II->getName(), II);
        NewCI->takeName(II);
        NewCI->copyMetadata(*II);
        II->mutateType(NewIF->getFunctionType()->getReturnType());
        II->replaceAllUsesWith(NewCI);
        II->eraseFromParent();
      } else if (auto *CI = dyn_cast<CallInst>(&I)) {
        FunctionType *FTy = CI->getFunctionType();
        if (HasByteArgsOrReturn(FTy)) {

          FunctionType *NewFTy = cast<FunctionType>(GetIsomorphicType(FTy));

          // llvm::errs()() << "Replacing call: " << *CI << "\n";
          if (CI->getCalledFunction() &&
              CI->getCalledFunction()->isIntrinsic()) {
            Function *IF = CI->getCalledFunction();
            Intrinsic::ID ID = CI->getCalledFunction()->getIntrinsicID();
            SmallVector<Type *, 4> ArgTys;
            SmallVector<Intrinsic::IITDescriptor, 8> Table;
            getIntrinsicInfoTableEntries(ID, Table);
            ArrayRef<Intrinsic::IITDescriptor> TableRef = Table;
            Intrinsic::matchIntrinsicSignature(NewFTy, TableRef, ArgTys);

            const std::string ExpectedName =
                Intrinsic::getName(ID, ArgTys, IF->getParent(), NewFTy);
            CI->getCalledFunction()->setName(ExpectedName);
          }
          CI->mutateFunctionType(NewFTy);
          for (unsigned i = 0; i < CI->arg_size(); ++i) {
            CI->getArgOperand(i)->mutateType(NewFTy->getParamType(i));
          }
        }
        // Mutate type of return value and return instruction
      } else if (auto *RI = dyn_cast<ReturnInst>(&I)) {
        if (RI->getReturnValue() &&
            RI->getReturnValue()->getType()->isByteOrByteVectorTy()) {
          RI->getReturnValue()->mutateType(
              GetIsomorphicType(RI->getReturnValue()->getType()));
          RI->mutateType(GetIsomorphicType(RI->getReturnValue()->getType()));
        }
        // TODO: can maybe bytecast here
        // Mutate types of operands and type
      } else if (auto *IEI = dyn_cast<InsertElementInst>(&I)) {
        if (IEI->getOperand(0)->getType()->isByteOrByteVectorTy() ||
            IEI->getOperand(1)->getType()->isByteOrByteVectorTy() ||
            IEI->getType()->isByteOrByteVectorTy()) {
          IEI->getOperand(0)->mutateType(
              GetIsomorphicType(IEI->getOperand(0)->getType()));
          IEI->getOperand(1)->mutateType(
              GetIsomorphicType(IEI->getOperand(1)->getType()));
          InsertElementInst *NewIEI = InsertElementInst::Create(
              IEI->getOperand(0), IEI->getOperand(1), IEI->getOperand(2),
              IEI->getName(), IEI);
          IEI->mutateType(GetIsomorphicType(IEI->getType()));
          NewIEI->copyMetadata(*IEI);
          IEI->replaceAllUsesWith(NewIEI);
          IEI->eraseFromParent();
        }
        // TODO: Does this work? We don't mutate the type
      } else if (auto *AI = dyn_cast<AllocaInst>(&I)) {
        if (AI->getAllocatedType()->isByteOrByteVectorTy()) {
          AllocaInst *NewAI =
              new AllocaInst(GetIsomorphicType(AI->getAllocatedType()),
                             AI->getAddressSpace(), AI->getName(), AI);
          NewAI->takeName(AI);
          NewAI->copyMetadata(*AI);
          AI->replaceAllUsesWith(NewAI);
          AI->eraseFromParent();
        }
      } else if (auto *LI = dyn_cast<LoadInst>(&I)) {
        if (LI->getType()->isByteOrByteVectorTy()) {
          LoadInst *NewLI = new LoadInst(
              GetIsomorphicType(LI->getType()), LI->getPointerOperand(),
              LI->getName(), LI->isVolatile(), LI->getAlign(),
              LI->getOrdering(), LI->getSyncScopeID(), LI);
          NewLI->takeName(LI);
          NewLI->copyMetadata(*LI);
          LI->mutateType(GetIsomorphicType(LI->getType()));
          LI->replaceAllUsesWith(NewLI);
          LI->eraseFromParent();
        }
      } else if (auto *SI = dyn_cast<StoreInst>(&I)) {
        if (SI->getValueOperand()->getType()->isByteOrByteVectorTy()) {
          SI->getValueOperand()->mutateType(
              GetIsomorphicType(SI->getValueOperand()->getType()));
          StoreInst *NewSI =
              new StoreInst(SI->getValueOperand(), SI->getPointerOperand(), SI);
          NewSI->copyMetadata(*SI);
          SI->replaceAllUsesWith(NewSI);
          SI->eraseFromParent();
        }
      } else if (auto *SVI = dyn_cast<ShuffleVectorInst>(&I)) {
        SVI->getOperand(0)->mutateType(
            GetIsomorphicType(SVI->getOperand(0)->getType()));
        SVI->getOperand(1)->mutateType(
            GetIsomorphicType(SVI->getOperand(1)->getType()));
        SVI->mutateType(GetIsomorphicType(SVI->getType()));
        auto *NewSVI =
            new ShuffleVectorInst(SVI->getOperand(0), SVI->getOperand(1),
                                  SVI->getShuffleMask(), SVI->getName(), SVI);
        NewSVI->takeName(SVI);
        NewSVI->copyMetadata(*SVI);
        SVI->replaceAllUsesWith(NewSVI);
        SVI->eraseFromParent();
      } else if (auto *SI = dyn_cast<SelectInst>(&I)) {
        if (SI->getTrueValue()->getType()->isByteOrByteVectorTy())
          SI->getTrueValue()->mutateType(
              GetIsomorphicType(SI->getTrueValue()->getType()));
        if (SI->getFalseValue()->getType()->isByteOrByteVectorTy())
          SI->getFalseValue()->mutateType(
              GetIsomorphicType(SI->getFalseValue()->getType()));
      } else if (auto *GEP = dyn_cast<GetElementPtrInst>(&I)) {
        ArrayType *ATy = dyn_cast<ArrayType>(GEP->getSourceElementType());
        if (GEP->getSourceElementType()->isByteOrByteVectorTy() ||
            (ATy && ATy->getArrayElementType()->isByteOrByteVectorTy())) {
          GEP->setSourceElementType(GetIsomorphicType(GEP->getSourceElementType()));
          GEP->setResultElementType(GetIsomorphicType(GEP->getResultElementType()));
        }
      } else if (auto *PHI = dyn_cast<PHINode>(&I)) {
        for (unsigned i = 0; i < PHI->getNumIncomingValues(); ++i) {
          if (PHI->getIncomingValue(i)->getType()->isByteOrByteVectorTy())
            PHI->getIncomingValue(i)->mutateType(
                GetIsomorphicType(PHI->getIncomingValue(i)->getType()));
        }
        PHI->mutateType(GetIsomorphicType(PHI->getType()));
      }
}

void ReplaceFunction(Function &F) {
  // Replace byte arguments types and return type
  SmallVector<Type *, 8> NewArgTys;
  for (Argument &Arg : F.args())
    NewArgTys.push_back(GetIsomorphicType(Arg.getType()));
  Type *RetTy = GetIsomorphicType(F.getReturnType());
  FunctionType *NewFuncTy = FunctionType::get(RetTy, NewArgTys, F.isVarArg());

  Function *NewFunction =
      Function::Create(NewFuncTy, F.getLinkage(), F.getName(), nullptr);
  Function::arg_iterator NewArgIter = NewFunction->arg_begin();
  for (auto &arg : F.args()) {
    NewArgIter->setName(arg.getName());
    ++NewArgIter;
  }

  ValueToValueMapTy VMap;
  NewArgIter = NewFunction->arg_begin();
  for (auto &arg : F.args())
    VMap[&arg] = &(*NewArgIter++);

  SmallVector<ReturnInst *, 8> Returns;
  ReplaceInstructions(F);
  CloneFunctionInto(NewFunction, &F, VMap,
                    CloneFunctionChangeType::GlobalChanges, Returns);

  // Set the attributes of the new function
  NewFunction->copyAttributesFrom(&F);
  NewFunction->setComdat(F.getComdat());
  NewFunction->takeName(&F);

  // Insert the new function in the module. We do it now in order to keep the
  // relative order of the functions in the module.
  F.getParent()->getFunctionList().insert(F.getIterator(), NewFunction);

  F.replaceAllUsesWith(NewFunction);
  F.eraseFromParent();
}

bool HasByteArgs(Function &F) {
  for (Argument &Arg : F.args()) {
    ArrayType *ATy = dyn_cast<ArrayType>(Arg.getType());
    if (ATy && ATy->getArrayElementType()->isByteOrByteVectorTy())
      return true;

    StructType *STy = dyn_cast<StructType>(Arg.getType());
    if (STy) {
      for (Type *ElTy : STy->elements())
        if (ElTy->isByteOrByteVectorTy())
          return true;
    }

    if (Arg.getType()->isByteOrByteVectorTy())
      return true;
  }
  return false;

}

bool HasByteReturn(Function &F) {
  return F.getReturnType()->isByteOrByteVectorTy();
}

bool UsesByteType(Function &F) {
  for (BasicBlock &BB : F) {
    for (Instruction &I : BB) {
      // Handle special cases
      if (auto *GEP = dyn_cast<GetElementPtrInst>(&I)) {
        ArrayType *ATy = dyn_cast<ArrayType>(GEP->getSourceElementType());
        if (GEP->getSourceElementType()->isByteOrByteVectorTy() ||
            (ATy && ATy->getArrayElementType()->isByteOrByteVectorTy())) {
          return true;
        }
      } else if (auto *AI = dyn_cast<AllocaInst>(&I))
        if (AI->getAllocatedType()->isByteOrByteVectorTy())
          return true;
      for (Use &U : I.operands()) {
        Value *Operand = U.get();
        if (Operand->getType()->isByteOrByteVectorTy())
          return true;
      }
    }
  }

  return HasByteArgs(F) || HasByteReturn(F);
}

PreservedAnalyses ByteReplacerPass::run(Module &M, ModuleAnalysisManager &) {
  // for (Function &F : M)
  //   llvm::errs() << F << "\n";

  // assert(false);
  // return PreservedAnalyses::all();

  SmallVector<Function *, 8> FunctionsToProcess;
  for (Function &F : make_early_inc_range(M)) {
    if (UsesByteType(F))
      ReplaceFunction(F);
  }

  // Anonymoys func to determine if global variable is byte or byte vector
  auto IsByteOrByteVector = [](GlobalVariable &GV) {
    if (auto *ATy = dyn_cast<ArrayType>(GV.getValueType()))
      return ATy->getElementType()->isByteOrByteVectorTy();
    return GV.getValueType()->isByteOrByteVectorTy();
  };

  for (GlobalVariable &GV : make_early_inc_range(M.globals())) {
    if (IsByteOrByteVector(GV)) {
      GlobalVariable *NewGV = nullptr;
      if (GV.hasInitializer()) {
      GV.getInitializer()->mutateType(
          GetIsomorphicType(GV.getInitializer()->getType()));
      NewGV = new GlobalVariable(
          M, GetIsomorphicType(GV.getValueType()), GV.isConstant(),
          GV.getLinkage(), GV.getInitializer(), GV.getName(), &GV,
          GV.getThreadLocalMode(), GV.getAddressSpace(),
          GV.isExternallyInitialized());
      } else {
        NewGV = new GlobalVariable(
            M, GetIsomorphicType(GV.getValueType()), GV.isConstant(),
            GV.getLinkage(), nullptr, GV.getName(), &GV,
            GV.getThreadLocalMode(), GV.getAddressSpace(),
            GV.isExternallyInitialized());
      }
      NewGV->takeName(&GV);
      NewGV->copyAttributesFrom(&GV);
      GV.replaceAllUsesWith(NewGV);
      GV.eraseFromParent();
    }
  }

  assert(!verifyModule(M, &llvm::errs()) && "Module verification failed");

  return PreservedAnalyses::none();
}
