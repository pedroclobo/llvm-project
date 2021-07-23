; RUN: not llvm-as -disable-output %s 2>&1 | FileCheck %s

; CHECK: invalid cast opcode for cast from '<8 x b8>' to '<4 x i16>'
define void @invalid_bytcast(<8 x b8> %b) {
  %t = bytecast <8 x b8> %b to <4 x i16>
  ret void
}
