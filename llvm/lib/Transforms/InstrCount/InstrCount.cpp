#include "llvm/Transforms/Utils/InstrCount.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

#define DEBUG_TYPE "instr-count"

PreservedAnalyses InstrCountPass::run(Function &F,
                                      FunctionAnalysisManager &AM) {
  uint64_t total = 0;
  uint64_t bitcasts = 0;
  uint64_t bitcasts_to_byte = 0;
  uint64_t trunc_bitcast_to_byte = 0;
  uint64_t bytecasts = 0;
  uint64_t bytecasts_exact = 0;
  uint64_t bytecast_int = 0;
  uint64_t bytecast_ptr = 0;
  uint64_t bytecast_trunc = 0;
  uint64_t bytecast_sext = 0;
  uint64_t bytecast_zext = 0;
  uint64_t bytecast_icmp = 0;

  for (auto &BB : F) {
    for (auto &I : BB) {
      ++total;
      if (auto *BC = dyn_cast<ByteCastInst>(&I)) {
        ++bytecasts;
        if (BC->isExact())
          ++bytecasts_exact;
        if (BC->getDestTy()->isIntOrIntVectorTy())
          ++bytecast_int;
        if (BC->getDestTy()->isPtrOrPtrVectorTy())
          ++bytecast_ptr;
      }
      if (auto *BC = dyn_cast<BitCastInst>(&I)) {
        ++bitcasts;
        if (BC->getDestTy()->isByteOrByteVectorTy())
          ++bitcasts_to_byte;
      }
      if (isa<TruncInst>(&I)) {
        if (I.hasOneUse()) {
          if (auto *BC = dyn_cast<BitCastInst>(*I.user_begin())) {
            if (BC->getDestTy()->isByteOrByteVectorTy())
              ++trunc_bitcast_to_byte;
          }
        }
      }
      if (isa<ByteCastInst>(&I)) {
        if (I.hasOneUse()) {
          if (isa<TruncInst>(*I.user_begin())) {
            ++bytecast_trunc;
          } else if (isa<SExtInst>(*I.user_begin())) {
            ++bytecast_sext;
          } else if (isa<ZExtInst>(*I.user_begin())) {
            ++bytecast_zext;
          } else if (isa<ICmpInst>(*I.user_begin())) {
            ++bytecast_icmp;
          }
        }
      }
    }
  }

  LLVM_DEBUG(dbgs() << "remark: " << F.getName() << ";" << total << ";" << bitcasts << ";"
                    << bitcasts_to_byte << ";" << trunc_bitcast_to_byte << ";"
                    << bytecasts << ";" << bytecasts_exact << ";"
                    << bytecast_int << ";" << bytecast_ptr << ";"
                    << bytecast_trunc << ";" << bytecast_sext << ";"
                    << bytecast_zext << ";" << bytecast_icmp << "\n");

  return PreservedAnalyses::all();
}
