// REQUIRES: powerpc-registered-target

// RUN: %clang -S -emit-llvm -target powerpc64-gnu-linux -mcpu=pwr8 -ffreestanding -DNO_WARN_X86_INTRINSICS %s \
// RUN:   -fno-discard-value-names -mllvm -disable-llvm-optzns -o - | llvm-cxxfilt -n | FileCheck %s --check-prefixes=CHECK,CHECK-BE
// RUN: %clang -S -emit-llvm -target powerpc64le-gnu-linux -mcpu=pwr8 -ffreestanding -DNO_WARN_X86_INTRINSICS %s \
// RUN:   -fno-discard-value-names -mllvm -disable-llvm-optzns -o - | llvm-cxxfilt -n | FileCheck %s --check-prefixes=CHECK,CHECK-LE

// RUN: %clang -S -emit-llvm -target powerpc64-unknown-freebsd13.0 -mcpu=pwr8 -ffreestanding -DNO_WARN_X86_INTRINSICS %s \
// RUN:   -fno-discard-value-names -mllvm -disable-llvm-optzns -o - | llvm-cxxfilt -n | FileCheck %s --check-prefixes=CHECK,CHECK-BE
// RUN: %clang -S -emit-llvm -target powerpc64le-unknown-freebsd13.0 -mcpu=pwr8 -ffreestanding -DNO_WARN_X86_INTRINSICS %s \
// RUN:   -fno-discard-value-names -mllvm -disable-llvm-optzns -o - | llvm-cxxfilt -n | FileCheck %s --check-prefixes=CHECK,CHECK-LE

// RUN: %clang -S -emit-llvm -target powerpc64-ibm-aix -mcpu=pwr8 -ffreestanding -DNO_WARN_X86_INTRINSICS %s \
// RUN:   -fno-discard-value-names -mllvm -disable-llvm-optzns -o - | llvm-cxxfilt -n | FileCheck %s --check-prefixes=CHECK,CHECK-BE

// RUN: %clang -x c++ -S -emit-llvm -target powerpc64le-gnu-linux -mcpu=pwr8 -ffreestanding -DNO_WARN_X86_INTRINSICS %s \
// RUN:   -fno-discard-value-names -mllvm -disable-llvm-optzns -fsyntax-only

#include <tmmintrin.h>

__m64 res, m1, m2;
__m128i resi, mi1, mi2;

void __attribute__((noinline))
test_abs() {
  resi = _mm_abs_epi16(mi1);
  resi = _mm_abs_epi32(mi1);
  resi = _mm_abs_epi8(mi1);
  res = _mm_abs_pi16(m1);
  res = _mm_abs_pi32(m1);
  res = _mm_abs_pi8(m1);
}

// CHECK-LABEL: @test_abs

// CHECK-LABEL: define available_externally <2 x i64> @_mm_abs_epi16
// CHECK: call <8 x i16> @vec_abs(short vector[8])

// CHECK-LABEL: define available_externally <2 x i64> @_mm_abs_epi32
// CHECK: call <4 x i32> @vec_abs(int vector[4])

// CHECK-LABEL: define available_externally <2 x i64> @_mm_abs_epi8
// CHECK: call <16 x b8> @vec_abs(signed char vector[16])

// CHECK-LABEL: define available_externally i64 @_mm_abs_pi16
// CHECK: %[[ABS:[0-9a-zA-Z_.]+]] = call <8 x i16> @vec_abs(short vector[8])
// CHECK: %[[CAST:[0-9a-zA-Z_.]+]] = bitcast <8 x i16> %[[ABS]] to <2 x i64>
// CHECK: extractelement <2 x i64> %[[CAST]], i32 0

// CHECK-LABEL: define available_externally i64 @_mm_abs_pi32
// CHECK: %[[ABS:[0-9a-zA-Z_.]+]] = call <4 x i32> @vec_abs(int vector[4])
// CHECK: %[[CAST:[0-9a-zA-Z_.]+]] = bitcast <4 x i32> %[[ABS]] to <2 x i64>
// CHECK: extractelement <2 x i64> %[[CAST]], i32 0

// CHECK-LABEL: define available_externally i64 @_mm_abs_pi8
// CHECK: %[[ABS:[0-9a-zA-Z_.]+]] = call <16 x b8> @vec_abs(signed char vector[16])
// CHECK: %[[CAST1:[0-9a-zA-Z_.]+]] = bitcast <16 x b8> %[[ABS]] to <2 x b64>
// CHECK: %[[CAST2:[0-9a-zA-Z_.]+]] = bytecast <2 x b64> %[[CAST1]] to <2 x i64>
// CHECK: extractelement <2 x i64> %[[CAST2]], i32 0

void __attribute__((noinline))
test_alignr() {
  resi = _mm_alignr_epi8(mi1, mi2, 1U);
  res = _mm_alignr_pi8(m1, m2, 1U);
}

// CHECK-LABEL: @test_alignr

