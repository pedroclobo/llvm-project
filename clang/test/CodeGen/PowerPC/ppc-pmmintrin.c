// REQUIRES: powerpc-registered-target

// RUN: %clang -S -emit-llvm -target powerpc64-gnu-linux -mcpu=pwr8 -DNO_MM_MALLOC -ffreestanding -DNO_WARN_X86_INTRINSICS %s \
// RUN:   -fno-discard-value-names -mllvm -disable-llvm-optzns -o - | llvm-cxxfilt -n | FileCheck %s
// RUN: %clang -S -emit-llvm -target powerpc64le-gnu-linux -mcpu=pwr8 -DNO_MM_MALLOC -ffreestanding -DNO_WARN_X86_INTRINSICS %s \
// RUN:   -fno-discard-value-names -mllvm -disable-llvm-optzns -o - | llvm-cxxfilt -n | FileCheck %s

// RUN: %clang -S -emit-llvm -target powerpc64-unknown-freebsd13.0 -mcpu=pwr8 -DNO_MM_MALLOC -ffreestanding -DNO_WARN_X86_INTRINSICS %s \
// RUN:   -fno-discard-value-names -mllvm -disable-llvm-optzns -o - | llvm-cxxfilt -n | FileCheck %s
// RUN: %clang -S -emit-llvm -target powerpc64le-unknown-freebsd13.0 -mcpu=pwr8 -DNO_MM_MALLOC -ffreestanding -DNO_WARN_X86_INTRINSICS %s \
// RUN:   -fno-discard-value-names -mllvm -disable-llvm-optzns -o - | llvm-cxxfilt -n | FileCheck %s

// RUN: %clang -S -emit-llvm -target powerpc64-ibm-aix -mcpu=pwr8 -DNO_MM_MALLOC -ffreestanding -DNO_WARN_X86_INTRINSICS %s \
// RUN:   -fno-discard-value-names -mllvm -disable-llvm-optzns -o - | llvm-cxxfilt -n | FileCheck %s

// RUN: %clang -x c++ -S -emit-llvm -target powerpc64le-gnu-linux -mcpu=pwr8 -DNO_MM_MALLOC -ffreestanding -DNO_WARN_X86_INTRINSICS %s \
// RUN:   -fno-discard-value-names -mllvm -disable-llvm-optzns -fsyntax-only

#include <pmmintrin.h>

__m128d resd, md1, md2;
__m128 res, m1, m2;
__m128i resi, mi;
double *d;

void __attribute__((noinline))
test_pmmintrin() {
  resd = _mm_addsub_pd(md1, md2);
  res = _mm_addsub_ps(m1, m2);
  resd = _mm_hadd_pd(md1, md2);
  res = _mm_hadd_ps(m1, m2);
  resd = _mm_hsub_pd(md1, md2);
  res = _mm_hsub_ps(m1, m2);
  resi = _mm_lddqu_si128(&mi);
  resd = _mm_loaddup_pd(d);
  resd = _mm_movedup_pd(md1);
  res = _mm_movehdup_ps(m1);
  res = _mm_moveldup_ps(m1);
}

// CHECK-LABEL: define available_externally <2 x double> @_mm_addsub_pd(<2 x double> noundef %{{[0-9a-zA-Z_.]+}}, <2 x double> noundef %{{[0-9a-zA-Z_.]+}})
// CHECK: call <2 x double> @vec_xor(double vector[2], double vector[2])(<2 x double> noundef %{{[0-9a-zA-Z_.]+}}, <2 x double> noundef <double -0.000000e+00, double 0.000000e+00>)
// CHECK: call <2 x double> @vec_add(double vector[2], double vector[2])

// CHECK-LABEL: define available_externally <4 x float> @_mm_addsub_ps(<4 x float> noundef %{{[0-9a-zA-Z_.]+}}, <4 x float> noundef %{{[0-9a-zA-Z_.]+}})
// CHECK: call <4 x float> @vec_xor(float vector[4], float vector[4])(<4 x float> noundef %{{[0-9a-zA-Z_.]+}}, <4 x float> noundef <float -0.000000e+00, float 0.000000e+00, float -0.000000e+00, float 0.000000e+00>)
// CHECK: call <4 x float> @vec_add(float vector[4], float vector[4])

// CHECK-LABEL: define available_externally <2 x double> @_mm_hadd_pd(<2 x double> noundef %{{[0-9a-zA-Z_.]+}}, <2 x double> noundef %{{[0-9a-zA-Z_.]+}})
// CHECK: %[[CALL:[0-9a-zA-Z_.]+]] = call <2 x double> @vec_mergeh(double vector[2], double vector[2])
// CHECK: %[[CALL1:[0-9a-zA-Z_.]+]] = call <2 x double> @vec_mergel(double vector[2], double vector[2])
// CHECK: %[[CALL2:[0-9a-zA-Z_.]+]] = call <2 x double> @vec_add(double vector[2], double vector[2])(<2 x double> noundef %[[CALL]], <2 x double> noundef %[[CALL1]])

