//=- NSErrorChecker.cpp - Coding conventions for uses of NSError -*- C++ -*-==//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
//  This file defines a CheckNSError, a flow-insensitive check
//  that determines if an Objective-C class interface correctly returns
//  a non-void return type.
//
//  File under feature request PR 2600.
//
//===----------------------------------------------------------------------===//

#include "clang/StaticAnalyzer/Checkers/BuiltinCheckerRegistration.h"
#include "clang/AST/Decl.h"
#include "clang/AST/DeclObjC.h"
#include "clang/StaticAnalyzer/Core/BugReporter/BugType.h"
#include "clang/StaticAnalyzer/Core/Checker.h"
#include "clang/StaticAnalyzer/Core/CheckerManager.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/CheckerContext.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/ProgramStateTrait.h"
#include "llvm/ADT/SmallString.h"
#include "llvm/Support/raw_ostream.h"
#include <optional>

using namespace clang;
using namespace ento;

static bool IsNSError(QualType T, IdentifierInfo *II);
static bool IsCFError(QualType T, IdentifierInfo *II);

//===----------------------------------------------------------------------===//
// NSErrorMethodChecker
//===----------------------------------------------------------------------===//

namespace {
class NSErrorMethodChecker
    : public Checker< check::ASTDecl<ObjCMethodDecl> > {
  mutable IdentifierInfo *II = nullptr;

public:
  NSErrorMethodChecker() = default;

  void checkASTDecl(const ObjCMethodDecl *D,
                    AnalysisManager &mgr, BugReporter &BR) const;
};
}

void NSErrorMethodChecker::checkASTDecl(const ObjCMethodDecl *D,
                                        AnalysisManager &mgr,
                                        BugReporter &BR) const {
  if (!D->isThisDeclarationADefinition())
    return;
  if (!D->getReturnType()->isVoidType())
    return;

  if (!II)
    II = &D->getASTContext().Idents.get("NSError");

  bool hasNSError = false;
  for (const auto *I : D->parameters())  {
    if (IsNSError(I->getType(), II)) {
      hasNSError = true;
      break;
    }
  }

  if (hasNSError) {
    const char *err = "Method accepting NSError** "
        "should have a non-void return value to indicate whether or not an "
        "error occurred";
    PathDiagnosticLocation L =
      PathDiagnosticLocation::create(D, BR.getSourceManager());
    BR.EmitBasicReport(D, this, "Bad return type when passing NSError**",
                       "Coding conventions (Apple)", err, L);
  }
}

//===----------------------------------------------------------------------===//
// CFErrorFunctionChecker
//===----------------------------------------------------------------------===//

namespace {
class CFErrorFunctionChecker
    : public Checker< check::ASTDecl<FunctionDecl> > {
  mutable IdentifierInfo *II;

public:
  CFErrorFunctionChecker() : II(nullptr) {}

  void checkASTDecl(const FunctionDecl *D,
                    AnalysisManager &mgr, BugReporter &BR) const;
};
}

static bool hasReservedReturnType(const FunctionDecl *D) {
  if (isa<CXXConstructorDecl>(D))
    return true;

  // operators delete and delete[] are required to have 'void' return type
  auto OperatorKind = D->getOverloadedOperator();
  return OperatorKind == OO_Delete || OperatorKind == OO_Array_Delete;
}

void CFErrorFunctionChecker::checkASTDecl(const FunctionDecl *D,
                                        AnalysisManager &mgr,
                                        BugReporter &BR) const {
  if (!D->doesThisDeclarationHaveABody())
    return;
  if (!D->getReturnType()->isVoidType())
    return;
  if (hasReservedReturnType(D))
    return;

  if (!II)
    II = &D->getASTContext().Idents.get("CFErrorRef");

  bool hasCFError = false;
  for (auto *I : D->parameters())  {
    if (IsCFError(I->getType(), II)) {
      hasCFError = true;
      break;
    }
  }

  if (hasCFError) {
    const char *err = "Function accepting CFErrorRef* "
        "should have a non-void return value to indicate whether or not an "
        "error occurred";
    PathDiagnosticLocation L =
      PathDiagnosticLocation::create(D, BR.getSourceManager());
    BR.EmitBasicReport(D, this, "Bad return type when passing CFErrorRef*",
                       "Coding conventions (Apple)", err, L);
  }
}

//===----------------------------------------------------------------------===//
// NSOrCFErrorDerefChecker
//===----------------------------------------------------------------------===//

namespace {
class NSOrCFErrorDerefChecker
    : public CheckerFamily<check::Location,
                           check::Event<ImplicitNullDerefEvent>> {
  mutable IdentifierInfo *NSErrorII = nullptr;
  mutable IdentifierInfo *CFErrorII = nullptr;

public:
  CheckerFrontendWithBugType NSError{"NSError** null dereference",
                                     "Coding conventions (Apple)"};
  CheckerFrontendWithBugType CFError{"CFErrorRef* null dereference",
                                     "Coding conventions (Apple)"};

  StringRef getDebugTag() const override { return "NSOrCFErrorDerefChecker"; }

  void checkLocation(SVal loc, bool isLoad, const Stmt *S,
                     CheckerContext &C) const;
  void checkEvent(ImplicitNullDerefEvent event) const;
};
}

typedef llvm::ImmutableMap<SymbolRef, unsigned> ErrorOutFlag;
REGISTER_TRAIT_WITH_PROGRAMSTATE(NSErrorOut, ErrorOutFlag)
REGISTER_TRAIT_WITH_PROGRAMSTATE(CFErrorOut, ErrorOutFlag)