// CHECK-LABEL: define available_externally <2 x i64> @_mm_alignr_epi8
// CHECK: %[[CONST:[0-9a-zA-Z_.]+]] = call i1 @llvm.is.constant.i32(i32 %0)
// CHECK: br i1 %[[CONST]]
// CHECK-BE: call <16 x b8> @vec_sld(unsigned char vector[16], unsigned char vector[16], unsigned int)
// CHECK-LE: call <16 x b8> @vec_reve(unsigned char vector[16])
// CHECK-LE: call <16 x b8> @vec_reve(unsigned char vector[16])
// CHECk-LE: call <16 x b8> @vec_sld(unsigned char vector[16], unsigned char vector[16], unsigned int)
// CHECK-LE: call <16 x b8> @vec_reve(unsigned char vector[16])
// CHECK: store <16 x b8> zeroinitializer, ptr %{{[0-9a-zA-Z_.]+}}, align 16
// CHECK: store <2 x i64> zeroinitializer, ptr %{{[0-9a-zA-Z_.]+}}, align 16
// CHECK: %[[SUB:[0-9a-zA-Z_.]+]] = sub i32 %{{[0-9a-zA-Z_.]+}}, 16
// CHECK: %[[MUL:[0-9a-zA-Z_.]+]] = mul i32 %[[SUB]], 8
// CHECK: %[[TRUNC:[0-9a-zA-Z_.]+]] = trunc i32 %[[MUL]] to i8
// CHECK: %[[CONV:[0-9a-zA-Z_.]+]] = bitcast i8 %[[TRUNC]] to b8
// CHECK: call <16 x b8> @vec_splats(unsigned char)(b8 noundef zeroext %[[CONV]])
// CHECK-BE: call <16 x b8> @vec_slo(unsigned char vector[16], unsigned char vector[16])
// CHECK-LE: call <16 x b8> @vec_sro(unsigned char vector[16], unsigned char vector[16])
// CHECK: %[[SUB2:[0-9a-zA-Z_.]+]] = sub i32 16, %{{[0-9a-zA-Z_.]+}}
// CHECK: %[[MUL2:[0-9a-zA-Z_.]+]] = mul i32 %[[SUB2]], 8
// CHECK-BE: %[[TRUNC:[0-9a-zA-Z_.]+]] = trunc i32 %[[MUL2]] to i8
// CHECK-BE: %[[CONV:[0-9a-zA-Z_.]+]] = bitcast i8 %[[TRUNC]] to b8
// CHECK-BE: call <16 x b8> @vec_splats(unsigned char)(b8 noundef zeroext %[[CONV]])
// CHECK-BE: mul i32 %{{[0-9a-zA-Z_.]+}}, 8
// CHECK-BE: call <16 x b8> @vec_sro(unsigned char vector[16], unsigned char vector[16])
// CHECK-BE: call <16 x b8> @vec_slo(unsigned char vector[16], unsigned char vector[16])
// CHECK-BE: call <16 x b8> @vec_or(unsigned char vector[16], unsigned char vector[16])
// CHECK-LE: %[[MUL3:[0-9a-zA-Z_.]+]] = mul i32 %{{[0-9a-zA-Z_.]+}}, 8
// CHECK-LE: trunc i32 %[[MUL3]] to i8
// CHECK-LE: call <16 x b8> @vec_splats(unsigned char)
// CHECK-LE: call <16 x b8> @vec_slo(unsigned char vector[16], unsigned char vector[16])
// CHECK-LE: call <16 x b8> @vec_sro(unsigned char vector[16], unsigned char vector[16])
// CHECK-LE: call <16 x b8> @vec_or(unsigned char vector[16], unsigned char vector[16])

// CHECK-LABEL: define available_externally i64 @_mm_alignr_pi8
// CHECK: %[[CMP:[0-9a-zA-Z_.]+]] = icmp ult i32 %{{[0-9a-zA-Z_.]+}}, 16
// CHECK: br i1 %[[CMP]]
// CHECK-BE: call <16 x b8> @vec_slo(unsigned char vector[16], unsigned char vector[16])
// CHECK-LE: call <16 x b8> @vec_sro(unsigned char vector[16], unsigned char vector[16])
// CHECK: extractelement <2 x i64> %{{[0-9a-zA-Z_.]+}}, i32 0
// CHECK: store i64 0, ptr %{{[0-9a-zA-Z_.]+}}, align 8
// CHECK: store i64 0, ptr %{{[0-9a-zA-Z_.]+}}, align 8

void __attribute__((noinline))
test_hadd() {
  resi = _mm_hadd_epi16(mi1, mi2);
  resi = _mm_hadd_epi32(mi1, mi2);
  res = _mm_hadd_pi16(m1, m2);
  res = _mm_hadd_pi32(m1, m2);
  resi = _mm_hadds_epi16(mi1, mi2);
  res = _mm_hadds_pi16(m1, m2);
}

// CHECK-LABEL: @test_hadd

// CHECK-LABEL: define available_externally <2 x i64> @_mm_hadd_epi16
// CHECK: store <16 x b8> <b8 0, b8 1, b8 4, b8 5, b8 8, b8 9, b8 12, b8 13, b8 16, b8 17, b8 20, b8 21, b8 24, b8 25, b8 28, b8 29>, ptr %{{[0-9a-zA-Z_.]+}}, align 16
// CHECK: store <16 x b8> <b8 2, b8 3, b8 6, b8 7, b8 10, b8 11, b8 14, b8 15, b8 18, b8 19, b8 22, b8 23, b8 26, b8 27, b8 30, b8 31>, ptr %{{[0-9a-zA-Z_.]+}}, align 16
// CHECK: call <8 x i16> @vec_perm(short vector[8], short vector[8], unsigned char vector[16])(<8 x i16> noundef %{{[0-9a-zA-Z_.]+}}, <8 x i16> noundef %{{[0-9a-zA-Z_.]+}}, <16 x b8> noundef <b8 0, b8 1, b8 4, b8 5, b8 8, b8 9, b8 12, b8 13, b8 16, b8 17, b8 20, b8 21, b8 24, b8 25, b8 28, b8 29>)
// CHECK: call <8 x i16> @vec_perm(short vector[8], short vector[8], unsigned char vector[16])(<8 x i16> noundef %{{[0-9a-zA-Z_.]+}}, <8 x i16> noundef %{{[0-9a-zA-Z_.]+}}, <16 x b8> noundef <b8 2, b8 3, b8 6, b8 7, b8 10, b8 11, b8 14, b8 15, b8 18, b8 19, b8 22, b8 23, b8 26, b8 27, b8 30, b8 31>)
// CHECK: call <8 x i16> @vec_add(short vector[8], short vector[8])

