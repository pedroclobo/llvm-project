; ModuleID = 'before-crash_20251018_230059.ll'
source_filename = "/tmp/simdjson-2.0.4/src/simdjson.cpp"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.lifetime.start.p0(ptr captures(none)) #0

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.lifetime.end.p0(ptr captures(none)) #0

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias writeonly captures(none), ptr noalias readonly captures(none), i64, i1 immarg) #1

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare ptr @llvm.invariant.start.p0(i64 immarg, ptr captures(none)) #0

; Function Attrs: mustprogress nounwind uwtable
define noundef zeroext i1 @_ZNK8simdjson7icelake14implementation13validate_utf8EPKcm() #2 {
entry:
  ret i1 false
}

; Function Attrs: mustprogress uwtable
define fastcc noundef zeroext i1 @_ZN8simdjson7icelake12_GLOBAL__N_16stage121generic_validate_utf8EPKcm() #3 {
entry:
  ret i1 false
}

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: write)
declare void @llvm.memset.p0.i64(ptr writeonly captures(none), i8, i64, i1 immarg) #4

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memmove.p0.p0.i64(ptr writeonly captures(none), ptr readonly captures(none), i64, i1 immarg) #1

; Function Attrs: cold noreturn nounwind memory(inaccessiblemem: write)
declare void @llvm.trap() #5

; Function Attrs: mustprogress uwtable
define fastcc noundef zeroext i1 @_ZN8simdjson7icelake12_GLOBAL__N_16stage121generic_validate_utf8INS1_15utf8_validation12utf8_checkerEEEbPKhm() #3 {
entry:
  br label %while.cond

while.cond:                                       ; preds = %while.cond, %entry
  br label %while.cond
}

; Function Attrs: alwaysinline mustprogress uwtable
declare fastcc void @_ZN8simdjson7icelake12_GLOBAL__N_14simd5simd8IhEC2Ev() #6

; Function Attrs: alwaysinline mustprogress nounwind uwtable
declare fastcc void @_ZN8simdjson7icelake12_GLOBAL__N_116buf_block_readerILm64EEC2EPKhm() #7

; Function Attrs: alwaysinline mustprogress nounwind uwtable
declare fastcc noundef zeroext i1 @_ZNK8simdjson7icelake12_GLOBAL__N_116buf_block_readerILm64EE14has_full_blockEv() #7

; Function Attrs: alwaysinline mustprogress nounwind uwtable
declare fastcc noundef ptr @_ZNK8simdjson7icelake12_GLOBAL__N_116buf_block_readerILm64EE10full_blockEv() #7

; Function Attrs: alwaysinline mustprogress uwtable
declare fastcc void @_ZN8simdjson7icelake12_GLOBAL__N_14simd8simd8x64IhEC2EPKh() #6

; Function Attrs: alwaysinline mustprogress uwtable
define fastcc void @_ZN8simdjson7icelake12_GLOBAL__N_115utf8_validation12utf8_checker16check_next_inputERKNS1_4simd8simd8x64IhEE() #6 {
entry:
  ret void
}

; Function Attrs: alwaysinline mustprogress uwtable
declare fastcc noundef zeroext i1 @_ZN8simdjson7icelake12_GLOBAL__N_18is_asciiERKNS1_4simd8simd8x64IhEE() #6

; Function Attrs: alwaysinline mustprogress uwtable
define fastcc void @_ZN8simdjson7icelake12_GLOBAL__N_115utf8_validation12utf8_checker16check_utf8_bytesENS1_4simd5simd8IhEES6_() #6 {
entry:
  ret void
}

; Function Attrs: alwaysinline mustprogress uwtable
declare fastcc void @_ZNK8simdjson7icelake12_GLOBAL__N_14simd4baseINS2_5simd8IhEEEorES5_() #8

; Function Attrs: alwaysinline mustprogress uwtable
declare fastcc void @_ZNK8simdjson7icelake12_GLOBAL__N_14simd5base8IhNS2_5simd8IbEEE4prevILi1EEENS4_IhEES8_() #8