template <typename T>
static bool hasFlag(SVal val, ProgramStateRef state) {
  if (SymbolRef sym = val.getAsSymbol())
    if (const unsigned *attachedFlags = state->get<T>(sym))
      return *attachedFlags;
  return false;
}

template <typename T>
static void setFlag(ProgramStateRef state, SVal val, CheckerContext &C) {
  // We tag the symbol that the SVal wraps.
  if (SymbolRef sym = val.getAsSymbol())
    C.addTransition(state->set<T>(sym, true));
}

static QualType parameterTypeFromSVal(SVal val, CheckerContext &C) {
  const StackFrameContext * SFC = C.getStackFrame();
  if (std::optional<loc::MemRegionVal> X = val.getAs<loc::MemRegionVal>()) {
    const MemRegion* R = X->getRegion();
    if (const VarRegion *VR = R->getAs<VarRegion>())
      if (const auto *StackSpace =
              VR->getMemorySpaceAs<StackArgumentsSpaceRegion>(C.getState()))
        if (StackSpace->getStackFrame() == SFC)
          return VR->getValueType();
  }

  return QualType();
}

void NSOrCFErrorDerefChecker::checkLocation(SVal loc, bool isLoad,
                                            const Stmt *S,
                                            CheckerContext &C) const {
  if (!isLoad)
    return;
  if (loc.isUndef() || !isa<Loc>(loc))
    return;

  ASTContext &Ctx = C.getASTContext();
  ProgramStateRef state = C.getState();

  // If we are loading from NSError**/CFErrorRef* parameter, mark the resulting
  // SVal so that we can later check it when handling the
  // ImplicitNullDerefEvent event.
  // FIXME: Cumbersome! Maybe add hook at construction of SVals at start of
  // function ?

  QualType parmT = parameterTypeFromSVal(loc, C);
  if (parmT.isNull())
    return;

  if (!NSErrorII)
    NSErrorII = &Ctx.Idents.get("NSError");
  if (!CFErrorII)
    CFErrorII = &Ctx.Idents.get("CFErrorRef");

  if (NSError.isEnabled() && IsNSError(parmT, NSErrorII)) {
    setFlag<NSErrorOut>(state, state->getSVal(loc.castAs<Loc>()), C);
    return;
  }

  if (CFError.isEnabled() && IsCFError(parmT, CFErrorII)) {
    setFlag<CFErrorOut>(state, state->getSVal(loc.castAs<Loc>()), C);
    return;
  }
}

void NSOrCFErrorDerefChecker::checkEvent(ImplicitNullDerefEvent event) const {
  if (event.IsLoad)
    return;

  SVal loc = event.Location;
  ProgramStateRef state = event.SinkNode->getState();
  BugReporter &BR = *event.BR;

  bool isNSError = hasFlag<NSErrorOut>(loc, state);
  bool isCFError = false;
  if (!isNSError)
    isCFError = hasFlag<CFErrorOut>(loc, state);

  if (!(isNSError || isCFError))
    return;

  // Storing to possible null NSError/CFErrorRef out parameter.
  SmallString<128> Buf;
  llvm::raw_svector_ostream os(Buf);

  os << "Potential null dereference. According to coding standards ";
  os << (isNSError
         ? "in 'Creating and Returning NSError Objects' the parameter"
         : "documented in CoreFoundation/CFError.h the parameter");

  os  << " may be null";

  const BugType &BT = isNSError ? NSError : CFError;
  BR.emitReport(
      std::make_unique<PathSensitiveBugReport>(BT, os.str(), event.SinkNode));
}

static bool IsNSError(QualType T, IdentifierInfo *II) {

  const PointerType* PPT = T->getAs<PointerType>();
  if (!PPT)
    return false;

  const ObjCObjectPointerType* PT =
    PPT->getPointeeType()->getAs<ObjCObjectPointerType>();

  if (!PT)
    return false;

  const ObjCInterfaceDecl *ID = PT->getInterfaceDecl();

  // FIXME: Can ID ever be NULL?
  if (ID)
    return II == ID->getIdentifier();

  return false;
}

static bool IsCFError(QualType T, IdentifierInfo *II) {
  const PointerType* PPT = T->getAs<PointerType>();
  if (!PPT) return false;

  const TypedefType* TT = PPT->getPointeeType()->getAs<TypedefType>();
  if (!TT) return false;

  return TT->getDecl()->getIdentifier() == II;
}

// This source file implements two user-facing checkers ("osx.cocoa.NSError"
// and "osx.coreFoundation.CFError") which are both implemented as the
// combination of two `CheckerFrontend`s that are registered under the same
// name (but otherwise act independently). Among these 2+2 `CheckerFrontend`s
// two are coming from the checker family `NSOrCFErrorDerefChecker` while the
// other two (the `ADDITIONAL_PART`s) are small standalone checkers.
#define REGISTER_CHECKER(NAME, ADDITIONAL_PART)                                \
  void ento::register##NAME##Checker(CheckerManager &Mgr) {                    \
    Mgr.getChecker<NSOrCFErrorDerefChecker>()->NAME.enable(Mgr);               \
    Mgr.registerChecker<ADDITIONAL_PART>();                                    \
  }                                                                            \
                                                                               \
  bool ento::shouldRegister##NAME##Checker(const CheckerManager &) {           \
    return true;                                                               \
  }

REGISTER_CHECKER(NSError, NSErrorMethodChecker)
REGISTER_CHECKER(CFError, CFErrorFunctionChecker)
