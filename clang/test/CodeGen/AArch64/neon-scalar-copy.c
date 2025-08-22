// RUN: %clang_cc1 -triple arm64-none-linux-gnu -target-feature +neon -flax-vector-conversions=none\
// RUN: -disable-O0-optnone -emit-llvm -o - %s | opt -S -passes=mem2reg | FileCheck %s

// REQUIRES: aarch64-registered-target || arm-registered-target

#include <arm_neon.h>

// CHECK-LABEL: define dso_local float @test_vdups_lane_f32(
// CHECK-SAME: <2 x float> noundef [[A:%.*]]) #[[ATTR0:[0-9]+]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    [[VDUPS_LANE:%.*]] = extractelement <2 x float> [[A]], i32 1
// CHECK-NEXT:    ret float [[VDUPS_LANE]]
//
float32_t test_vdups_lane_f32(float32x2_t a) {
  return vdups_lane_f32(a, 1);
}


// CHECK-LABEL: define dso_local double @test_vdupd_lane_f64(
// CHECK-SAME: <1 x double> noundef [[A:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    [[VDUPD_LANE:%.*]] = extractelement <1 x double> [[A]], i32 0
// CHECK-NEXT:    ret double [[VDUPD_LANE]]
//
float64_t test_vdupd_lane_f64(float64x1_t a) {
  return vdupd_lane_f64(a, 0);
}


// CHECK-LABEL: define dso_local float @test_vdups_laneq_f32(
// CHECK-SAME: <4 x float> noundef [[A:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    [[VGETQ_LANE:%.*]] = extractelement <4 x float> [[A]], i32 3
// CHECK-NEXT:    ret float [[VGETQ_LANE]]
//
float32_t test_vdups_laneq_f32(float32x4_t a) {
  return vdups_laneq_f32(a, 3);
}


// CHECK-LABEL: define dso_local double @test_vdupd_laneq_f64(
// CHECK-SAME: <2 x double> noundef [[A:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    [[VGETQ_LANE:%.*]] = extractelement <2 x double> [[A]], i32 1
// CHECK-NEXT:    ret double [[VGETQ_LANE]]
//
float64_t test_vdupd_laneq_f64(float64x2_t a) {
  return vdupd_laneq_f64(a, 1);
}


// CHECK-LABEL: define dso_local b8 @test_vdupb_lane_s8(
// CHECK-SAME: <8 x b8> noundef [[A:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    [[REF_TMP:%.*]] = alloca b8, align 1
// CHECK-NEXT:    [[TMP0:%.*]] = bytecast exact <8 x b8> [[A]] to <8 x i8>
// CHECK-NEXT:    [[VGET_LANE:%.*]] = extractelement <8 x i8> [[TMP0]], i32 7
// CHECK-NEXT:    store i8 [[VGET_LANE]], ptr [[REF_TMP]], align 1
// CHECK-NEXT:    [[TMP1:%.*]] = load b8, ptr [[REF_TMP]], align 1
// CHECK-NEXT:    ret b8 [[TMP1]]
//
int8_t test_vdupb_lane_s8(int8x8_t a) {
  return vdupb_lane_s8(a, 7);
}


// CHECK-LABEL: define dso_local i16 @test_vduph_lane_s16(
// CHECK-SAME: <4 x i16> noundef [[A:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    [[VGET_LANE:%.*]] = extractelement <4 x i16> [[A]], i32 3
// CHECK-NEXT:    ret i16 [[VGET_LANE]]
//
int16_t test_vduph_lane_s16(int16x4_t a) {
  return vduph_lane_s16(a, 3);
}


// CHECK-LABEL: define dso_local i32 @test_vdups_lane_s32(
// CHECK-SAME: <2 x i32> noundef [[A:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    [[VGET_LANE:%.*]] = extractelement <2 x i32> [[A]], i32 1
// CHECK-NEXT:    ret i32 [[VGET_LANE]]
//
int32_t test_vdups_lane_s32(int32x2_t a) {
  return vdups_lane_s32(a, 1);
}


// CHECK-LABEL: define dso_local i64 @test_vdupd_lane_s64(
// CHECK-SAME: <1 x i64> noundef [[A:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    [[VGET_LANE:%.*]] = extractelement <1 x i64> [[A]], i32 0
// CHECK-NEXT:    ret i64 [[VGET_LANE]]
//
int64_t test_vdupd_lane_s64(int64x1_t a) {
  return vdupd_lane_s64(a, 0);
}


