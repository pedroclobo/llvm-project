target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define b16 @_ZN4llvm9DwarfUnit7addSIntERNS_6DIELocESt8optionalINS_5dwarf4FormEEl(i64 %Integer, i1 %cmp.i.i) {
entry:
  br i1 %cmp.i.i, label %_ZN4llvm10DIEInteger8BestFormEbm.exit.i, label %if.end.i.i

if.end.i.i:                                       ; preds = %entry
  %cmp6.i.i = icmp eq i64 %Integer, 0
  br i1 %cmp6.i.i, label %_ZN4llvm10DIEInteger8BestFormEbm.exit.i, label %common.ret1

common.ret1:                                      ; preds = %if.end.i.i
  %common.ret1.op = phi b16 [ 0, %if.end.i.i ]
  ret b16 %common.ret1.op

_ZN4llvm10DIEInteger8BestFormEbm.exit.i:          ; preds = %if.end.i.i, %entry
  %retval.1.i.i = phi b32 [ 0, %if.end.i.i ], [ 65547, %entry ]
  %0 = trunc b32 %retval.1.i.i to b16
  ret b16 %0
}
