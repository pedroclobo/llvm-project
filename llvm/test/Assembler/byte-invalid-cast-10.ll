; RUN: not llvm-as -disable-output %s 2>&1 | FileCheck %s

; CHECK: invalid cast opcode for cast from 'b8' to 'i16'
define void @invalid_bytcast(b8 %b) {
  %t = bytecast b8 %b to i16
  ret void
}
