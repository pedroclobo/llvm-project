; NOTE: Assertions have been autogenerated by utils/update_test_checks.py
; RUN: opt -mtriple=amdgcn-amd-amdhsa -passes=loop-unroll -S %s -o - | FileCheck %s

%struct.wombat = type { %struct.zot, i32, [16 x i32], [16 x i32], i32, i32, [16 x i32], i32 }
%struct.zot = type { i32, i32, [1024 x i32] }

@global = external addrspace(3) global %struct.wombat

; Ensure that a cascaded GEP for local address space does not inhibit unrolling
;
define amdgpu_kernel void @unroll_when_cascaded_gep(i32 %arg) {
; CHECK-LABEL: @unroll_when_cascaded_gep(
; CHECK-NEXT:  bb:
; CHECK-NEXT:    [[TMP0:%.*]] = add i32 [[ARG:%.*]], 1
; CHECK-NEXT:    [[XTRAITER:%.*]] = and i32 [[TMP0]], 7
; CHECK-NEXT:    [[TMP1:%.*]] = icmp ult i32 [[ARG]], 7
; CHECK-NEXT:    br i1 [[TMP1]], label [[BB2_UNR_LCSSA:%.*]], label [[BB_NEW:%.*]]
; CHECK:       bb.new:
; CHECK-NEXT:    [[UNROLL_ITER:%.*]] = sub i32 [[TMP0]], [[XTRAITER]]
; CHECK-NEXT:    br label [[BB1:%.*]]
; CHECK:       bb1:
; CHECK-NEXT:    [[PHI:%.*]] = phi i32 [ 0, [[BB_NEW]] ], [ [[ADD_7:%.*]], [[BB1]] ]
; CHECK-NEXT:    [[NITER:%.*]] = phi i32 [ 0, [[BB_NEW]] ], [ [[NITER_NEXT_7:%.*]], [[BB1]] ]
; CHECK-NEXT:    [[ADD_7]] = add i32 [[PHI]], 8
; CHECK-NEXT:    [[NITER_NEXT_7]] = add i32 [[NITER]], 8
; CHECK-NEXT:    [[NITER_NCMP_7:%.*]] = icmp eq i32 [[NITER_NEXT_7]], [[UNROLL_ITER]]
; CHECK-NEXT:    br i1 [[NITER_NCMP_7]], label [[BB2_UNR_LCSSA_LOOPEXIT:%.*]], label [[BB1]]
; CHECK:       bb2.unr-lcssa.loopexit:
; CHECK-NEXT:    [[PHI_UNR_PH:%.*]] = phi i32 [ [[ADD_7]], [[BB1]] ]
; CHECK-NEXT:    br label [[BB2_UNR_LCSSA]]
; CHECK:       bb2.unr-lcssa:
; CHECK-NEXT:    [[PHI_UNR:%.*]] = phi i32 [ 0, [[BB:%.*]] ], [ [[PHI_UNR_PH]], [[BB2_UNR_LCSSA_LOOPEXIT]] ]
; CHECK-NEXT:    [[LCMP_MOD:%.*]] = icmp ne i32 [[XTRAITER]], 0
; CHECK-NEXT:    br i1 [[LCMP_MOD]], label [[BB1_EPIL_PREHEADER:%.*]], label [[BB2:%.*]]
; CHECK:       bb1.epil.preheader:
; CHECK-NEXT:    br label [[BB1_EPIL:%.*]]
; CHECK:       bb1.epil:
; CHECK-NEXT:    [[PHI_EPIL:%.*]] = phi i32 [ [[PHI_UNR]], [[BB1_EPIL_PREHEADER]] ], [ [[ADD_EPIL:%.*]], [[BB1_EPIL]] ]
; CHECK-NEXT:    [[EPIL_ITER:%.*]] = phi i32 [ 0, [[BB1_EPIL_PREHEADER]] ], [ [[EPIL_ITER_NEXT:%.*]], [[BB1_EPIL]] ]
; CHECK-NEXT:    [[GETELEMENTPTR_EPIL:%.*]] = getelementptr [1024 x i32], ptr addrspace(3) getelementptr inbounds nuw (i8, ptr addrspace(3) @global, i32 8), i32 0, i32 0
; CHECK-NEXT:    [[ADD_EPIL]] = add i32 [[PHI_EPIL]], 1
; CHECK-NEXT:    [[ICMP_EPIL:%.*]] = icmp eq i32 [[PHI_EPIL]], [[ARG]]
; CHECK-NEXT:    [[EPIL_ITER_NEXT]] = add i32 [[EPIL_ITER]], 1
; CHECK-NEXT:    [[EPIL_ITER_CMP:%.*]] = icmp ne i32 [[EPIL_ITER_NEXT]], [[XTRAITER]]
; CHECK-NEXT:    br i1 [[EPIL_ITER_CMP]], label [[BB1_EPIL]], label [[BB2_EPILOG_LCSSA:%.*]], !llvm.loop [[LOOP0:![0-9]+]]
; CHECK:       bb2.epilog-lcssa:
; CHECK-NEXT:    br label [[BB2]]
; CHECK:       bb2:
; CHECK-NEXT:    ret void
;
bb:
  br label %bb1

bb1:                                              ; preds = %bb1, %bb
  %phi = phi i32 [ 0, %bb ], [ %add, %bb1 ]
  %getelementptr = getelementptr [1024 x i32], ptr addrspace(3) getelementptr inbounds nuw (i8, ptr addrspace(3) @global, i32 8), i32 0, i32 0
  %add = add i32 %phi, 1
  %icmp = icmp eq i32 %phi, %arg
  br i1 %icmp, label %bb2, label %bb1

bb2:                                              ; preds = %bb1
  ret void
}

