; RUN: not llvm-as -disable-output %s 2>&1 | FileCheck %s

; CHECK: invalid cast opcode for cast from '<4 x b32>' to '<4 x ptr>'
define void @invalid_bytcast(<4 x b32> %b) {
  %t = bytecast <4 x b32> %b to <4 x ptr>
  ret void
}
