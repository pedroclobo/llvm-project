; RUN: llc -mtriple=amdgcn < %s | FileCheck -allow-deprecated-dag-overlap -check-prefix=SI -check-prefix=FUNC %s
; RUN: llc -mtriple=amdgcn -mcpu=tonga -mattr=-flat-for-global < %s | FileCheck -allow-deprecated-dag-overlap -check-prefix=SI -check-prefix=FUNC %s
; RUN: llc -mtriple=r600 -mcpu=redwood < %s | FileCheck -allow-deprecated-dag-overlap -check-prefix=R600 -check-prefix=FUNC %s

; FUNC-LABEL: {{^}}s_uint_to_fp_i32_to_f32:
; SI: v_cvt_f32_u32_e32

; R600: UINT_TO_FLT * T{{[0-9]+\.[XYZW]}}, KC0[2].Z
define amdgpu_kernel void @s_uint_to_fp_i32_to_f32(ptr addrspace(1) %out, i32 %in) #0 {
  %result = uitofp i32 %in to float
  store float %result, ptr addrspace(1) %out
  ret void
}

; FUNC-LABEL: {{^}}v_uint_to_fp_i32_to_f32:
; SI: v_cvt_f32_u32_e32 {{v[0-9]+}}, {{v[0-9]+$}}

; R600: INT_TO_FLT
define amdgpu_kernel void @v_uint_to_fp_i32_to_f32(ptr addrspace(1) %out, ptr addrspace(1) %in) #0 {
  %tid = call i32 @llvm.amdgcn.workitem.id.x()
  %in.gep = getelementptr i32, ptr addrspace(1) %in, i32 %tid
  %out.gep = getelementptr float, ptr addrspace(1) %out, i32 %tid
  %val = load i32, ptr addrspace(1) %in.gep
  %result = uitofp i32 %val to float
  store float %result, ptr addrspace(1) %out.gep
  ret void
}

; FUNC-LABEL: {{^}}s_uint_to_fp_v2i32_to_v2f32:
; SI: v_cvt_f32_u32_e32
; SI: v_cvt_f32_u32_e32

; R600-DAG: UINT_TO_FLT * T{{[0-9]+\.[XYZW]}}, KC0[2].W
; R600-DAG: UINT_TO_FLT * T{{[0-9]+\.[XYZW]}}, KC0[3].X
define amdgpu_kernel void @s_uint_to_fp_v2i32_to_v2f32(ptr addrspace(1) %out, <2 x i32> %in) #0 {
  %result = uitofp <2 x i32> %in to <2 x float>
  store <2 x float> %result, ptr addrspace(1) %out
  ret void
}

; FUNC-LABEL: {{^}}s_uint_to_fp_v4i32_to_v4f32:
; SI: v_cvt_f32_u32_e32
; SI: v_cvt_f32_u32_e32
; SI: v_cvt_f32_u32_e32
; SI: v_cvt_f32_u32_e32
; SI: s_endpgm

; R600: UINT_TO_FLT * T{{[0-9]+\.[XYZW], T[0-9]+\.[XYZW]}}
; R600: UINT_TO_FLT * T{{[0-9]+\.[XYZW], T[0-9]+\.[XYZW]}}
; R600: UINT_TO_FLT * T{{[0-9]+\.[XYZW], T[0-9]+\.[XYZW]}}
; R600: UINT_TO_FLT * T{{[0-9]+\.[XYZW], T[0-9]+\.[XYZW]}}
define amdgpu_kernel void @s_uint_to_fp_v4i32_to_v4f32(ptr addrspace(1) %out, ptr addrspace(1) %in) #0 {
  %value = load <4 x i32>, ptr addrspace(1) %in
  %result = uitofp <4 x i32> %value to <4 x float>
  store <4 x float> %result, ptr addrspace(1) %out
  ret void
}

; FUNC-LABEL: {{^}}v_uint_to_fp_v4i32:
; SI: v_cvt_f32_u32_e32
; SI: v_cvt_f32_u32_e32
; SI: v_cvt_f32_u32_e32
; SI: v_cvt_f32_u32_e32