// CHECK-LABEL: define dso_local b8 @test_vdupb_lane_u8(
// CHECK-SAME: <8 x b8> noundef [[A:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    [[REF_TMP:%.*]] = alloca b8, align 1
// CHECK-NEXT:    [[TMP0:%.*]] = bytecast exact <8 x b8> [[A]] to <8 x i8>
// CHECK-NEXT:    [[VGET_LANE:%.*]] = extractelement <8 x i8> [[TMP0]], i32 7
// CHECK-NEXT:    store i8 [[VGET_LANE]], ptr [[REF_TMP]], align 1
// CHECK-NEXT:    [[TMP1:%.*]] = load b8, ptr [[REF_TMP]], align 1
// CHECK-NEXT:    ret b8 [[TMP1]]
//
uint8_t test_vdupb_lane_u8(uint8x8_t a) {
  return vdupb_lane_u8(a, 7);
}


// CHECK-LABEL: define dso_local i16 @test_vduph_lane_u16(
// CHECK-SAME: <4 x i16> noundef [[A:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    [[VGET_LANE:%.*]] = extractelement <4 x i16> [[A]], i32 3
// CHECK-NEXT:    ret i16 [[VGET_LANE]]
//
uint16_t test_vduph_lane_u16(uint16x4_t a) {
  return vduph_lane_u16(a, 3);
}


// CHECK-LABEL: define dso_local i32 @test_vdups_lane_u32(
// CHECK-SAME: <2 x i32> noundef [[A:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    [[VGET_LANE:%.*]] = extractelement <2 x i32> [[A]], i32 1
// CHECK-NEXT:    ret i32 [[VGET_LANE]]
//
uint32_t test_vdups_lane_u32(uint32x2_t a) {
  return vdups_lane_u32(a, 1);
}


// CHECK-LABEL: define dso_local i64 @test_vdupd_lane_u64(
// CHECK-SAME: <1 x i64> noundef [[A:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    [[VGET_LANE:%.*]] = extractelement <1 x i64> [[A]], i32 0
// CHECK-NEXT:    ret i64 [[VGET_LANE]]
//
uint64_t test_vdupd_lane_u64(uint64x1_t a) {
  return vdupd_lane_u64(a, 0);
}

// CHECK-LABEL: define dso_local b8 @test_vdupb_laneq_s8(
// CHECK-SAME: <16 x b8> noundef [[A:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    [[REF_TMP:%.*]] = alloca b8, align 1
// CHECK-NEXT:    [[TMP0:%.*]] = bytecast exact <16 x b8> [[A]] to <16 x i8>
// CHECK-NEXT:    [[VGETQ_LANE:%.*]] = extractelement <16 x i8> [[TMP0]], i32 15
// CHECK-NEXT:    store i8 [[VGETQ_LANE]], ptr [[REF_TMP]], align 1
// CHECK-NEXT:    [[TMP1:%.*]] = load b8, ptr [[REF_TMP]], align 1
// CHECK-NEXT:    ret b8 [[TMP1]]
//
int8_t test_vdupb_laneq_s8(int8x16_t a) {
  return vdupb_laneq_s8(a, 15);
}


// CHECK-LABEL: define dso_local i16 @test_vduph_laneq_s16(
// CHECK-SAME: <8 x i16> noundef [[A:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    [[VGETQ_LANE:%.*]] = extractelement <8 x i16> [[A]], i32 7
// CHECK-NEXT:    ret i16 [[VGETQ_LANE]]
//
int16_t test_vduph_laneq_s16(int16x8_t a) {
  return vduph_laneq_s16(a, 7);
}


// CHECK-LABEL: define dso_local i32 @test_vdups_laneq_s32(
// CHECK-SAME: <4 x i32> noundef [[A:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    [[VGETQ_LANE:%.*]] = extractelement <4 x i32> [[A]], i32 3
// CHECK-NEXT:    ret i32 [[VGETQ_LANE]]
//
int32_t test_vdups_laneq_s32(int32x4_t a) {
  return vdups_laneq_s32(a, 3);
}


// CHECK-LABEL: define dso_local i64 @test_vdupd_laneq_s64(
// CHECK-SAME: <2 x i64> noundef [[A:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    [[VGETQ_LANE:%.*]] = extractelement <2 x i64> [[A]], i32 1
// CHECK-NEXT:    ret i64 [[VGETQ_LANE]]
//
int64_t test_vdupd_laneq_s64(int64x2_t a) {
  return vdupd_laneq_s64(a, 1);
}