; Function Attrs: alwaysinline mustprogress uwtable
declare fastcc void @_ZN8simdjson7icelake12_GLOBAL__N_115utf8_validation19check_special_casesENS1_4simd5simd8IhEES5_() #6

; Function Attrs: alwaysinline mustprogress uwtable
define fastcc void @_ZN8simdjson7icelake12_GLOBAL__N_115utf8_validation23check_multibyte_lengthsENS1_4simd5simd8IhEES5_S5_() #6 {
entry:
  ret void
}

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(none)
declare <8 x i64> @llvm.x86.avx512.vpermi2var.q.512(<8 x i64>, <8 x i64>, <8 x i64>) #9

; Function Attrs: alwaysinline mustprogress uwtable
declare fastcc void @_ZN8simdjson7icelake12_GLOBAL__N_14simd5simd8IhEC2Eh() #6

; Function Attrs: alwaysinline mustprogress nounwind uwtable
declare fastcc noundef nonnull align 64 dereferenceable(64) ptr @_ZN8simdjson7icelake12_GLOBAL__N_14simd4baseINS2_5simd8IhEEEcvRDv8_xEv() #7

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(none)
declare <64 x i8> @llvm.x86.avx512.pshuf.b.512(<64 x i8>, <64 x i8>) #9

; Function Attrs: alwaysinline mustprogress nounwind uwtable
declare fastcc noundef <8 x i64> @_ZL16_mm512_set1_epi8c() #10

; Function Attrs: alwaysinline mustprogress uwtable
declare fastcc void @_ZNK8simdjson7icelake12_GLOBAL__N_14simd5base8IhNS2_5simd8IbEEE4prevILi2EEENS4_IhEES8_() #8

; Function Attrs: alwaysinline mustprogress uwtable
declare fastcc void @_ZNK8simdjson7icelake12_GLOBAL__N_14simd5base8IhNS2_5simd8IbEEE4prevILi3EEENS4_IhEES8_() #8

; Function Attrs: alwaysinline mustprogress uwtable
define fastcc void @_ZN8simdjson7icelake12_GLOBAL__N_124must_be_2_3_continuationENS1_4simd5simd8IhEES4_() #6 {
entry:
  ret void
}

; Function Attrs: alwaysinline mustprogress uwtable
declare fastcc void @_ZNK8simdjson7icelake12_GLOBAL__N_14simd5simd8IhE14saturating_subES4_() #8

; Function Attrs: alwaysinline mustprogress uwtable
declare fastcc void @_ZN8simdjson7icelake12_GLOBAL__N_14simd5simd8IaEC2EDv8_x() #6

; Function Attrs: alwaysinline mustprogress uwtable
define fastcc void @_ZNK8simdjson7icelake12_GLOBAL__N_14simd5simd8IaEgtES4_() #8 {
entry:
  ret void
}

; Function Attrs: alwaysinline mustprogress uwtable
declare fastcc void @_ZN8simdjson7icelake12_GLOBAL__N_14simd5simd8IaEC2Ea() #6

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare <64 x i8> @llvm.usub.sat.v64i8(<64 x i8>, <64 x i8>) #11

; Function Attrs: alwaysinline mustprogress uwtable
define fastcc noundef <8 x i64> @_ZL21_mm512_maskz_abs_epi8yDv8_x(i64 noundef %__U) #12 {
entry:
  %call = call fastcc noundef <8 x i64> @_ZL15_mm512_abs_epi8Dv8_x()
  %0 = bitcast <8 x i64> %call to <64 x b8>
  %1 = bitcast i64 %__U to <64 x i1>
  %2 = select <64 x i1> %1, <64 x b8> %0, <64 x b8> zeroinitializer
  %3 = bitcast <64 x b8> %2 to <8 x b64>
  %4 = bytecast <8 x b64> %3 to <8 x i64>
  ret <8 x i64> %4
}