; R600: UINT_TO_FLT * T{{[0-9]+\.[XYZW], T[0-9]+\.[XYZW]}}
; R600: UINT_TO_FLT * T{{[0-9]+\.[XYZW], T[0-9]+\.[XYZW]}}
; R600: UINT_TO_FLT * T{{[0-9]+\.[XYZW], T[0-9]+\.[XYZW]}}
; R600: UINT_TO_FLT * T{{[0-9]+\.[XYZW], T[0-9]+\.[XYZW]}}
define amdgpu_kernel void @v_uint_to_fp_v4i32(ptr addrspace(1) %out, ptr addrspace(1) %in) #0 {
  %tid = call i32 @llvm.amdgcn.workitem.id.x()
  %in.gep = getelementptr <4 x i32>, ptr addrspace(1) %in, i32 %tid
  %out.gep = getelementptr <4 x float>, ptr addrspace(1) %out, i32 %tid
  %value = load <4 x i32>, ptr addrspace(1) %in.gep
  %result = uitofp <4 x i32> %value to <4 x float>
  store <4 x float> %result, ptr addrspace(1) %out.gep
  ret void
}

; FUNC-LABEL: {{^}}s_uint_to_fp_i1_to_f32:
; SI: s_cmp_eq_u32
; SI: s_cselect_b64  [[CMP:s\[[0-9]+:[0-9]\]]], -1, 0
; SI: v_cndmask_b32_e64 [[RESULT:v[0-9]+]], 0, 1.0, [[CMP]]
; SI: buffer_store_dword [[RESULT]],
; SI: s_endpgm
define amdgpu_kernel void @s_uint_to_fp_i1_to_f32(ptr addrspace(1) %out, i32 %in) #0 {
  %cmp = icmp eq i32 %in, 0
  %fp = uitofp i1 %cmp to float
  store float %fp, ptr addrspace(1) %out
  ret void
}

; FUNC-LABEL: {{^}}s_uint_to_fp_i1_to_f32_load:
; SI: v_cndmask_b32_e64 [[RESULT:v[0-9]+]], 0, 1.0
; SI: buffer_store_dword [[RESULT]],
; SI: s_endpgm
define amdgpu_kernel void @s_uint_to_fp_i1_to_f32_load(ptr addrspace(1) %out, i1 %in) #0 {
  %fp = uitofp i1 %in to float
  store float %fp, ptr addrspace(1) %out
  ret void
}

; FUNC-LABEL: {{^}}v_uint_to_fp_i1_f32_load:
; SI: {{buffer|flat}}_load_ubyte
; SI: v_and_b32_e32 {{v[0-9]+}}, 1, {{v[0-9]+}}
; SI: v_cmp_eq_u32
; SI: v_cndmask_b32_e64 [[RESULT:v[0-9]+]], 0, 1.0
; SI: {{buffer|flat}}_store_dword {{.*}}[[RESULT]]
; SI: s_endpgm
define amdgpu_kernel void @v_uint_to_fp_i1_f32_load(ptr addrspace(1) %out, ptr addrspace(1) %in) #0 {
  %tid = call i32 @llvm.amdgcn.workitem.id.x()
  %in.gep = getelementptr i1, ptr addrspace(1) %in, i32 %tid
  %out.gep = getelementptr float, ptr addrspace(1) %out, i32 %tid
  %val = load i1, ptr addrspace(1) %in.gep
  %fp = uitofp i1 %val to float
  store float %fp, ptr addrspace(1) %out.gep
  ret void
}

; FIXME: Repeated here to test r600
; FUNC-LABEL: {{^}}s_uint_to_fp_i64_to_f32:
; R600: FFBH_UINT
; R600: CNDE_INT
; R600: UINT_TO_FLT

define amdgpu_kernel void @s_uint_to_fp_i64_to_f32(ptr addrspace(1) %out, i64 %in) #0 {
entry:
  %cvt = uitofp i64 %in to float
  store float %cvt, ptr addrspace(1) %out
  ret void
}

declare i32 @llvm.amdgcn.workitem.id.x() #1

attributes #0 = { nounwind }
attributes #1 = { nounwind readnone }
