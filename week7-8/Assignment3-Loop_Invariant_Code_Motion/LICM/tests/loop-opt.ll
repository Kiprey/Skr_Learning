; ModuleID = 'tests/loop-opt.bc'
source_filename = "tests/loop.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @loop(i32, i32, i32) #0 {
  %4 = icmp ne i32 0, 0
  %5 = add nsw i32 %1, 2
  br label %6

6:                                                ; preds = %19, %3
  %.01 = phi i32 [ %2, %3 ], [ %5, %19 ]
  %.0 = phi i32 [ 0, %3 ], [ %18, %19 ]
  br label %7

7:                                                ; preds = %14, %6
  %.1 = phi i32 [ %.0, %6 ], [ %.2, %14 ]
  %8 = icmp ne i32 %.1, 0
  br i1 %8, label %9, label %15

9:                                                ; preds = %7
  %10 = add nsw i32 %0, %.01
  %11 = icmp eq i32 %.1, 2
  br i1 %11, label %12, label %13

12:                                               ; preds = %9
  br label %14

13:                                               ; preds = %9
  br label %14

14:                                               ; preds = %13, %12
  %.2 = phi i32 [ 5, %12 ], [ 3, %13 ]
  br label %7

15:                                               ; preds = %7
  br i1 %4, label %16, label %17

16:                                               ; preds = %15
  br label %17

17:                                               ; preds = %16, %15
  %.3 = phi i32 [ 3, %16 ], [ %.1, %15 ]
  %18 = add nsw i32 %.3, 1
  br label %19

19:                                               ; preds = %17
  %20 = icmp slt i32 %18, %0
  br i1 %20, label %6, label %21

21:                                               ; preds = %19
  %22 = add nsw i32 0, %5
  ret i32 %22
}

attributes #0 = { noinline nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 8.0.1-7 (tags/RELEASE_801/final)"}
