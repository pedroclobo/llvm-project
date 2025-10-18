#ifndef LLVM_TRANSFORMS_UTILS_INSTRCOUNT_H
#define LLVM_TRANSFORMS_UTILS_INSTRCOUNT_H

#include "llvm/IR/PassManager.h"

namespace llvm {

class InstrCountPass : public PassInfoMixin<InstrCountPass> {
public:
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
};

} // namespace llvm

#endif // LLVM_TRANSFORMS_UTILS_INSTRCOUNT_H

