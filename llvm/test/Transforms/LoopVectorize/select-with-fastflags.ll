; NOTE: Assertions have been autogenerated by utils/update_test_checks.py UTC_ARGS: --version 5

; RUN: opt < %s -passes=loop-vectorize -force-vector-width=4 -force-vector-interleave=1 -S | FileCheck %s

define void @select_with_fastmath_flags(ptr noalias %a, ptr noalias %b, ptr noalias %c, i64 %N) {
; CHECK-LABEL: define void @select_with_fastmath_flags(
; CHECK-SAME: ptr noalias [[A:%.*]], ptr noalias [[B:%.*]], ptr noalias [[C:%.*]], i64 [[N:%.*]]) {
; CHECK-NEXT:  [[ENTRY:.*]]:
; CHECK-NEXT:    [[MIN_ITERS_CHECK:%.*]] = icmp ult i64 [[N]], 4
; CHECK-NEXT:    br i1 [[MIN_ITERS_CHECK]], label %[[SCALAR_PH:.*]], label %[[VECTOR_PH:.*]]
; CHECK:       [[VECTOR_PH]]:
; CHECK-NEXT:    [[N_MOD_VF:%.*]] = urem i64 [[N]], 4
; CHECK-NEXT:    [[N_VEC:%.*]] = sub i64 [[N]], [[N_MOD_VF]]
; CHECK-NEXT:    br label %[[VECTOR_BODY:.*]]
; CHECK:       [[VECTOR_BODY]]:
; CHECK-NEXT:    [[TMP0:%.*]] = phi i64 [ 0, %[[VECTOR_PH]] ], [ [[INDEX_NEXT:%.*]], %[[VECTOR_BODY]] ]
; CHECK-NEXT:    [[TMP1:%.*]] = getelementptr inbounds nuw float, ptr [[B]], i64 [[TMP0]]
; CHECK-NEXT:    [[WIDE_LOAD:%.*]] = load <4 x float>, ptr [[TMP1]], align 4
; CHECK-NEXT:    [[TMP3:%.*]] = getelementptr inbounds nuw float, ptr [[C]], i64 [[TMP0]]
; CHECK-NEXT:    [[WIDE_LOAD1:%.*]] = load <4 x float>, ptr [[TMP3]], align 4
; CHECK-NEXT:    [[TMP5:%.*]] = fcmp fast ogt <4 x float> [[WIDE_LOAD]], [[WIDE_LOAD1]]
; CHECK-NEXT:    [[TMP6:%.*]] = fadd fast <4 x float> [[WIDE_LOAD]], splat (float 1.000000e+01)
; CHECK-NEXT:    [[TMP7:%.*]] = select fast <4 x i1> [[TMP5]], <4 x float> [[TMP6]], <4 x float> [[WIDE_LOAD1]]
; CHECK-NEXT:    [[TMP8:%.*]] = getelementptr inbounds nuw float, ptr [[A]], i64 [[TMP0]]
; CHECK-NEXT:    store <4 x float> [[TMP7]], ptr [[TMP8]], align 4
; CHECK-NEXT:    [[INDEX_NEXT]] = add nuw i64 [[TMP0]], 4
; CHECK-NEXT:    [[TMP10:%.*]] = icmp eq i64 [[INDEX_NEXT]], [[N_VEC]]
; CHECK-NEXT:    br i1 [[TMP10]], label %[[MIDDLE_BLOCK:.*]], label %[[VECTOR_BODY]], !llvm.loop [[LOOP0:![0-9]+]]
; CHECK:       [[MIDDLE_BLOCK]]:
; CHECK-NEXT:    [[CMP_N:%.*]] = icmp eq i64 [[N]], [[N_VEC]]
; CHECK-NEXT:    br i1 [[CMP_N]], label %[[EXIT:.*]], label %[[SCALAR_PH]]
; CHECK:       [[SCALAR_PH]]:
; CHECK-NEXT:    [[BC_RESUME_VAL:%.*]] = phi i64 [ [[N_VEC]], %[[MIDDLE_BLOCK]] ], [ 0, %[[ENTRY]] ]
; CHECK-NEXT:    br label %[[FOR_BODY:.*]]
; CHECK:       [[FOR_BODY]]:
; CHECK-NEXT:    [[IV:%.*]] = phi i64 [ [[IV_NEXT:%.*]], %[[FOR_BODY]] ], [ [[BC_RESUME_VAL]], %[[SCALAR_PH]] ]
; CHECK-NEXT:    [[GEP:%.*]] = getelementptr inbounds nuw float, ptr [[B]], i64 [[IV]]
; CHECK-NEXT:    [[TMP11:%.*]] = load float, ptr [[GEP]], align 4
; CHECK-NEXT:    [[GEP3:%.*]] = getelementptr inbounds nuw float, ptr [[C]], i64 [[IV]]
; CHECK-NEXT:    [[TMP12:%.*]] = load float, ptr [[GEP3]], align 4
; CHECK-NEXT:    [[CMP4:%.*]] = fcmp fast ogt float [[TMP11]], [[TMP12]]
; CHECK-NEXT:    [[ADD:%.*]] = fadd fast float [[TMP11]], 1.000000e+01
; CHECK-NEXT:    [[COND:%.*]] = select fast i1 [[CMP4]], float [[ADD]], float [[TMP12]]
; CHECK-NEXT:    [[GEP11:%.*]] = getelementptr inbounds nuw float, ptr [[A]], i64 [[IV]]
; CHECK-NEXT:    store float [[COND]], ptr [[GEP11]], align 4
; CHECK-NEXT:    [[IV_NEXT]] = add nuw nsw i64 [[IV]], 1
; CHECK-NEXT:    [[EXITCOND_NOT:%.*]] = icmp eq i64 [[IV_NEXT]], [[N]]
; CHECK-NEXT:    br i1 [[EXITCOND_NOT]], label %[[EXIT]], label %[[FOR_BODY]], !llvm.loop [[LOOP3:![0-9]+]]
; CHECK:       [[EXIT]]:
; CHECK-NEXT:    ret void
;
entry:
  br label %for.body

for.body:
  %iv = phi i64 [ %iv.next, %for.body ], [ 0, %entry ]
  %gep = getelementptr inbounds nuw float, ptr %b, i64 %iv
  %0 = load float, ptr %gep, align 4
  %gep3 = getelementptr inbounds nuw float, ptr %c, i64 %iv
  %1 = load float, ptr %gep3, align 4
  %cmp4 = fcmp fast ogt float %0, %1
  %add = fadd fast float %0, 1.000000e+01
  %cond = select fast i1 %cmp4, float %add, float %1
  %gep11 = getelementptr inbounds nuw float, ptr %a, i64 %iv
  store float %cond, ptr %gep11, align 4
  %iv.next = add nuw nsw i64 %iv, 1
  %exitcond.not = icmp eq i64 %iv.next, %N
  br i1 %exitcond.not, label %exit, label %for.body

exit:
  ret void
}

;.
; CHECK: [[LOOP0]] = distinct !{[[LOOP0]], [[META1:![0-9]+]], [[META2:![0-9]+]]}
; CHECK: [[META1]] = !{!"llvm.loop.isvectorized", i32 1}
; CHECK: [[META2]] = !{!"llvm.loop.unroll.runtime.disable"}
; CHECK: [[LOOP3]] = distinct !{[[LOOP3]], [[META2]], [[META1]]}
;.
