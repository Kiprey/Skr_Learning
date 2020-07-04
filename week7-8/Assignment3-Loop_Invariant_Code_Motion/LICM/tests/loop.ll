; ModuleID = 'tests/loop-preproc.bc'
source_filename = "tests/loop.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @loop(i32, i32, i32) #0 {
  br label %4

4:                                                ; preds = %19, %3
  %.01 = phi i32 [ %2, %3 ], [ %17, %19 ]
  %.0 = phi i32 [ 0, %3 ], [ %18, %19 ]
  br label %5

5:                                                ; preds = %12, %4
  %.1 = phi i32 [ %.0, %4 ], [ %.2, %12 ]
  %6 = icmp ne i32 %.1, 0
  br i1 %6, label %7, label %13

7:                                                ; preds = %5
  %8 = add nsw i32 %0, %.01
  %9 = icmp eq i32 %.1, 2
  br i1 %9, label %10, label %11

10:                                               ; preds = %7
  br label %12

11:                                               ; preds = %7
  br label %12

12:                                               ; preds = %11, %10
  %.2 = phi i32 [ 5, %10 ], [ 3, %11 ]
  br label %5

13:                                               ; preds = %5
  %14 = icmp ne i32 0, 0
  br i1 %14, label %15, label %16

15:                                               ; preds = %13
  br label %16

16:                                               ; preds = %15, %13
  %.3 = phi i32 [ 3, %15 ], [ %.1, %13 ]
  %17 = add nsw i32 %1, 2
  %18 = add nsw i32 %.3, 1
  br label %19

19:                                               ; preds = %16
  %20 = icmp slt i32 %18, %0
  br i1 %20, label %4, label %21

21:                                               ; preds = %19
  %22 = add nsw i32 0, %17
  ret i32 %22
}

attributes #0 = { noinline nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 8.0.1-7 (tags/RELEASE_801/final)"}
