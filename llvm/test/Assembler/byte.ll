; RUN: llvm-as < %s | llvm-dis | llvm-as | llvm-dis | FileCheck %s
; RUN: verify-uselistorder %s

; CHECK: common global [32 x b8] zeroinitializer
; CHECK: constant [1 x b8] zeroinitializer
; CHECK: constant [15 x b8] c"Hello, World!\0A\00"
; CHECK: constant [15 x b8] c"Hello, World!\0A\00"
@a = common global [32 x b8] zeroinitializer, align 1
@b = constant [1 x b8] zeroinitializer
@c = constant [15 x b8] [b8 72, b8 101, b8 108, b8 108, b8 111, b8 44, b8 32,  b8 87, b8 111, b8 114, b8 108, b8 100, b8 33,  b8 10, b8 0]
@d = constant [15 x b8] c"Hello, World!\0A\00"

; CHECK: @bytes(b1 %{{.*}}, b8 %{{.*}}, b16 %{{.*}}, b32 %{{.*}}, b64 %{{.*}}, b128 %{{.*}})
define void @bytes(b1 %a, b8 %b, b16 %c, b32 %d, b64 %e, b128 %f) {
  ret void
}

; CHECK-LABEL: byte_alloca
; CHECK: alloca b8
; CHECK: alloca b64
define void @byte_alloca() {
  %b1 = alloca b8, align 1
  %b8 = alloca b64, align 8
  ret void
}

; CHECK-LABEL: byte_load
; CHECK: load b8, ptr %{{.*}}
define void @byte_load(ptr %ptr) {
  %v = load b8, ptr %ptr, align 1
  ret void
}

; CHECK-LABEL: byte_store
; CHECK: store b8 %{{.*}}, ptr %{{.*}}
define void @byte_store(b8 %v, ptr %ptr) {
  store b8 %v, ptr %ptr, align 1
  ret void
}

; CHECK-LABEL: bitcasts
; CHECK: bitcast ptr %{{.*}} to b64
; CHECK: bitcast i64 %{{.*}} to b64
; CHECK: bitcast b64 %{{.*}} to <8 x b8>
; CHECK: bitcast <8 x b8> %{{.*}} to <2 x b32>
; CHECK: bitcast <2 x b32> %{{.*}} to b64
define void @bitcasts(i64 %i, b64 %s, ptr %p) {
  %1 = bitcast ptr %p to b64
  %2 = bitcast i64 %i to b64
  %3 = bitcast b64 %s to <8 x b8>
  %4 = bitcast <8 x b8> %3 to <2 x b32>
  %5 = bitcast <2 x b32> %4 to b64
  ret void
}

; CHECK-LABEL: byte_constant
; CHECK: store b8 0, ptr %{{.*}}
define void @byte_constant(ptr %ptr) {
  store b8 0, ptr %ptr, align 1
  ret void
}

; CHECK-LABEL: bytecasts
; CHECK: bytecast b8 %{{.*}} to i8
; CHECK: bytecast b16 %{{.*}} to i16
; CHECK: bytecast b32 %{{.*}} to i32
; CHECK: bytecast b64 %{{.*}} to i64
; CHECK: bytecast b64 %{{.*}} to ptr
; CHECK: bytecast b16 %{{.*}} to half
; CHECK: bytecast b32 %{{.*}} to float
; CHECK: bytecast b64 %{{.*}} to double
; CHECK: bytecast exact b8 %{{.*}} to i8
define void @bytecasts(b8 %b1, b16 %b2, b32 %b3, b64 %b4, b128 %b5) {
  %1 = bytecast b8 %b1 to i8
  %2 = bytecast b16 %b2 to i16
  %3 = bytecast b32 %b3 to i32
  %4 = bytecast b64 %b4 to i64
  %5 = bytecast b64 %b4 to ptr
  %6 = bytecast b16 %b2 to half
  %7 = bytecast b32 %b3 to float
  %8 = bytecast b64 %b4 to double
  %9 = bytecast exact b8 %b1 to i8
  ret void
}

; CHECK-LABEL: vector_bytecasts
; CHECK: bytecast <2 x b64> %{{.*}} to <2 x i64>
; CHECK: bytecast <2 x b64> %{{.*}} to <2 x double>
; CHECK: bytecast <2 x b64> %{{.*}} to <2 x ptr>
; CHECK: bytecast b64 %{{.*}} to <2 x i32>
; CHECK: bytecast b64 %{{.*}} to <1 x ptr>
; CHECK: bytecast <2 x b64> %{{.*}} to i128
; CHECK: bytecast <2 x b64> %{{.*}} to <4 x i32>
; CHECK: bytecast <2 x b128> %{{.*}} to <4 x ptr>
define void @vector_bytecasts(<2 x b64> %b1, <2 x b128> %b2, b64 %b3) {
  %1 = bytecast <2 x b64> %b1 to <2 x i64>
  %2 = bytecast <2 x b64> %b1 to <2 x double>
  %3 = bytecast <2 x b64> %b1 to <2 x ptr>
  %4 = bytecast b64 %b3 to <2 x i32>
  %5 = bytecast b64 %b3 to <1 x ptr>
  %6 = bytecast <2 x b64> %b1 to i128
  %7 = bytecast <2 x b64> %b1 to <4 x i32>
  %8 = bytecast <2 x b128> %b2 to <4 x ptr>
  ret void
}

; CHECK-LABEL: trunc
; CHECK: trunc b16 %{{.*}} to b8
; CHECK: trunc <2 x b16> %{{.*}} to <2 x b8>
define void @trunc(b16 %b, <2 x b16> %v) {
  %t1 = trunc b16 %b to b8
  %t2 = trunc <2 x b16> %v to <2 x b8>
  ret void
}