// CHECK-LABEL: define available_externally <4 x float> @_mm_hadd_ps(<4 x float> noundef %{{[0-9a-zA-Z_.]+}}, <4 x float> noundef %{{[0-9a-zA-Z_.]+}})
// CHECK: store <16 x b8> <b8 0, b8 1, b8 2, b8 3, b8 8, b8 9, b8 10, b8 11, b8 16, b8 17, b8 18, b8 19, b8 24, b8 25, b8 26, b8 27>, ptr %{{[0-9a-zA-Z_.]+}}, align 16
// CHECK: store <16 x b8> <b8 4, b8 5, b8 6, b8 7, b8 12, b8 13, b8 14, b8 15, b8 20, b8 21, b8 22, b8 23, b8 28, b8 29, b8 30, b8 31>, ptr %{{[0-9a-zA-Z_.]+}}, align 16
// CHECK: %[[CALL:[0-9a-zA-Z_.]+]] = call <4 x float> @vec_perm(float vector[4], float vector[4], unsigned char vector[16])
// CHECK: %[[CALL1:[0-9a-zA-Z_.]+]] = call <4 x float> @vec_perm(float vector[4], float vector[4], unsigned char vector[16])
// CHECK: %[[CALL2:[0-9a-zA-Z_.]+]] = call <4 x float> @vec_add(float vector[4], float vector[4])(<4 x float> noundef %[[CALL]], <4 x float> noundef %[[CALL1]])

// CHECK-LABEL: define available_externally <2 x double> @_mm_hsub_pd(<2 x double> noundef %{{[0-9a-zA-Z_.]+}}, <2 x double> noundef %{{[0-9a-zA-Z_.]+}})
// CHECK: %[[CALL:[0-9a-zA-Z_.]+]] = call <2 x double> @vec_mergeh(double vector[2], double vector[2])
// CHECK: %[[CALL1:[0-9a-zA-Z_.]+]] = call <2 x double> @vec_mergel(double vector[2], double vector[2])
// CHECK: %[[CALL2:[0-9a-zA-Z_.]+]] = call <2 x double> @vec_sub(double vector[2], double vector[2])(<2 x double> noundef %[[CALL]], <2 x double> noundef %[[CALL1]])

// CHECK-LABEL: define available_externally <4 x float> @_mm_hsub_ps(<4 x float> noundef %{{[0-9a-zA-Z_.]+}}, <4 x float> noundef %{{[0-9a-zA-Z_.]+}})
// CHECK: store <16 x b8> <b8 0, b8 1, b8 2, b8 3, b8 8, b8 9, b8 10, b8 11, b8 16, b8 17, b8 18, b8 19, b8 24, b8 25, b8 26, b8 27>, ptr %{{[0-9a-zA-Z_.]+}}, align 16
// CHECK: store <16 x b8> <b8 4, b8 5, b8 6, b8 7, b8 12, b8 13, b8 14, b8 15, b8 20, b8 21, b8 22, b8 23, b8 28, b8 29, b8 30, b8 31>, ptr %{{[0-9a-zA-Z_.]+}}, align 16
// CHECK: %[[CALL:[0-9a-zA-Z_.]+]] = call <4 x float> @vec_perm(float vector[4], float vector[4], unsigned char vector[16])
// CHECK: %[[CALL1:[0-9a-zA-Z_.]+]] = call <4 x float> @vec_perm(float vector[4], float vector[4], unsigned char vector[16])
// CHECK: %[[CALL2:[0-9a-zA-Z_.]+]] = call <4 x float> @vec_sub(float vector[4], float vector[4])(<4 x float> noundef %[[CALL]], <4 x float> noundef %[[CALL1]])

// CHECK-LABEL: define available_externally <2 x i64> @_mm_lddqu_si128(ptr noundef %{{[0-9a-zA-Z_.]+}})
// CHECK: call <4 x i32> @vec_vsx_ld(int, int const*)(i32 noundef signext 0, ptr noundef %{{[0-9a-zA-Z_.]+}})

// CHECK-LABEL: define available_externally <2 x double> @_mm_loaddup_pd(ptr noundef %{{[0-9a-zA-Z_.]+}})
// CHECK: call <2 x double> @vec_splats(double)

// CHECK-LABEL: define available_externally <2 x double> @_mm_movedup_pd(<2 x double> noundef %{{[0-9a-zA-Z_.]+}})
// CHECK: call <2 x double> @_mm_shuffle_pd(<2 x double> noundef %{{[0-9a-zA-Z_.]+}}, <2 x double> noundef %{{[0-9a-zA-Z_.]+}}, i32 noundef signext 0)

// CHECK-LABEL: define available_externally <4 x float> @_mm_movehdup_ps(<4 x float> noundef %{{[0-9a-zA-Z_.]+}})
// CHECK: call <4 x i32> @vec_mergeo(unsigned int vector[4], unsigned int vector[4])

// CHECK-LABEL: define available_externally <4 x float> @_mm_moveldup_ps(<4 x float> noundef %{{[0-9a-zA-Z_.]+}})
// CHECK: call <4 x i32> @vec_mergee(unsigned int vector[4], unsigned int vector[4])