// CHECK-LABEL: define available_externally <2 x i64> @_mm_hadd_epi32
// CHECK: store <16 x b8> <b8 0, b8 1, b8 2, b8 3, b8 8, b8 9, b8 10, b8 11, b8 16, b8 17, b8 18, b8 19, b8 24, b8 25, b8 26, b8 27>, ptr %{{[0-9a-zA-Z_.]+}}, align 16
// CHECK: store <16 x b8> <b8 4, b8 5, b8 6, b8 7, b8 12, b8 13, b8 14, b8 15, b8 20, b8 21, b8 22, b8 23, b8 28, b8 29, b8 30, b8 31>, ptr %{{[0-9a-zA-Z_.]+}}, align 16
// CHECK: call <4 x i32> @vec_perm(int vector[4], int vector[4], unsigned char vector[16])(<4 x i32> noundef %{{[0-9a-zA-Z_.]+}}, <4 x i32> noundef %{{[0-9a-zA-Z_.]+}}, <16 x b8> noundef <b8 0, b8 1, b8 2, b8 3, b8 8, b8 9, b8 10, b8 11, b8 16, b8 17, b8 18, b8 19, b8 24, b8 25, b8 26, b8 27>)
// CHECK: call <4 x i32> @vec_perm(int vector[4], int vector[4], unsigned char vector[16])(<4 x i32> noundef %{{[0-9a-zA-Z_.]+}}, <4 x i32> noundef %{{[0-9a-zA-Z_.]+}}, <16 x b8> noundef <b8 4, b8 5, b8 6, b8 7, b8 12, b8 13, b8 14, b8 15, b8 20, b8 21, b8 22, b8 23, b8 28, b8 29, b8 30, b8 31>)
// CHECK: call <4 x i32> @vec_add(int vector[4], int vector[4])

// CHECK-LABEL: define available_externally i64 @_mm_hadd_pi16
// CHECK: store <16 x b8> <b8 0, b8 1, b8 4, b8 5, b8 8, b8 9, b8 12, b8 13, b8 0, b8 1, b8 4, b8 5, b8 8, b8 9, b8 12, b8 13>, ptr %{{[0-9a-zA-Z_.]+}}, align 16
// CHECK: store <16 x b8> <b8 2, b8 3, b8 6, b8 7, b8 10, b8 11, b8 14, b8 15, b8 2, b8 3, b8 6, b8 7, b8 10, b8 11, b8 14, b8 15>, ptr %{{[0-9a-zA-Z_.]+}}, align 16
// CHECK: call <8 x i16> @vec_perm(short vector[8], short vector[8], unsigned char vector[16])(<8 x i16> noundef %{{[0-9a-zA-Z_.]+}}, <8 x i16> noundef %{{[0-9a-zA-Z_.]+}}, <16 x b8> noundef <b8 2, b8 3, b8 6, b8 7, b8 10, b8 11, b8 14, b8 15, b8 2, b8 3, b8 6, b8 7, b8 10, b8 11, b8 14, b8 15>)
// CHECK: call <8 x i16> @vec_perm(short vector[8], short vector[8], unsigned char vector[16])(<8 x i16> noundef %{{[0-9a-zA-Z_.]+}}, <8 x i16> noundef %{{[0-9a-zA-Z_.]+}}, <16 x b8> noundef <b8 0, b8 1, b8 4, b8 5, b8 8, b8 9, b8 12, b8 13, b8 0, b8 1, b8 4, b8 5, b8 8, b8 9, b8 12, b8 13>)
// CHECK: call <8 x i16> @vec_add(short vector[8], short vector[8])
// CHECK: %[[CAST:[0-9a-zA-Z_.]+]] = bitcast <8 x i16> %{{[0-9a-zA-Z_.]+}} to <2 x i64>
// CHECK: extractelement <2 x i64> %[[CAST]], i32 1

// CHECK-LABEL: define available_externally i64 @_mm_hadd_pi32
// CHECK: store <16 x b8> <b8 0, b8 1, b8 2, b8 3, b8 8, b8 9, b8 10, b8 11, b8 0, b8 1, b8 2, b8 3, b8 8, b8 9, b8 10, b8 11>, ptr %{{[0-9a-zA-Z_.]+}}, align 16
// CHECK: store <16 x b8> <b8 4, b8 5, b8 6, b8 7, b8 12, b8 13, b8 14, b8 15, b8 4, b8 5, b8 6, b8 7, b8 12, b8 13, b8 14, b8 15>, ptr %{{[0-9a-zA-Z_.]+}}, align 16
// CHECK: call <4 x i32> @vec_perm(int vector[4], int vector[4], unsigned char vector[16])(<4 x i32> noundef %{{[0-9a-zA-Z_.]+}}, <4 x i32> noundef %{{[0-9a-zA-Z_.]+}}, <16 x b8> noundef <b8 4, b8 5, b8 6, b8 7, b8 12, b8 13, b8 14, b8 15, b8 4, b8 5, b8 6, b8 7, b8 12, b8 13, b8 14, b8 15>)
// CHECK: call <4 x i32> @vec_perm(int vector[4], int vector[4], unsigned char vector[16])(<4 x i32> noundef %{{[0-9a-zA-Z_.]+}}, <4 x i32> noundef %{{[0-9a-zA-Z_.]+}}, <16 x b8> noundef <b8 0, b8 1, b8 2, b8 3, b8 8, b8 9, b8 10, b8 11, b8 0, b8 1, b8 2, b8 3, b8 8, b8 9, b8 10, b8 11>)
// CHECK: call <4 x i32> @vec_add(int vector[4], int vector[4])
// CHECK: %[[CAST:[0-9a-zA-Z_.]+]] = bitcast <4 x i32> %{{[0-9a-zA-Z_.]+}} to <2 x i64>
// CHECK: extractelement <2 x i64> %[[CAST]], i32 1

