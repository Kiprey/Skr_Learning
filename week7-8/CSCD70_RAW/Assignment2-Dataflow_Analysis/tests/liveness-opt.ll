; ModuleID = 'tests/liveness-opt.bc'
source_filename = "tests/liveness.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @sum(i32, i32) #0 {
  br label %3

3:                                                ; preds = %7, %2
  %.01 = phi i32 [ 1, %2 ], [ %6, %7 ]
  %.0 = phi i32 [ %0, %2 ], [ %8, %7 ]
  %4 = icmp slt i32 %.0, %1
  br i1 %4, label %5, label %9

5:                                                ; preds = %3
  %6 = mul nsw i32 %.01, %.0
  br label %7

7:                                                ; preds = %5
  %8 = add nsw i32 %.0, 1
  br label %3

9:                                                ; preds = %3
  ret i32 %.01
}

; Function Attrs: noinline nounwind uwtable
define dso_local void @test(i32, i32) #0 {
  br label %3

3:                                                ; preds = %9, %2
  %.02 = phi i32 [ 1, %2 ], [ 2, %9 ]
  %.01 = phi i32 [ %1, %2 ], [ %8, %9 ]
  %.0 = phi i32 [ %0, %2 ], [ %.1, %9 ]
  %4 = icmp slt i32 %.0, %.01
  br i1 %4, label %5, label %7

5:                                                ; preds = %3
  %6 = add nsw i32 %.0, 1
  br label %7

7:                                                ; preds = %5, %3
  %.1 = phi i32 [ %6, %5 ], [ %.0, %3 ]
  %8 = sub nsw i32 %.01, 1
  br label %9

9:                                                ; preds = %7
  %10 = icmp sle i32 %8, 20
  br i1 %10, label %3, label %11

11:                                               ; preds = %9
  ret void
}

attributes #0 = { noinline nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 8.0.1-7 (tags/RELEASE_801/final)"}
