; NOTE: Assertions have been autogenerated by utils/update_test_checks.py UTC_ARGS: --version 5
; RUN: opt -passes='print<scalar-evolution>,loop-vectorize' -force-vector-width=4 -scalar-evolution-classify-expressions=false -S %s | FileCheck %s

; Test case for https://github.com/llvm/llvm-project/issues/119665.

; %loop.2's backedge-taken-count depends on %add.1 from %loop.1 via its
; corresponding SCEV at the scope of %loop.2. After vectorizing %loop.1, %add.1
; isn't available at the entry of %loop.2 anymore and %add.1 at %loop.2's scope
; must be invalidated, as well as %loop.2's backedge-taken count.
define void @test_invalidate_scevs_at_scope(ptr %p) {
; CHECK-LABEL: define void @test_invalidate_scevs_at_scope(
; CHECK-SAME: ptr [[P:%.*]]) {
; CHECK-NEXT:  [[ENTRY:.*]]:
; CHECK-NEXT:    br i1 false, label %[[SCALAR_PH:.*]], label %[[VECTOR_PH:.*]]
; CHECK:       [[VECTOR_PH]]:
; CHECK-NEXT:    br label %[[VECTOR_BODY:.*]]
; CHECK:       [[VECTOR_BODY]]:
; CHECK-NEXT:    [[INDEX:%.*]] = phi i32 [ 0, %[[VECTOR_PH]] ], [ [[INDEX_NEXT:%.*]], %[[VECTOR_BODY]] ]
; CHECK-NEXT:    [[INDEX_NEXT]] = add nuw i32 [[INDEX]], 4
; CHECK-NEXT:    [[TMP2:%.*]] = icmp eq i32 [[INDEX_NEXT]], 100
; CHECK-NEXT:    br i1 [[TMP2]], label %[[MIDDLE_BLOCK:.*]], label %[[VECTOR_BODY]], !llvm.loop [[LOOP0:![0-9]+]]
; CHECK:       [[MIDDLE_BLOCK]]:
; CHECK-NEXT:    br label %[[SCALAR_PH]]
; CHECK:       [[SCALAR_PH]]:
; CHECK-NEXT:    [[BC_RESUME_VAL:%.*]] = phi i32 [ 100, %[[MIDDLE_BLOCK]] ], [ 0, %[[ENTRY]] ]
; CHECK-NEXT:    br label %[[LOOP_1:.*]]
; CHECK:       [[LOOP_1]]:
; CHECK-NEXT:    [[IV_1:%.*]] = phi i32 [ [[BC_RESUME_VAL]], %[[SCALAR_PH]] ], [ [[IV_1_NEXT:%.*]], %[[LOOP_1]] ]
; CHECK-NEXT:    [[TMP4:%.*]] = load i32, ptr [[P]], align 4
; CHECK-NEXT:    [[ADD_1:%.*]] = add i32 [[TMP4]], [[IV_1]]
; CHECK-NEXT:    [[IV_1_NEXT]] = add i32 [[IV_1]], 1
; CHECK-NEXT:    [[C_1:%.*]] = icmp eq i32 [[IV_1]], 100
; CHECK-NEXT:    br i1 [[C_1]], label %[[EXIT_1:.*]], label %[[LOOP_1]], !llvm.loop [[LOOP3:![0-9]+]]
; CHECK:       [[EXIT_1]]:
; CHECK-NEXT:    [[DOTLCSSA:%.*]] = phi i32 [ [[TMP4]], %[[LOOP_1]] ]
; CHECK-NEXT:    [[ADD_LCSSA1:%.*]] = phi i32 [ [[ADD_1]], %[[LOOP_1]] ]
; CHECK-NEXT:    [[ADD_LCSSA:%.*]] = add i32 [[DOTLCSSA]], 100
; CHECK-NEXT:    [[SMAX:%.*]] = call i32 @llvm.smax.i32(i32 [[ADD_LCSSA]], i32 100)
; CHECK-NEXT:    [[TMP3:%.*]] = add i32 [[SMAX]], -100
; CHECK-NEXT:    [[TMP5:%.*]] = sub i32 [[TMP3]], [[DOTLCSSA]]
; CHECK-NEXT:    [[TMP6:%.*]] = zext i32 [[TMP5]] to i64
; CHECK-NEXT:    [[TMP7:%.*]] = add nuw nsw i64 [[TMP6]], 1
; CHECK-NEXT:    [[MIN_ITERS_CHECK:%.*]] = icmp ult i64 [[TMP7]], 4
; CHECK-NEXT:    br i1 [[MIN_ITERS_CHECK]], label %[[SCALAR_PH1:.*]], label %[[VECTOR_PH2:.*]]
; CHECK:       [[VECTOR_PH2]]:
; CHECK-NEXT:    [[N_MOD_VF:%.*]] = urem i64 [[TMP7]], 4
; CHECK-NEXT:    [[N_VEC:%.*]] = sub i64 [[TMP7]], [[N_MOD_VF]]
; CHECK-NEXT:    br label %[[VECTOR_BODY3:.*]]
; CHECK:       [[VECTOR_BODY3]]:
; CHECK-NEXT:    [[INDEX5:%.*]] = phi i64 [ 0, %[[VECTOR_PH2]] ], [ [[INDEX_NEXT8:%.*]], %[[VECTOR_BODY3]] ]
; CHECK-NEXT:    [[VEC_IND6:%.*]] = phi <4 x i64> [ <i64 0, i64 1, i64 2, i64 3>, %[[VECTOR_PH2]] ], [ [[VEC_IND_NEXT7:%.*]], %[[VECTOR_BODY3]] ]
; CHECK-NEXT:    [[TMP9:%.*]] = getelementptr inbounds i64, ptr [[P]], i64 [[INDEX5]]
; CHECK-NEXT:    store <4 x i64> [[VEC_IND6]], ptr [[TMP9]], align 4
; CHECK-NEXT:    [[INDEX_NEXT8]] = add nuw i64 [[INDEX5]], 4
; CHECK-NEXT:    [[VEC_IND_NEXT7]] = add <4 x i64> [[VEC_IND6]], splat (i64 4)
; CHECK-NEXT:    [[TMP11:%.*]] = icmp eq i64 [[INDEX_NEXT8]], [[N_VEC]]
; CHECK-NEXT:    br i1 [[TMP11]], label %[[MIDDLE_BLOCK6:.*]], label %[[VECTOR_BODY3]], !llvm.loop [[LOOP4:![0-9]+]]
; CHECK:       [[MIDDLE_BLOCK6]]:
; CHECK-NEXT:    [[CMP_N:%.*]] = icmp eq i64 [[TMP7]], [[N_VEC]]
; CHECK-NEXT:    br i1 [[CMP_N]], label %[[EXIT_2:.*]], label %[[SCALAR_PH1]]
; CHECK:       [[SCALAR_PH1]]:
; CHECK-NEXT:    [[BC_RESUME_VAL7:%.*]] = phi i64 [ [[N_VEC]], %[[MIDDLE_BLOCK6]] ], [ 0, %[[EXIT_1]] ]
; CHECK-NEXT:    br label %[[LOOP_2:.*]]
; CHECK:       [[LOOP_2]]:
; CHECK-NEXT:    [[IV_2:%.*]] = phi i64 [ [[BC_RESUME_VAL7]], %[[SCALAR_PH1]] ], [ [[IV_2_NEXT:%.*]], %[[LOOP_2]] ]
; CHECK-NEXT:    [[IV_2_TRUNC:%.*]] = trunc i64 [[IV_2]] to i32
; CHECK-NEXT:    [[IV_2_NEXT]] = add i64 [[IV_2]], 1
; CHECK-NEXT:    [[GEP:%.*]] = getelementptr inbounds i64, ptr [[P]], i64 [[IV_2]]
; CHECK-NEXT:    store i64 [[IV_2]], ptr [[GEP]], align 4
; CHECK-NEXT:    [[ADD_2:%.*]] = add i32 [[ADD_LCSSA1]], [[IV_2_TRUNC]]
; CHECK-NEXT:    [[C_2:%.*]] = icmp slt i32 [[ADD_2]], 100
; CHECK-NEXT:    br i1 [[C_2]], label %[[LOOP_2]], label %[[EXIT_2]], !llvm.loop [[LOOP5:![0-9]+]]
; CHECK:       [[EXIT_2]]:
; CHECK-NEXT:    ret void
;
entry:
  br label %loop.1

loop.1:
  %iv.1 = phi i32 [ 0, %entry ], [ %iv.1.next, %loop.1 ]
  %1 = load i32, ptr %p, align 4
  %add.1 = add i32 %1, %iv.1
  %iv.1.next = add i32 %iv.1, 1
  %c.1 = icmp eq i32 %iv.1, 100
  br i1 %c.1, label %exit.1, label %loop.1

exit.1:
  %add.lcssa = phi i32 [ %add.1, %loop.1 ]
  br label %loop.2

loop.2:
  %iv.2 = phi i64 [ 0, %exit.1 ], [ %iv.2.next, %loop.2 ]
  %iv.2.trunc = trunc i64 %iv.2 to i32
  %iv.2.next = add i64 %iv.2, 1
  %gep = getelementptr inbounds i64, ptr %p, i64 %iv.2
  store i64 %iv.2, ptr %gep
  %add.2 = add i32 %add.lcssa, %iv.2.trunc
  %c.2 = icmp slt i32 %add.2, 100
  br i1 %c.2, label %loop.2, label %exit.2

exit.2:
  ret void
}

;.
; CHECK: [[LOOP0]] = distinct !{[[LOOP0]], [[META1:![0-9]+]], [[META2:![0-9]+]]}
; CHECK: [[META1]] = !{!"llvm.loop.isvectorized", i32 1}
; CHECK: [[META2]] = !{!"llvm.loop.unroll.runtime.disable"}
; CHECK: [[LOOP3]] = distinct !{[[LOOP3]], [[META2]], [[META1]]}
; CHECK: [[LOOP4]] = distinct !{[[LOOP4]], [[META1]], [[META2]]}
; CHECK: [[LOOP5]] = distinct !{[[LOOP5]], [[META2]], [[META1]]}
;.
