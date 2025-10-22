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

  for (auto &BB : F) {
    for (auto &I : BB) {
      ++total;
      if (isa<BitCastInst>(&I)) {
        ++bitcasts;
      }
    }
  }

  LLVM_DEBUG(dbgs() << "remark: " << F.getName() << ";" << total << ";"
                    << bitcasts << "\n");

  return PreservedAnalyses::all();
}