// CHECK-LABEL: define available_externally <2 x i64> @_mm_hadds_epi16
// CHECK: store <4 x i32> zeroinitializer, ptr %{{[0-9a-zA-Z_.]+}}, align 16
// CHECK: store <4 x i32> zeroinitializer, ptr %{{[0-9a-zA-Z_.]+}}, align 16
// CHECK: call <4 x i32> @vec_sum4s(short vector[8], int vector[4])
// CHECK: call <4 x i32> @vec_sum4s(short vector[8], int vector[4])
// CHECK: call <8 x i16> @vec_packs(int vector[4], int vector[4])

// CHECK-LABEL: define available_externally i64 @_mm_hadds_pi16
// CHECK: call <4 x i32> @vec_sum4s(short vector[8], int vector[4])(<8 x i16> noundef %{{[0-9a-zA-Z_.]+}}, <4 x i32> noundef zeroinitializer)
// CHECK: call <8 x i16> @vec_packs(int vector[4], int vector[4])
// CHECK: %[[CAST:[0-9a-zA-Z_.]+]] = bitcast <8 x i16> %{{[0-9a-zA-Z_.]+}} to <2 x i64>
// CHECK: extractelement <2 x i64> %[[CAST]], i32 1

void __attribute__((noinline))
test_hsub() {
  resi = _mm_hsub_epi16(mi1, mi2);
  resi = _mm_hsub_epi32(mi1, mi2);
  res = _mm_hsub_pi16(m1, m2);
  res = _mm_hsub_pi32(m1, m2);
  resi = _mm_hsubs_epi16(mi1, mi2);
  res = _mm_hsubs_pi16(m1, m2);
}

// CHECK-LABEL: @test_hsub

// CHECK-LABEL: define available_externally <2 x i64> @_mm_hsub_epi16
// CHECK: store <16 x b8> <b8 0, b8 1, b8 4, b8 5, b8 8, b8 9, b8 12, b8 13, b8 16, b8 17, b8 20, b8 21, b8 24, b8 25, b8 28, b8 29>, ptr %{{[0-9a-zA-Z_.]+}}, align 16
// CHECK: store <16 x b8> <b8 2, b8 3, b8 6, b8 7, b8 10, b8 11, b8 14, b8 15, b8 18, b8 19, b8 22, b8 23, b8 26, b8 27, b8 30, b8 31>, ptr %{{[0-9a-zA-Z_.]+}}, align 16
// CHECK: call <8 x i16> @vec_perm(short vector[8], short vector[8], unsigned char vector[16])(<8 x i16> noundef %{{[0-9a-zA-Z_.]+}}, <8 x i16> noundef %{{[0-9a-zA-Z_.]+}}, <16 x b8> noundef <b8 0, b8 1, b8 4, b8 5, b8 8, b8 9, b8 12, b8 13, b8 16, b8 17, b8 20, b8 21, b8 24, b8 25, b8 28, b8 29>)
// CHECK: call <8 x i16> @vec_perm(short vector[8], short vector[8], unsigned char vector[16])(<8 x i16> noundef %{{[0-9a-zA-Z_.]+}}, <8 x i16> noundef %{{[0-9a-zA-Z_.]+}}, <16 x b8> noundef <b8 2, b8 3, b8 6, b8 7, b8 10, b8 11, b8 14, b8 15, b8 18, b8 19, b8 22, b8 23, b8 26, b8 27, b8 30, b8 31>)
// CHECK: call <8 x i16> @vec_sub(short vector[8], short vector[8])

// CHECK-LABEL: define available_externally <2 x i64> @_mm_hsub_epi32
// CHECK: store <16 x b8> <b8 0, b8 1, b8 2, b8 3, b8 8, b8 9, b8 10, b8 11, b8 16, b8 17, b8 18, b8 19, b8 24, b8 25, b8 26, b8 27>, ptr %{{[0-9a-zA-Z_.]+}}, align 16
// CHECK: store <16 x b8> <b8 4, b8 5, b8 6, b8 7, b8 12, b8 13, b8 14, b8 15, b8 20, b8 21, b8 22, b8 23, b8 28, b8 29, b8 30, b8 31>, ptr %{{[0-9a-zA-Z_.]+}}, align 16
// CHECK: call <4 x i32> @vec_perm(int vector[4], int vector[4], unsigned char vector[16])(<4 x i32> noundef %{{[0-9a-zA-Z_.]+}}, <4 x i32> noundef %{{[0-9a-zA-Z_.]+}}, <16 x b8> noundef <b8 0, b8 1, b8 2, b8 3, b8 8, b8 9, b8 10, b8 11, b8 16, b8 17, b8 18, b8 19, b8 24, b8 25, b8 26, b8 27>)
// CHECK: call <4 x i32> @vec_perm(int vector[4], int vector[4], unsigned char vector[16])(<4 x i32> noundef %{{[0-9a-zA-Z_.]+}}, <4 x i32> noundef %{{[0-9a-zA-Z_.]+}}, <16 x b8> noundef <b8 4, b8 5, b8 6, b8 7, b8 12, b8 13, b8 14, b8 15, b8 20, b8 21, b8 22, b8 23, b8 28, b8 29, b8 30, b8 31>)
// CHECK: call <4 x i32> @vec_sub(int vector[4], int vector[4])

