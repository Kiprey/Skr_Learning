; ModuleID = 'tests/benchmark-opt.bc'
source_filename = "tests/benchmark.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @f() #0 {
  br label %1

1:                                                ; preds = %9, %0
  %.01 = phi i32 [ 1, %0 ], [ %.12, %9 ]
  %.0 = phi i32 [ 0, %0 ], [ %.1, %9 ]
  %2 = icmp slt i32 %.0, 100
  br i1 %2, label %3, label %10

3:                                                ; preds = %1
  %4 = icmp slt i32 %.01, 20
  br i1 %4, label %5, label %7

5:                                                ; preds = %3
  %6 = add nsw i32 %.0, 1
  br label %9

7:                                                ; preds = %3
  %8 = add nsw i32 %.0, 2
  br label %9

9:                                                ; preds = %7, %5
  %.12 = phi i32 [ 1, %5 ], [ %.0, %7 ]
  %.1 = phi i32 [ %6, %5 ], [ %8, %7 ]
  br label %1

10:                                               ; preds = %1
  ret i32 %.01
}

attributes #0 = { noinline nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 8.0.1-7 (tags/RELEASE_801/final)"}
