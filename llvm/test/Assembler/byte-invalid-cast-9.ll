; RUN: not llvm-as -disable-output %s 2>&1 | FileCheck %s

; CHECK: invalid cast opcode for cast from 'b8' to 'ptr'
define void @invalid_bytcast(b8 %b) {
  %t = bytecast b8 %b to ptr
  ret void
}
