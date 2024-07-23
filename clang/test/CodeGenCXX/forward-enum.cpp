// RUN: %clang_cc1 -std=c++11 -triple x86_64-apple-darwin11.0.0 -emit-llvm -o - %s | FileCheck %s

enum MyEnum : char;
void bar(MyEnum value) { }

// CHECK-LABEL: define{{.*}} void @_Z3foo6MyEnum
void foo(MyEnum value)
{
  // CHECK: call void @_Z3bar6MyEnum(b8 noundef signext
  bar(value);
}