; Function Attrs: alwaysinline mustprogress nounwind uwtable
declare fastcc noundef nonnull align 64 dereferenceable(64) ptr @_ZNK8simdjson7icelake12_GLOBAL__N_14simd4baseINS2_5simd8IaEEEcvRKDv8_xEv() #7

; Function Attrs: alwaysinline mustprogress nounwind uwtable
declare fastcc noundef <8 x i64> @_ZL15_mm512_abs_epi8Dv8_x() #13

; Function Attrs: alwaysinline mustprogress nounwind uwtable
declare fastcc <8 x i64> @_ZL20_mm512_setzero_si512v() #10

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare <64 x i8> @llvm.abs.v64i8(<64 x i8>, i1 immarg) #11

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(none)
declare i32 @llvm.x86.avx2.pmovmskb(<32 x i8>) #9

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(none)
declare <16 x i16> @llvm.x86.avx2.psrli.w(<16 x i16>, i32) #9

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(none)
declare <32 x i8> @llvm.x86.avx2.pshuf.b(<32 x i8>, <32 x i8>) #9

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare <32 x i8> @llvm.usub.sat.v32i8(<32 x i8>, <32 x i8>) #11

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(none)
declare i32 @llvm.x86.avx.ptestz.256(<4 x i64>, <4 x i64>) #9

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(none)
declare i32 @llvm.x86.sse2.pmovmskb.128(<16 x i8>) #9

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(none)
declare <8 x i16> @llvm.x86.sse2.psrli.w(<8 x i16>, i32) #9

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(none)
declare <16 x i8> @llvm.x86.ssse3.pshuf.b.128(<16 x i8>, <16 x i8>) #9

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare <16 x i8> @llvm.usub.sat.v16i8(<16 x i8>, <16 x i8>) #11

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(none)
declare i32 @llvm.x86.sse41.ptestz(<2 x i64>, <2 x i64>) #9

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare i64 @llvm.ctlz.i64(i64, i1 immarg) #11

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare { i64, i1 } @llvm.uadd.with.overflow.i64(i64, i64) #11

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(none)
declare <2 x i64> @llvm.x86.pclmulqdq(<2 x i64>, <2 x i64>, i8 immarg) #9

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: write)
declare void @llvm.masked.compressstore.v64b8(<64 x b8>, ptr captures(none), <64 x i1>) #14

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare i64 @llvm.ctpop.i64(i64) #11

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare <64 x i8> @llvm.umax.v64i8(<64 x i8>, <64 x i8>) #11

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(none)
declare <64 x b8> @llvm.x86.avx512.mask.compress.v64b8(<64 x b8>, <64 x b8>, <64 x i1>) #9

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare i64 @llvm.cttz.i64(i64, i1 immarg) #11

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(none)
declare <8 x i16> @llvm.x86.ssse3.pmadd.ub.sw.128(<16 x i8>, <16 x i8>) #9

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(none)
declare <4 x i32> @llvm.x86.sse2.pmadd.wd(<8 x i16>, <8 x i16>) #9

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(none)
declare <8 x i16> @llvm.x86.sse41.packusdw(<4 x i32>, <4 x i32>) #9

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare <32 x i8> @llvm.umax.v32i8(<32 x i8>, <32 x i8>) #11

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare <16 x i8> @llvm.umax.v16i8(<16 x i8>, <16 x i8>) #11