// CHECK-LABEL: define available_externally i64 @_mm_hsub_pi16
// CHECK: store <16 x b8> <b8 0, b8 1, b8 4, b8 5, b8 8, b8 9, b8 12, b8 13, b8 0, b8 1, b8 4, b8 5, b8 8, b8 9, b8 12, b8 13>, ptr %{{[0-9a-zA-Z_.]+}}, align 16
// CHECK: store <16 x b8> <b8 2, b8 3, b8 6, b8 7, b8 10, b8 11, b8 14, b8 15, b8 2, b8 3, b8 6, b8 7, b8 10, b8 11, b8 14, b8 15>, ptr %{{[0-9a-zA-Z_.]+}}, align 16
// CHECK: call <8 x i16> @vec_perm(short vector[8], short vector[8], unsigned char vector[16])(<8 x i16> noundef %{{[0-9a-zA-Z_.]+}}, <8 x i16> noundef %{{[0-9a-zA-Z_.]+}}, <16 x b8> noundef <b8 2, b8 3, b8 6, b8 7, b8 10, b8 11, b8 14, b8 15, b8 2, b8 3, b8 6, b8 7, b8 10, b8 11, b8 14, b8 15>)
// CHECK: call <8 x i16> @vec_perm(short vector[8], short vector[8], unsigned char vector[16])(<8 x i16> noundef %{{[0-9a-zA-Z_.]+}}, <8 x i16> noundef %{{[0-9a-zA-Z_.]+}}, <16 x b8> noundef <b8 0, b8 1, b8 4, b8 5, b8 8, b8 9, b8 12, b8 13, b8 0, b8 1, b8 4, b8 5, b8 8, b8 9, b8 12, b8 13>)
// CHECK: call <8 x i16> @vec_sub(short vector[8], short vector[8])
// CHECK: %[[CAST:[0-9a-zA-Z_.]+]] = bitcast <8 x i16> %{{[0-9a-zA-Z_.]+}} to <2 x i64>
// CHECK: extractelement <2 x i64> %[[CAST]], i32 1

// CHECK-LABEL: define available_externally i64 @_mm_hsub_pi32
// CHECK: store <16 x b8> <b8 0, b8 1, b8 2, b8 3, b8 8, b8 9, b8 10, b8 11, b8 0, b8 1, b8 2, b8 3, b8 8, b8 9, b8 10, b8 11>, ptr %{{[0-9a-zA-Z_.]+}}, align 16
// CHECK: store <16 x b8> <b8 4, b8 5, b8 6, b8 7, b8 12, b8 13, b8 14, b8 15, b8 4, b8 5, b8 6, b8 7, b8 12, b8 13, b8 14, b8 15>, ptr %{{[0-9a-zA-Z_.]+}}, align 16
// CHECK: call <4 x i32> @vec_perm(int vector[4], int vector[4], unsigned char vector[16])(<4 x i32> noundef %{{[0-9a-zA-Z_.]+}}, <4 x i32> noundef %{{[0-9a-zA-Z_.]+}}, <16 x b8> noundef <b8 4, b8 5, b8 6, b8 7, b8 12, b8 13, b8 14, b8 15, b8 4, b8 5, b8 6, b8 7, b8 12, b8 13, b8 14, b8 15>)
// CHECK: call <4 x i32> @vec_perm(int vector[4], int vector[4], unsigned char vector[16])(<4 x i32> noundef %{{[0-9a-zA-Z_.]+}}, <4 x i32> noundef %{{[0-9a-zA-Z_.]+}}, <16 x b8> noundef <b8 0, b8 1, b8 2, b8 3, b8 8, b8 9, b8 10, b8 11, b8 0, b8 1, b8 2, b8 3, b8 8, b8 9, b8 10, b8 11>)
// CHECK: call <4 x i32> @vec_sub(int vector[4], int vector[4])
// CHECK: %[[CAST:[0-9a-zA-Z_.]+]] = bitcast <4 x i32> %{{[0-9a-zA-Z_.]+}} to <2 x i64>
// CHECK: extractelement <2 x i64> %[[CAST]], i32 1

// CHECK-LABEL: define available_externally <2 x i64> @_mm_hsubs_epi16
// CHECK: store <16 x b8> <b8 0, b8 1, b8 4, b8 5, b8 8, b8 9, b8 12, b8 13, b8 16, b8 17, b8 20, b8 21, b8 24, b8 25, b8 28, b8 29>, ptr %{{[0-9a-zA-Z_.]+}}, align 16
// CHECK: store <16 x b8> <b8 2, b8 3, b8 6, b8 7, b8 10, b8 11, b8 14, b8 15, b8 18, b8 19, b8 22, b8 23, b8 26, b8 27, b8 30, b8 31>, ptr %{{[0-9a-zA-Z_.]+}}, align 16
// CHECK: call <8 x i16> @vec_perm(short vector[8], short vector[8], unsigned char vector[16])(<8 x i16> noundef %{{[0-9a-zA-Z_.]+}}, <8 x i16> noundef %{{[0-9a-zA-Z_.]+}}, <16 x b8> noundef <b8 0, b8 1, b8 4, b8 5, b8 8, b8 9, b8 12, b8 13, b8 16, b8 17, b8 20, b8 21, b8 24, b8 25, b8 28, b8 29>)
// CHECK: call <8 x i16> @vec_perm(short vector[8], short vector[8], unsigned char vector[16])(<8 x i16> noundef %{{[0-9a-zA-Z_.]+}}, <8 x i16> noundef %{{[0-9a-zA-Z_.]+}}, <16 x b8> noundef <b8 2, b8 3, b8 6, b8 7, b8 10, b8 11, b8 14, b8 15, b8 18, b8 19, b8 22, b8 23, b8 26, b8 27, b8 30, b8 31>)
// CHECK: call <8 x i16> @vec_subs(short vector[8], short vector[8])

