; Test strict conversions of unsigned i64s to floating-point values (z10 only).
;
; RUN: llc < %s -mtriple=s390x-linux-gnu -mcpu=z10 | FileCheck %s

declare half @llvm.experimental.constrained.uitofp.f16.i64(i64, metadata, metadata)
declare float @llvm.experimental.constrained.uitofp.f32.i64(i64, metadata, metadata)
declare double @llvm.experimental.constrained.uitofp.f64.i64(i64, metadata, metadata)
declare fp128 @llvm.experimental.constrained.uitofp.f128.i64(i64, metadata, metadata)

; Test i64->f16. For z10, this results in just a single a libcall.
define half @f0(i64 %i) #0 {
; CHECK-LABEL: f0:
; CHECK: cegbr
; CHECK: aebr
; CHECK: brasl %r14, __truncsfhf2@PLT
; CHECK: br %r14
  %conv = call half @llvm.experimental.constrained.uitofp.f16.i64(i64 %i,
                                               metadata !"round.dynamic",
                                               metadata !"fpexcept.strict") #0
  ret half %conv
}

; Test i64->f32.  There's no native support for unsigned i64-to-fp conversions,
; but we should be able to implement them using signed i64-to-fp conversions.
define float @f1(i64 %i) #0 {
; CHECK-LABEL: f1:
; CHECK: cegbr
; CHECK: aebr
; CHECK: br %r14
  %conv = call float @llvm.experimental.constrained.uitofp.f32.i64(i64 %i,
                                               metadata !"round.dynamic",
                                               metadata !"fpexcept.strict") #0
  ret float %conv
}

; Test i64->f64.
define double @f2(i64 %i) #0 {
; CHECK-LABEL: f2:
; CHECK: cdgbr
; CHECK: adbr
; CHECK: br %r14
  %conv = call double @llvm.experimental.constrained.uitofp.f64.i64(i64 %i,
                                               metadata !"round.dynamic",
                                               metadata !"fpexcept.strict") #0
  ret double %conv
}

; Test i64->f128.
define void @f3(i64 %i, ptr %dst) #0 {
; CHECK-LABEL: f3:
; CHECK: cxgbr
; CHECK: axbr
; CHECK: br %r14
  %conv = call fp128 @llvm.experimental.constrained.uitofp.f128.i64(i64 %i,
                                               metadata !"round.dynamic",
                                               metadata !"fpexcept.strict") #0
  store fp128 %conv, ptr %dst
  ret void
}

attributes #0 = { strictfp }