attributes #0 = { nocallback nofree nosync nounwind willreturn memory(argmem: readwrite) }
attributes #1 = { nocallback nofree nounwind willreturn memory(argmem: readwrite) }
attributes #2 = { mustprogress nounwind uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+avx,+avx2,+avx512bw,+avx512cd,+avx512dq,+avx512f,+avx512vbmi,+avx512vbmi2,+avx512vl,+bmi,+cmov,+crc32,+cx8,+evex512,+f16c,+fma,+fxsr,+lzcnt,+mmx,+pclmul,+popcnt,+sse,+sse2,+sse3,+sse4.1,+sse4.2,+ssse3,+x87,+xsave" "tune-cpu"="generic" }
attributes #3 = { mustprogress uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+avx,+avx2,+avx512bw,+avx512cd,+avx512dq,+avx512f,+avx512vbmi,+avx512vbmi2,+avx512vl,+bmi,+cmov,+crc32,+cx8,+evex512,+f16c,+fma,+fxsr,+lzcnt,+mmx,+pclmul,+popcnt,+sse,+sse2,+sse3,+sse4.1,+sse4.2,+ssse3,+x87,+xsave" "tune-cpu"="generic" }
attributes #4 = { nocallback nofree nounwind willreturn memory(argmem: write) }
attributes #5 = { cold noreturn nounwind memory(inaccessiblemem: write) }
attributes #6 = { alwaysinline mustprogress uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+avx,+avx2,+avx512bw,+avx512cd,+avx512dq,+avx512f,+avx512vbmi,+avx512vbmi2,+avx512vl,+bmi,+cmov,+crc32,+cx8,+evex512,+f16c,+fma,+fxsr,+lzcnt,+mmx,+pclmul,+popcnt,+sse,+sse2,+sse3,+sse4.1,+sse4.2,+ssse3,+x87,+xsave" "tune-cpu"="generic" }
attributes #7 = { alwaysinline mustprogress nounwind uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+avx,+avx2,+avx512bw,+avx512cd,+avx512dq,+avx512f,+avx512vbmi,+avx512vbmi2,+avx512vl,+bmi,+cmov,+crc32,+cx8,+evex512,+f16c,+fma,+fxsr,+lzcnt,+mmx,+pclmul,+popcnt,+sse,+sse2,+sse3,+sse4.1,+sse4.2,+ssse3,+x87,+xsave" "tune-cpu"="generic" }
attributes #8 = { alwaysinline mustprogress uwtable "min-legal-vector-width"="512" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+avx,+avx2,+avx512bw,+avx512cd,+avx512dq,+avx512f,+avx512vbmi,+avx512vbmi2,+avx512vl,+bmi,+cmov,+crc32,+cx8,+evex512,+f16c,+fma,+fxsr,+lzcnt,+mmx,+pclmul,+popcnt,+sse,+sse2,+sse3,+sse4.1,+sse4.2,+ssse3,+x87,+xsave" "tune-cpu"="generic" }
attributes #9 = { nocallback nofree nosync nounwind willreturn memory(none) }
attributes #10 = { alwaysinline mustprogress nounwind uwtable "min-legal-vector-width"="512" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+avx,+avx2,+avx512f,+cmov,+crc32,+cx8,+evex512,+f16c,+fma,+fxsr,+mmx,+popcnt,+sse,+sse2,+sse3,+sse4.1,+sse4.2,+ssse3,+x87,+xsave" "tune-cpu"="generic" }
attributes #11 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }
attributes #12 = { alwaysinline mustprogress uwtable "min-legal-vector-width"="512" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+avx,+avx2,+avx512bw,+avx512f,+cmov,+crc32,+cx8,+evex512,+f16c,+fma,+fxsr,+mmx,+popcnt,+sse,+sse2,+sse3,+sse4.1,+sse4.2,+ssse3,+x87,+xsave" "tune-cpu"="generic" }
attributes #13 = { alwaysinline mustprogress nounwind uwtable "min-legal-vector-width"="512" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+avx,+avx2,+avx512bw,+avx512f,+cmov,+crc32,+cx8,+evex512,+f16c,+fma,+fxsr,+mmx,+popcnt,+sse,+sse2,+sse3,+sse4.1,+sse4.2,+ssse3,+x87,+xsave" "tune-cpu"="generic" }
attributes #14 = { nocallback nofree nosync nounwind willreturn memory(argmem: write) }