// CHECK-LABEL: define available_externally i64 @_mm_hsubs_pi16
// CHECK: store <16 x b8> <b8 0, b8 1, b8 4, b8 5, b8 8, b8 9, b8 12, b8 13, b8 0, b8 1, b8 4, b8 5, b8 8, b8 9, b8 12, b8 13>, ptr %{{[0-9a-zA-Z_.]+}}, align 16
// CHECK: store <16 x b8> <b8 2, b8 3, b8 6, b8 7, b8 10, b8 11, b8 14, b8 15, b8 2, b8 3, b8 6, b8 7, b8 10, b8 11, b8 14, b8 15>, ptr %{{[0-9a-zA-Z_.]+}}, align 16
// CHECK: call <8 x i16> @vec_perm(short vector[8], short vector[8], unsigned char vector[16])(<8 x i16> noundef %{{[0-9a-zA-Z_.]+}}, <8 x i16> noundef %{{[0-9a-zA-Z_.]+}}, <16 x b8> noundef <b8 0, b8 1, b8 4, b8 5, b8 8, b8 9, b8 12, b8 13, b8 0, b8 1, b8 4, b8 5, b8 8, b8 9, b8 12, b8 13>)
// CHECK: call <8 x i16> @vec_perm(short vector[8], short vector[8], unsigned char vector[16])(<8 x i16> noundef %{{[0-9a-zA-Z_.]+}}, <8 x i16> noundef %{{[0-9a-zA-Z_.]+}}, <16 x b8> noundef <b8 2, b8 3, b8 6, b8 7, b8 10, b8 11, b8 14, b8 15, b8 2, b8 3, b8 6, b8 7, b8 10, b8 11, b8 14, b8 15>)
// CHECK: call <8 x i16> @vec_subs(short vector[8], short vector[8])
// CHECK: %[[CAST:[0-9a-zA-Z_.]+]] = bitcast <8 x i16> %{{[0-9a-zA-Z_.]+}} to <2 x i64>
// CHECK: extractelement <2 x i64> %[[CAST]], i32 1

void __attribute__((noinline))
test_shuffle() {
  resi = _mm_shuffle_epi8(mi1, mi2);
  res = _mm_shuffle_pi8(m1, m2);
}

// CHECK-LABEL: @test_shuffle

// CHECK-LABEL: define available_externally <2 x i64> @_mm_shuffle_epi8
// CHECK: store <16 x b8> zeroinitializer, ptr %{{[0-9a-zA-Z_.]+}}, align 16
// CHECK: call <16 x b8> @vec_cmplt(signed char vector[16], signed char vector[16])(<16 x b8> noundef %{{[0-9a-zA-Z_.]+}}, <16 x b8> noundef zeroinitializer)
// CHECK: call <16 x b8> @vec_perm(signed char vector[16], signed char vector[16], unsigned char vector[16])
// CHECK: call <16 x b8> @vec_sel(signed char vector[16], signed char vector[16], bool vector[16])(<16 x b8> noundef %{{[0-9a-zA-Z_.]+}}, <16 x b8> noundef zeroinitializer, <16 x b8> noundef %{{[0-9a-zA-Z_.]+}})

// CHECK-LABEL: define available_externally i64 @_mm_shuffle_pi8
// CHECK: call <16 x b8> @vec_cmplt(signed char vector[16], signed char vector[16])(<16 x b8> noundef %{{[0-9a-zA-Z_.]+}}, <16 x b8> noundef zeroinitializer)
// CHECK: call <16 x b8> @vec_perm(signed char vector[16], signed char vector[16], unsigned char vector[16])
// CHECK: call <16 x b8> @vec_sel(signed char vector[16], signed char vector[16], bool vector[16])(<16 x b8> noundef %{{[0-9a-zA-Z_.]+}}, <16 x b8> noundef zeroinitializer, <16 x b8> noundef %{{[0-9a-zA-Z_.]+}})
// CHECK: %[[CAST1:[0-9a-zA-Z_.]+]] = bitcast <16 x b8> %{{[0-9a-zA-Z_.]+}} to <2 x b64>
// CHECK: %[[CAST2:[0-9a-zA-Z_.]+]] = bytecast <2 x b64> %[[CAST1]] to <2 x i64>
// CHECK: extractelement <2 x i64> %[[CAST2]], i32 0

void __attribute__((noinline))
test_sign() {
  resi = _mm_sign_epi8(mi1, mi2);
  resi = _mm_sign_epi16(mi1, mi2);
  resi = _mm_sign_epi32(mi1, mi2);
  res = _mm_sign_pi8(m1, m2);
  res = _mm_sign_pi16(m1, m2);
  res = _mm_sign_pi32(m1, m2);
}

// CHECK-LABEL: @test_sign

// CHECK-LABEL: define available_externally <2 x i64> @_mm_sign_epi8
// CHECK: call <16 x b8> @vec_cmplt(signed char vector[16], signed char vector[16])(<16 x b8> noundef %{{[0-9a-zA-Z_.]+}}, <16 x b8> noundef zeroinitializer)
// CHECK: call <16 x b8> @vec_cmpgt(signed char vector[16], signed char vector[16])(<16 x b8> noundef %{{[0-9a-zA-Z_.]+}}, <16 x b8> noundef zeroinitializer)
// CHECK: call <16 x b8> @vec_neg(signed char vector[16])
// CHECK: call <16 x b8> @vec_add(signed char vector[16], signed char vector[16])
// CHECK: call <16 x b8> @vec_mul(signed char vector[16], signed char vector[16])

