//===-- ByteReplacer.h - Pass to inline "always_inline" functions --------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
///
/// \file
/// Provides passes to inlining "always_inline" functions.
///
//===----------------------------------------------------------------------===//

#ifndef LLVM_TRANSFORMS_IPO_BYTEREPLACER_H
#define LLVM_TRANSFORMS_IPO_BYTEREPLACER_H

#include "llvm/IR/PassManager.h"

namespace llvm {

class Module;
class Pass;

class ByteReplacerPass : public PassInfoMixin<ByteReplacerPass> {
public:
  ByteReplacerPass() {}

  PreservedAnalyses run(Module &M, ModuleAnalysisManager &);
  static bool isRequired() { return false; }
};

} // namespace llvm

#endif // LLVM_TRANSFORMS_IPO_BYTEREPLACER_H
