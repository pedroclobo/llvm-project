; NOTE: Assertions have been autogenerated by utils/update_test_checks.py
; RUN: opt < %s -passes=instcombine -mtriple=x86_64-unknown-unknown -S | FileCheck %s

;
; UNDEF Elts
;

define <8 x i16> @undef_pmaddubsw_128(<16 x i8> %a0) {
; CHECK-LABEL: @undef_pmaddubsw_128(
; CHECK-NEXT:    ret <8 x i16> zeroinitializer
;
  %1 = call <8 x i16> @llvm.x86.ssse3.pmadd.ub.sw.128(<16 x i8> %a0, <16 x i8> undef)
  ret <8 x i16> %1
}

define <8 x i16> @undef_pmaddubsw_128_commute(<16 x i8> %a0) {
; CHECK-LABEL: @undef_pmaddubsw_128_commute(
; CHECK-NEXT:    ret <8 x i16> zeroinitializer
;
  %1 = call <8 x i16> @llvm.x86.ssse3.pmadd.ub.sw.128(<16 x i8> undef, <16 x i8> %a0)
  ret <8 x i16> %1
}

define <16 x i16> @undef_pmaddubsw_256(<32 x i8> %a0) {
; CHECK-LABEL: @undef_pmaddubsw_256(
; CHECK-NEXT:    ret <16 x i16> zeroinitializer
;
  %1 = call <16 x i16> @llvm.x86.avx2.pmadd.ub.sw(<32 x i8> %a0, <32 x i8> undef)
  ret <16 x i16> %1
}

define <16 x i16> @undef_pmaddubsw_256_commute(<32 x i8> %a0) {
; CHECK-LABEL: @undef_pmaddubsw_256_commute(
; CHECK-NEXT:    ret <16 x i16> zeroinitializer
;
  %1 = call <16 x i16> @llvm.x86.avx2.pmadd.ub.sw(<32 x i8> undef, <32 x i8> %a0)
  ret <16 x i16> %1
}

define <32 x i16> @undef_pmaddubsw_512(<64 x i8> %a0) {
; CHECK-LABEL: @undef_pmaddubsw_512(
; CHECK-NEXT:    ret <32 x i16> zeroinitializer
;
  %1 = call <32 x i16> @llvm.x86.avx512.pmaddubs.w.512(<64 x i8> %a0, <64 x i8> undef)
  ret <32 x i16> %1
}

define <32 x i16> @undef_pmaddubsw_512_commute(<64 x i8> %a0) {
; CHECK-LABEL: @undef_pmaddubsw_512_commute(
; CHECK-NEXT:    ret <32 x i16> zeroinitializer
;
  %1 = call <32 x i16> @llvm.x86.avx512.pmaddubs.w.512(<64 x i8> undef, <64 x i8> %a0)
  ret <32 x i16> %1
}

;
; Zero Elts
;

define <8 x i16> @zero_pmaddubsw_128(<16 x i8> %a0) {
; CHECK-LABEL: @zero_pmaddubsw_128(
; CHECK-NEXT:    ret <8 x i16> zeroinitializer
;
  %1 = call <8 x i16> @llvm.x86.ssse3.pmadd.ub.sw.128(<16 x i8> %a0, <16 x i8> zeroinitializer)
  ret <8 x i16> %1
}

define <8 x i16> @zero_pmaddubsw_128_commute(<16 x i8> %a0) {
; CHECK-LABEL: @zero_pmaddubsw_128_commute(
; CHECK-NEXT:    ret <8 x i16> zeroinitializer
;
  %1 = call <8 x i16> @llvm.x86.ssse3.pmadd.ub.sw.128(<16 x i8> zeroinitializer, <16 x i8> %a0)
  ret <8 x i16> %1
}

define <16 x i16> @zero_pmaddubsw_256(<32 x i8>%a0) {
; CHECK-LABEL: @zero_pmaddubsw_256(
; CHECK-NEXT:    ret <16 x i16> zeroinitializer
;
  %1 = call <16 x i16> @llvm.x86.avx2.pmadd.ub.sw(<32 x i8> %a0, <32 x i8> zeroinitializer)
  ret <16 x i16> %1
}

define <16 x i16> @zero_pmaddubsw_256_commute(<32 x i8> %a0) {
; CHECK-LABEL: @zero_pmaddubsw_256_commute(
; CHECK-NEXT:    ret <16 x i16> zeroinitializer
;
  %1 = call <16 x i16> @llvm.x86.avx2.pmadd.ub.sw(<32 x i8> zeroinitializer, <32 x i8> %a0)
  ret <16 x i16> %1
}

define <32 x i16> @zero_pmaddubsw_512(<64 x i8> %a0) {
; CHECK-LABEL: @zero_pmaddubsw_512(
; CHECK-NEXT:    ret <32 x i16> zeroinitializer
;
  %1 = call <32 x i16> @llvm.x86.avx512.pmaddubs.w.512(<64 x i8> %a0, <64 x i8> zeroinitializer)
  ret <32 x i16> %1
}

define <32 x i16> @zero_pmaddubsw_512_commute(<64 x i8> %a0) {
; CHECK-LABEL: @zero_pmaddubsw_512_commute(
; CHECK-NEXT:    ret <32 x i16> zeroinitializer
;
  %1 = call <32 x i16> @llvm.x86.avx512.pmaddubs.w.512(<64 x i8> zeroinitializer, <64 x i8> %a0)
  ret <32 x i16> %1
}

;
; Constant Folding
;

define <8 x i16> @fold_pmaddubsw_128() {
; CHECK-LABEL: @fold_pmaddubsw_128(
; CHECK-NEXT:    ret <8 x i16> <i16 -32768, i16 18, i16 50, i16 1694, i16 162, i16 242, i16 338, i16 450>
;
  %1 = call <8 x i16> @llvm.x86.ssse3.pmadd.ub.sw.128(<16 x i8> <i8 -1, i8 -1, i8 2, i8 3, i8 4, i8 5, i8 -6, i8 7, i8 8, i8 9, i8 10, i8 11, i8 12, i8 13, i8 14, i8 15>, <16 x i8> <i8 -128, i8 -128, i8 3, i8 4, i8 5, i8 6, i8 7, i8 -8, i8 9, i8 10, i8 11, i8 12, i8 13, i8 14, i8 15, i8 16>)
  ret <8 x i16> %1
}

define <16 x i16> @fold_pmaddubsw_256() {
; CHECK-LABEL: @fold_pmaddubsw_256(
; CHECK-NEXT:    ret <16 x i16> <i16 -32768, i16 18, i16 50, i16 1694, i16 162, i16 242, i16 338, i16 450, i16 -256, i16 18, i16 50, i16 1694, i16 162, i16 242, i16 338, i16 450>
;
  %1 = call <16 x i16> @llvm.x86.avx2.pmadd.ub.sw(<32 x i8> <i8 -1, i8 -1, i8 2, i8 3, i8 4, i8 5, i8 -6, i8 7, i8 8, i8 9, i8 10, i8 11, i8 12, i8 13, i8 14, i8 15, i8 -128, i8 -128, i8 3, i8 4, i8 5, i8 6, i8 7, i8 -8, i8 9, i8 10, i8 11, i8 12, i8 13, i8 14, i8 15, i8 16>, <32 x i8> <i8 -128, i8 -128, i8 3, i8 4, i8 5, i8 6, i8 7, i8 -8, i8 9, i8 10, i8 11, i8 12, i8 13, i8 14, i8 15, i8 16, i8 -1, i8 -1, i8 2, i8 3, i8 4, i8 5, i8 -6, i8 7, i8 8, i8 9, i8 10, i8 11, i8 12, i8 13, i8 14, i8 15>)
  ret <16 x i16> %1
}

define <32 x i16> @fold_pmaddubsw_512() {
; CHECK-LABEL: @fold_pmaddubsw_512(
; CHECK-NEXT:    ret <32 x i16> <i16 -32768, i16 18, i16 50, i16 1694, i16 162, i16 242, i16 338, i16 450, i16 -256, i16 18, i16 50, i16 1694, i16 162, i16 242, i16 338, i16 450, i16 -256, i16 18, i16 50, i16 1694, i16 162, i16 242, i16 338, i16 450, i16 -32768, i16 18, i16 50, i16 1694, i16 162, i16 242, i16 338, i16 450>
;
  %1 = call <32 x i16> @llvm.x86.avx512.pmaddubs.w.512(<64 x i8> <i8 -1, i8 -1, i8 2, i8 3, i8 4, i8 5, i8 -6, i8 7, i8 8, i8 9, i8 10, i8 11, i8 12, i8 13, i8 14, i8 15, i8 -128, i8 -128, i8 3, i8 4, i8 5, i8 6, i8 7, i8 -8, i8 9, i8 10, i8 11, i8 12, i8 13, i8 14, i8 15, i8 16, i8 -128, i8 -128, i8 3, i8 4, i8 5, i8 6, i8 7, i8 -8, i8 9, i8 10, i8 11, i8 12, i8 13, i8 14, i8 15, i8 16, i8 -1, i8 -1, i8 2, i8 3, i8 4, i8 5, i8 -6, i8 7, i8 8, i8 9, i8 10, i8 11, i8 12, i8 13, i8 14, i8 15>, <64 x i8> <i8 -128, i8 -128, i8 3, i8 4, i8 5, i8 6, i8 7, i8 -8, i8 9, i8 10, i8 11, i8 12, i8 13, i8 14, i8 15, i8 16, i8 -1, i8 -1, i8 2, i8 3, i8 4, i8 5, i8 -6, i8 7, i8 8, i8 9, i8 10, i8 11, i8 12, i8 13, i8 14, i8 15, i8 -1, i8 -1, i8 2, i8 3, i8 4, i8 5, i8 -6, i8 7, i8 8, i8 9, i8 10, i8 11, i8 12, i8 13, i8 14, i8 15, i8 -128, i8 -128, i8 3, i8 4, i8 5, i8 6, i8 7, i8 -8, i8 9, i8 10, i8 11, i8 12, i8 13, i8 14, i8 15, i8 16>)
  ret <32 x i16> %1
}

;
; Demanded Elts
;

define <8 x i16> @elts_pmaddubsw_128(<16 x i8> %a0, <16 x i8> %a1) {
; CHECK-LABEL: @elts_pmaddubsw_128(
; CHECK-NEXT:    [[TMP1:%.*]] = call <8 x i16> @llvm.x86.ssse3.pmadd.ub.sw.128(<16 x i8> [[A0:%.*]], <16 x i8> [[A1:%.*]])
; CHECK-NEXT:    [[TMP2:%.*]] = shufflevector <8 x i16> [[TMP1]], <8 x i16> poison, <8 x i32> zeroinitializer
; CHECK-NEXT:    ret <8 x i16> [[TMP2]]
;
  %1 = shufflevector <16 x i8> %a0, <16 x i8> undef, <16 x i32> <i32 0, i32 1, i32 7, i32 6, i32 5, i32 4, i32 3, i32 2, i32 8, i32 9, i32 10, i32 11, i32 12, i32 13, i32 14, i32 15>
  %2 = shufflevector <16 x i8> %a1, <16 x i8> undef, <16 x i32> <i32 0, i32 1, i32 3, i32 2, i32 5, i32 4, i32 7, i32 6, i32 8, i32 9, i32 10, i32 11, i32 12, i32 13, i32 14, i32 15>
  %3 = call <8 x i16> @llvm.x86.ssse3.pmadd.ub.sw.128(<16 x i8> %1, <16 x i8> %2)
  %4 = shufflevector <8 x i16> %3, <8 x i16> poison, <8 x i32> zeroinitializer
  ret <8 x i16> %4
}

define <16 x i16> @elts_pmaddubsw_256(<32 x i8> %a0, <32 x i8> %a1) {
; CHECK-LABEL: @elts_pmaddubsw_256(
; CHECK-NEXT:    [[TMP1:%.*]] = call <16 x i16> @llvm.x86.avx2.pmadd.ub.sw(<32 x i8> [[A0:%.*]], <32 x i8> [[A1:%.*]])
; CHECK-NEXT:    [[TMP2:%.*]] = shufflevector <16 x i16> [[TMP1]], <16 x i16> poison, <16 x i32> zeroinitializer
; CHECK-NEXT:    ret <16 x i16> [[TMP2]]
;
  %1 = shufflevector <32 x i8> %a0, <32 x i8> undef, <32 x i32> <i32 0, i32 1, i32 7, i32 6, i32 5, i32 4, i32 3, i32 2, i32 8, i32 9, i32 10, i32 11, i32 12, i32 13, i32 14, i32 15, i32 16, i32 17, i32 18, i32 19, i32 20, i32 21, i32 22, i32 23, i32 24, i32 25, i32 26, i32 27, i32 28, i32 29, i32 30, i32 31>
  %2 = shufflevector <32 x i8> %a1, <32 x i8> undef, <32 x i32> <i32 0, i32 1, i32 3, i32 2, i32 5, i32 4, i32 7, i32 6, i32 8, i32 9, i32 10, i32 11, i32 12, i32 13, i32 14, i32 15, i32 16, i32 17, i32 18, i32 19, i32 20, i32 21, i32 22, i32 23, i32 24, i32 25, i32 26, i32 27, i32 28, i32 29, i32 30, i32 31>
  %3 = call <16 x i16> @llvm.x86.avx2.pmadd.ub.sw(<32 x i8> %1, <32 x i8> %2)
  %4 = shufflevector <16 x i16> %3, <16 x i16> poison, <16 x i32> zeroinitializer
  ret <16 x i16> %4
}

define <32 x i16> @elts_pmaddubsw_512(<64 x i8> %a0, <64 x i8> %a1) {
; CHECK-LABEL: @elts_pmaddubsw_512(
; CHECK-NEXT:    [[TMP1:%.*]] = call <32 x i16> @llvm.x86.avx512.pmaddubs.w.512(<64 x i8> [[A0:%.*]], <64 x i8> [[A1:%.*]])
; CHECK-NEXT:    [[TMP2:%.*]] = shufflevector <32 x i16> [[TMP1]], <32 x i16> poison, <32 x i32> zeroinitializer
; CHECK-NEXT:    ret <32 x i16> [[TMP2]]
;
  %1 = shufflevector <64 x i8> %a0, <64 x i8> undef, <64 x i32> <i32 0, i32 1, i32 7, i32 6, i32 5, i32 4, i32 3, i32 2, i32 8, i32 9, i32 10, i32 11, i32 12, i32 13, i32 14, i32 15, i32 16, i32 17, i32 18, i32 19, i32 20, i32 21, i32 22, i32 23, i32 24, i32 25, i32 26, i32 27, i32 28, i32 29, i32 30, i32 31, i32 63, i32 62, i32 61, i32 60, i32 59, i32 58, i32 57, i32 56, i32 55, i32 54, i32 53, i32 52, i32 51, i32 50, i32 49, i32 48, i32 47, i32 46, i32 45, i32 44, i32 43, i32 42, i32 41, i32 40, i32 39, i32 38, i32 37, i32 36, i32 35, i32 34, i32 33, i32 32>
  %2 = shufflevector <64 x i8> %a1, <64 x i8> undef, <64 x i32> <i32 0, i32 1, i32 3, i32 2, i32 5, i32 4, i32 7, i32 6, i32 8, i32 9, i32 10, i32 11, i32 12, i32 13, i32 14, i32 15, i32 16, i32 17, i32 18, i32 19, i32 20, i32 21, i32 22, i32 23, i32 24, i32 25, i32 26, i32 27, i32 28, i32 29, i32 30, i32 31, i32 63, i32 62, i32 61, i32 60, i32 59, i32 58, i32 57, i32 56, i32 55, i32 54, i32 53, i32 52, i32 51, i32 50, i32 49, i32 48, i32 47, i32 46, i32 45, i32 44, i32 43, i32 42, i32 41, i32 40, i32 39, i32 38, i32 37, i32 36, i32 35, i32 34, i32 33, i32 32>
  %3 = call <32 x i16> @llvm.x86.avx512.pmaddubs.w.512(<64 x i8> %1, <64 x i8> %2)
  %4 = shufflevector <32 x i16> %3, <32 x i16> poison, <32 x i32> zeroinitializer
  ret <32 x i16> %4
}