// CHECK-LABEL: define available_externally <2 x i64> @_mm_sign_epi16
// CHECK: call <8 x i16> @vec_cmplt(short vector[8], short vector[8])(<8 x i16> noundef %{{[0-9a-zA-Z_.]+}}, <8 x i16> noundef zeroinitializer)
// CHECK: call <8 x i16> @vec_cmpgt(short vector[8], short vector[8])(<8 x i16> noundef %{{[0-9a-zA-Z_.]+}}, <8 x i16> noundef zeroinitializer)
// CHECK: call <8 x i16> @vec_neg(short vector[8])
// CHECK: call <8 x i16> @vec_add(short vector[8], short vector[8])
// CHECK: call <8 x i16> @vec_mul(short vector[8], short vector[8])

// CHECK-LABEL: define available_externally <2 x i64> @_mm_sign_epi32
// CHECK: store <4 x i32> zeroinitializer, ptr %{{[0-9a-zA-Z_.]+}}, align 16
// CHECK: call <4 x i32> @vec_cmplt(int vector[4], int vector[4])(<4 x i32> noundef %{{[0-9a-zA-Z_.]+}}, <4 x i32> noundef zeroinitializer)
// CHECK: call <4 x i32> @vec_cmpgt(int vector[4], int vector[4])(<4 x i32> noundef %{{[0-9a-zA-Z_.]+}}, <4 x i32> noundef zeroinitializer)
// CHECK: call <4 x i32> @vec_neg(int vector[4])
// CHECK: call <4 x i32> @vec_add(int vector[4], int vector[4])
// CHECK: call <4 x i32> @vec_mul(int vector[4], int vector[4])

// CHECK-LABEL: define available_externally i64 @_mm_sign_pi8
// CHECK: store <16 x b8> zeroinitializer, ptr %{{[0-9a-zA-Z_.]+}}, align 16
// CHECK: call <2 x i64> @_mm_sign_epi8
// CHECK: %[[CAST1:[0-9a-zA-Z_.]+]] = bitcast <16 x b8> %{{[0-9a-zA-Z_.]+}} to <2 x b64>
// CHECK: %[[CAST2:[0-9a-zA-Z_.]+]] = bytecast <2 x b64> %[[CAST1]] to <2 x i64>
// CHECK: extractelement <2 x i64> %[[CAST2]], i32 0

// CHECK-LABEL: define available_externally i64 @_mm_sign_pi16
// CHECK: store <8 x i16> zeroinitializer, ptr %{{[0-9a-zA-Z_.]+}}, align 16
// CHECK: call <2 x i64> @_mm_sign_epi16
// CHECK: %[[CAST:[0-9a-zA-Z_.]+]] = bitcast <8 x i16> %{{[0-9a-zA-Z_.]+}} to <2 x i64>
// CHECK: extractelement <2 x i64> %[[CAST]], i32 0

// CHECK-LABEL: define available_externally i64 @_mm_sign_pi32
// CHECK: store <4 x i32> zeroinitializer, ptr %{{[0-9a-zA-Z_.]+}}, align 16
// CHECK: call <2 x i64> @_mm_sign_epi32
// CHECK: %[[CAST:[0-9a-zA-Z_.]+]] = bitcast <4 x i32> %{{[0-9a-zA-Z_.]+}} to <2 x i64>
// CHECK: extractelement <2 x i64> %[[CAST]], i32 0

void __attribute__((noinline))
test_maddubs() {
  resi = _mm_maddubs_epi16(mi1, mi2);
  res = _mm_maddubs_pi16(m1, m2);
}

// CHECK-LABEL: @test_maddubs

// CHECK-LABEL: define available_externally <2 x i64> @_mm_maddubs_epi16
// CHECK: call <8 x i16> @vec_splats(short)(i16 noundef signext 255)
// CHECK: call <8 x i16> @vec_unpackh(signed char vector[16])
// CHECK: call <8 x i16> @vec_and(short vector[8], short vector[8])
// CHECK: call <8 x i16> @vec_unpackl(signed char vector[16])
// CHECK: call <8 x i16> @vec_and(short vector[8], short vector[8])
// CHECK: call <8 x i16> @vec_unpackh(signed char vector[16])
// CHECK: call <8 x i16> @vec_unpackl(signed char vector[16])
// CHECK: call <8 x i16> @vec_mul(short vector[8], short vector[8])
// CHECK: call <8 x i16> @vec_mul(short vector[8], short vector[8])
// CHECK: store <16 x b8> <b8 0, b8 1, b8 4, b8 5, b8 8, b8 9, b8 12, b8 13, b8 16, b8 17, b8 20, b8 21, b8 24, b8 25, b8 28, b8 29>, ptr %{{[0-9a-zA-Z_.]+}}, align 16
// CHECK: store <16 x b8> <b8 2, b8 3, b8 6, b8 7, b8 10, b8 11, b8 14, b8 15, b8 18, b8 19, b8 22, b8 23, b8 26, b8 27, b8 30, b8 31>, ptr %{{[0-9a-zA-Z_.]+}}, align 16
// CHECK: call <8 x i16> @vec_perm(short vector[8], short vector[8], unsigned char vector[16])(<8 x i16> noundef %{{[0-9a-zA-Z_.]+}}, <8 x i16> noundef %{{[0-9a-zA-Z_.]+}}, <16 x b8> noundef <b8 0, b8 1, b8 4, b8 5, b8 8, b8 9, b8 12, b8 13, b8 16, b8 17, b8 20, b8 21, b8 24, b8 25, b8 28, b8 29>)
// CHECK: call <8 x i16> @vec_perm(short vector[8], short vector[8], unsigned char vector[16])(<8 x i16> noundef %{{[0-9a-zA-Z_.]+}}, <8 x i16> noundef %{{[0-9a-zA-Z_.]+}}, <16 x b8> noundef <b8 2, b8 3, b8 6, b8 7, b8 10, b8 11, b8 14, b8 15, b8 18, b8 19, b8 22, b8 23, b8 26, b8 27, b8 30, b8 31>)
// CHECK: call <8 x i16> @vec_adds(short vector[8], short vector[8])