// CHECK-LABEL: define dso_local b8 @test_vdupb_laneq_u8(
// CHECK-SAME: <16 x b8> noundef [[A:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    [[REF_TMP:%.*]] = alloca b8, align 1
// CHECK-NEXT:    [[TMP0:%.*]] = bytecast exact <16 x b8> [[A]] to <16 x i8>
// CHECK-NEXT:    [[VGETQ_LANE:%.*]] = extractelement <16 x i8> [[TMP0]], i32 15
// CHECK-NEXT:    store i8 [[VGETQ_LANE]], ptr [[REF_TMP]], align 1
// CHECK-NEXT:    [[TMP1:%.*]] = load b8, ptr [[REF_TMP]], align 1
// CHECK-NEXT:    ret b8 [[TMP1]]
//
uint8_t test_vdupb_laneq_u8(uint8x16_t a) {
  return vdupb_laneq_u8(a, 15);
}


// CHECK-LABEL: define dso_local i16 @test_vduph_laneq_u16(
// CHECK-SAME: <8 x i16> noundef [[A:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    [[VGETQ_LANE:%.*]] = extractelement <8 x i16> [[A]], i32 7
// CHECK-NEXT:    ret i16 [[VGETQ_LANE]]
//
uint16_t test_vduph_laneq_u16(uint16x8_t a) {
  return vduph_laneq_u16(a, 7);
}


// CHECK-LABEL: define dso_local i32 @test_vdups_laneq_u32(
// CHECK-SAME: <4 x i32> noundef [[A:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    [[VGETQ_LANE:%.*]] = extractelement <4 x i32> [[A]], i32 3
// CHECK-NEXT:    ret i32 [[VGETQ_LANE]]
//
uint32_t test_vdups_laneq_u32(uint32x4_t a) {
  return vdups_laneq_u32(a, 3);
}


// CHECK-LABEL: define dso_local i64 @test_vdupd_laneq_u64(
// CHECK-SAME: <2 x i64> noundef [[A:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    [[VGETQ_LANE:%.*]] = extractelement <2 x i64> [[A]], i32 1
// CHECK-NEXT:    ret i64 [[VGETQ_LANE]]
//
uint64_t test_vdupd_laneq_u64(uint64x2_t a) {
  return vdupd_laneq_u64(a, 1);
}

// CHECK-LABEL: define dso_local b8 @test_vdupb_lane_p8(
// CHECK-SAME: <8 x b8> noundef [[A:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    [[REF_TMP:%.*]] = alloca b8, align 1
// CHECK-NEXT:    [[TMP0:%.*]] = bytecast exact <8 x b8> [[A]] to <8 x i8>
// CHECK-NEXT:    [[VGET_LANE:%.*]] = extractelement <8 x i8> [[TMP0]], i32 7
// CHECK-NEXT:    store i8 [[VGET_LANE]], ptr [[REF_TMP]], align 1
// CHECK-NEXT:    [[TMP1:%.*]] = load b8, ptr [[REF_TMP]], align 1
// CHECK-NEXT:    ret b8 [[TMP1]]
//
poly8_t test_vdupb_lane_p8(poly8x8_t a) {
  return vdupb_lane_p8(a, 7);
}

// CHECK-LABEL: define dso_local i16 @test_vduph_lane_p16(
// CHECK-SAME: <4 x i16> noundef [[A:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    [[VGET_LANE:%.*]] = extractelement <4 x i16> [[A]], i32 3
// CHECK-NEXT:    ret i16 [[VGET_LANE]]
//
poly16_t test_vduph_lane_p16(poly16x4_t a) {
  return vduph_lane_p16(a, 3);
}

// CHECK-LABEL: define dso_local b8 @test_vdupb_laneq_p8(
// CHECK-SAME: <16 x b8> noundef [[A:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    [[REF_TMP:%.*]] = alloca b8, align 1
// CHECK-NEXT:    [[TMP0:%.*]] = bytecast exact <16 x b8> [[A]] to <16 x i8>
// CHECK-NEXT:    [[VGETQ_LANE:%.*]] = extractelement <16 x i8> [[TMP0]], i32 15
// CHECK-NEXT:    store i8 [[VGETQ_LANE]], ptr [[REF_TMP]], align 1
// CHECK-NEXT:    [[TMP1:%.*]] = load b8, ptr [[REF_TMP]], align 1
// CHECK-NEXT:    ret b8 [[TMP1]]
//
poly8_t test_vdupb_laneq_p8(poly8x16_t a) {
  return vdupb_laneq_p8(a, 15);
}

// CHECK-LABEL: define dso_local i16 @test_vduph_laneq_p16(
// CHECK-SAME: <8 x i16> noundef [[A:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    [[VGETQ_LANE:%.*]] = extractelement <8 x i16> [[A]], i32 7
// CHECK-NEXT:    ret i16 [[VGETQ_LANE]]
//
poly16_t test_vduph_laneq_p16(poly16x8_t a) {
  return vduph_laneq_p16(a, 7);
}
