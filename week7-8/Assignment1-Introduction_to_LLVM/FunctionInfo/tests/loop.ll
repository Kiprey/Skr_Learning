; ModuleID = 'tests/loop.bc'
source_filename = "tests/loop.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@g = common dso_local local_unnamed_addr global i32 0, align 4

; Function Attrs: norecurse nounwind uwtable
define dso_local i32 @g_incr(i32) local_unnamed_addr #0 {
  %2 = load i32, i32* @g, align 4, !tbaa !2
  %3 = add nsw i32 %2, %0
  store i32 %3, i32* @g, align 4, !tbaa !2
  ret i32 %3
}

; Function Attrs: norecurse nounwind uwtable
define dso_local i32 @loop(i32, i32, i32) local_unnamed_addr #0 {
  %4 = icmp sgt i32 %1, %0
  %5 = load i32, i32* @g, align 4, !tbaa !2
  br i1 %4, label %6, label %10

6:                                                ; preds = %3
  %7 = sub i32 %1, %0
  %8 = mul i32 %7, %2
  %9 = add i32 %5, %8
  store i32 %9, i32* @g, align 4, !tbaa !2
  br label %10

10:                                               ; preds = %6, %3
  %11 = phi i32 [ %9, %6 ], [ %5, %3 ]
  ret i32 %11
}

attributes #0 = { norecurse nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 8.0.1-7 (tags/RELEASE_801/final)"}
!2 = !{!3, !3, i64 0}
!3 = !{!"int", !4, i64 0}
!4 = !{!"omnipotent char", !5, i64 0}
!5 = !{!"Simple C/C++ TBAA"}