// CHECK-LABEL: define available_externally i64 @_mm_maddubs_pi16
// CHECK: call <8 x i16> @vec_unpackl(signed char vector[16])
// CHECK: call <8 x i16> @vec_splats(short)(i16 noundef signext 255)
// CHECK: call <8 x i16> @vec_and(short vector[8], short vector[8])
// CHECK: call <8 x i16> @vec_unpackl(signed char vector[16])
// CHECK: call <8 x i16> @vec_mul(short vector[8], short vector[8])
// CHECK: store <16 x b8> <b8 0, b8 1, b8 4, b8 5, b8 8, b8 9, b8 12, b8 13, b8 16, b8 17, b8 20, b8 21, b8 24, b8 25, b8 28, b8 29>, ptr %{{[0-9a-zA-Z_.]+}}, align 16
// CHECK: store <16 x b8> <b8 2, b8 3, b8 6, b8 7, b8 10, b8 11, b8 14, b8 15, b8 18, b8 19, b8 22, b8 23, b8 26, b8 27, b8 30, b8 31>, ptr %{{[0-9a-zA-Z_.]+}}, align 16
// CHECK: call <8 x i16> @vec_perm(short vector[8], short vector[8], unsigned char vector[16])(<8 x i16> noundef %{{[0-9a-zA-Z_.]+}}, <8 x i16> noundef %{{[0-9a-zA-Z_.]+}}, <16 x b8> noundef <b8 0, b8 1, b8 4, b8 5, b8 8, b8 9, b8 12, b8 13, b8 16, b8 17, b8 20, b8 21, b8 24, b8 25, b8 28, b8 29>)
// CHECK: call <8 x i16> @vec_perm(short vector[8], short vector[8], unsigned char vector[16])(<8 x i16> noundef %{{[0-9a-zA-Z_.]+}}, <8 x i16> noundef %{{[0-9a-zA-Z_.]+}}, <16 x b8> noundef <b8 2, b8 3, b8 6, b8 7, b8 10, b8 11, b8 14, b8 15, b8 18, b8 19, b8 22, b8 23, b8 26, b8 27, b8 30, b8 31>)
// CHECK: call <8 x i16> @vec_adds(short vector[8], short vector[8])
// CHECK: %[[CAST:[0-9a-zA-Z_.]+]] = bitcast <8 x i16> %{{[0-9a-zA-Z_.]+}} to <2 x i64>
// CHECK: extractelement <2 x i64> %[[CAST]], i32 0

void __attribute__((noinline))
test_mulhrs() {
  resi = _mm_mulhrs_epi16(mi1, mi2);
  res = _mm_mulhrs_pi16(m1, m2);
}

// CHECK-LABEL: @test_mulhrs

// CHECK-LABEL: define available_externally <2 x i64> @_mm_mulhrs_epi16
// CHECK: call <4 x i32> @vec_unpackh(short vector[8])
// CHECK: call <4 x i32> @vec_unpackh(short vector[8])
// CHECK: call <4 x i32> @vec_mul(int vector[4], int vector[4])
// CHECK: call <4 x i32> @vec_unpackl(short vector[8])
// CHECK: call <4 x i32> @vec_unpackl(short vector[8])
// CHECK: call <4 x i32> @vec_mul(int vector[4], int vector[4])
// CHECK: call <4 x i32> @vec_splats(unsigned int)(i32 noundef zeroext 14)
// CHECK: call <4 x i32> @vec_sr(int vector[4], unsigned int vector[4])
// CHECK: call <4 x i32> @vec_sr(int vector[4], unsigned int vector[4])
// CHECK: call <4 x i32> @vec_splats(int)(i32 noundef signext 1)
// CHECK: call <4 x i32> @vec_add(int vector[4], int vector[4])
// CHECK: call <4 x i32> @vec_sr(int vector[4], unsigned int vector[4])
// CHECK: call <4 x i32> @vec_add(int vector[4], int vector[4])
// CHECK: call <4 x i32> @vec_sr(int vector[4], unsigned int vector[4])
// CHECK: %[[PACK:[0-9a-zA-Z_.]+]] = call <8 x i16> @vec_pack(int vector[4], int vector[4])

// CHECK-LABEL: define available_externally i64 @_mm_mulhrs_pi16
// CHECK: call <4 x i32> @vec_unpackh(short vector[8])
// CHECK: call <4 x i32> @vec_unpackh(short vector[8])
// CHECK: call <4 x i32> @vec_mul(int vector[4], int vector[4])
// CHECK: call <4 x i32> @vec_splats(unsigned int)(i32 noundef zeroext 14)
// CHECK: call <4 x i32> @vec_sr(int vector[4], unsigned int vector[4])
// CHECK: call <4 x i32> @vec_splats(int)(i32 noundef signext 1)
// CHECK: call <4 x i32> @vec_add(int vector[4], int vector[4])
// CHECK: call <4 x i32> @vec_sr(int vector[4], unsigned int vector[4])
// CHECK: call <8 x i16> @vec_pack(int vector[4], int vector[4])
// CHECK: %[[CAST:[0-9a-zA-Z_.]+]] = bitcast <8 x i16> %{{[0-9a-zA-Z_.]+}} to <2 x i64>
// CHECK: extractelement <2 x i64> %[[CAST]], i32 0
