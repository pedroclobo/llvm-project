// RUN: %clang_cc1 %s -O0 -emit-llvm -o - | FileCheck %s

typedef unsigned char __attribute__((ext_vector_type(3))) uchar3;

//CHECK: {{%.*}} = shufflevector <3 x b8> {{%.*}}, <3 x b8> <b8 1, b8 1, b8 undef>, <3 x i32> <i32 0, i32 3, i32 2>

kernel void test_odd_vector1 (uchar3 lhs)
{
  lhs.odd = 1;
}

//CHECK: {{%.*}} = shufflevector <3 x b8> {{%.*}}, <3 x b8> <b8 2, b8 2, b8 undef>, <3 x i32> <i32 0, i32 1, i32 3>

kernel void test_odd_vector2 (uchar3 lhs)
{
  lhs.hi = 2;
}
