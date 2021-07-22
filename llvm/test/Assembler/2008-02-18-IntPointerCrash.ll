; RUN: not llvm-as %s 2>&1 | grep "integer/byte constant must have integer/byte type"
; PR2060

define ptr @foo() {
       ret ptr 0
}
