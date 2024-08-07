; NOTE: Assertions have been autogenerated by utils/update_llc_test_checks.py
; RUN: llc -mtriple=x86_64-unknown-unknown -mattr=+mmx < %s | FileCheck %s --check-prefix=X64
; RUN: llc -mtriple=i686-unknown-unknown -mattr=+mmx < %s | FileCheck %s --check-prefix=X86


; From source: clang -02
;__m64 test47(int a)
;{
;    __m64 x = (a)? (__m64)(7): (__m64)(0);
; return __builtin_ia32_psllw(x, x);
;}

define i64 @test47(i64 %arg)  {
;
; X64-LABEL: test47:
; X64:       # %bb.0:
; X64-NEXT:    xorl %eax, %eax
; X64-NEXT:    testq %rdi, %rdi
; X64-NEXT:    movl $7, %ecx
; X64-NEXT:    cmovneq %rax, %rcx
; X64-NEXT:    movq %rcx, %mm0
; X64-NEXT:    psllw %mm0, %mm0
; X64-NEXT:    movq %mm0, %rax
; X64-NEXT:    retq
;
; X86-LABEL: test47:
; X86:       # %bb.0:
; X86-NEXT:    pushl %ebp
; X86-NEXT:    .cfi_def_cfa_offset 8
; X86-NEXT:    .cfi_offset %ebp, -8
; X86-NEXT:    movl %esp, %ebp
; X86-NEXT:    .cfi_def_cfa_register %ebp
; X86-NEXT:    andl $-8, %esp
; X86-NEXT:    subl $16, %esp
; X86-NEXT:    movl 8(%ebp), %eax
; X86-NEXT:    orl 12(%ebp), %eax
; X86-NEXT:    movl $7, %eax
; X86-NEXT:    je .LBB0_2
; X86-NEXT:  # %bb.1:
; X86-NEXT:    xorl %eax, %eax
; X86-NEXT:  .LBB0_2:
; X86-NEXT:    movl %eax, {{[0-9]+}}(%esp)
; X86-NEXT:    movl $0, {{[0-9]+}}(%esp)
; X86-NEXT:    movq {{[0-9]+}}(%esp), %mm0
; X86-NEXT:    psllw %mm0, %mm0
; X86-NEXT:    movq %mm0, (%esp)
; X86-NEXT:    movl (%esp), %eax
; X86-NEXT:    movl {{[0-9]+}}(%esp), %edx
; X86-NEXT:    movl %ebp, %esp
; X86-NEXT:    popl %ebp
; X86-NEXT:    .cfi_def_cfa %esp, 4
; X86-NEXT:    retl
  %cond = icmp eq i64 %arg, 0
  %slct = select i1 %cond, <1 x i64> bitcast (i64 7 to <1 x i64>), <1 x i64> bitcast (i64 0 to <1 x i64>)
  %psll = tail call <1 x i64> @llvm.x86.mmx.psll.w(<1 x i64> %slct, <1 x i64> %slct)
  %retc = bitcast <1 x i64> %psll to i64
  ret i64 %retc
}


; From source: clang -O2
;__m64 test49(int a, long long n, long long m)
;{
;    __m64 x = (a)? (__m64)(n): (__m64)(m);
; return __builtin_ia32_psllw(x, x);
;}

define i64 @test49(i64 %arg, i64 %x, i64 %y) {
;
; X64-LABEL: test49:
; X64:       # %bb.0:
; X64-NEXT:    testq %rdi, %rdi
; X64-NEXT:    cmovneq %rdx, %rsi
; X64-NEXT:    movq %rsi, %mm0
; X64-NEXT:    psllw %mm0, %mm0
; X64-NEXT:    movq %mm0, %rax
; X64-NEXT:    retq
;
; X86-LABEL: test49:
; X86:       # %bb.0:
; X86-NEXT:    pushl %ebp
; X86-NEXT:    .cfi_def_cfa_offset 8
; X86-NEXT:    .cfi_offset %ebp, -8
; X86-NEXT:    movl %esp, %ebp
; X86-NEXT:    .cfi_def_cfa_register %ebp
; X86-NEXT:    andl $-8, %esp
; X86-NEXT:    subl $8, %esp
; X86-NEXT:    movl 8(%ebp), %eax
; X86-NEXT:    orl 12(%ebp), %eax
; X86-NEXT:    je .LBB1_1
; X86-NEXT:  # %bb.2:
; X86-NEXT:    leal 24(%ebp), %eax
; X86-NEXT:    jmp .LBB1_3
; X86-NEXT:  .LBB1_1:
; X86-NEXT:    leal 16(%ebp), %eax
; X86-NEXT:  .LBB1_3:
; X86-NEXT:    movq (%eax), %mm0
; X86-NEXT:    psllw %mm0, %mm0
; X86-NEXT:    movq %mm0, (%esp)
; X86-NEXT:    movl (%esp), %eax
; X86-NEXT:    movl {{[0-9]+}}(%esp), %edx
; X86-NEXT:    movl %ebp, %esp
; X86-NEXT:    popl %ebp
; X86-NEXT:    .cfi_def_cfa %esp, 4
; X86-NEXT:    retl
  %cond = icmp eq i64 %arg, 0
  %xmmx = bitcast i64 %x to <1 x i64>
  %ymmx = bitcast i64 %y to <1 x i64>
  %slct = select i1 %cond, <1 x i64> %xmmx, <1 x i64> %ymmx
  %psll = tail call <1 x i64> @llvm.x86.mmx.psll.w(<1 x i64> %slct, <1 x i64> %slct)
  %retc = bitcast <1 x i64> %psll to i64
  ret i64 %retc
}

declare <1 x i64> @llvm.x86.mmx.psll.w(<1 x i64>, <1 x i64>)

