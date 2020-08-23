; ModuleID = 'tests/avail_expr-m2r.bc'
source_filename = "tests/avail_expr.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @main(i32, i8**) #0 {
  %3 = add nsw i32 %0, 50
  %4 = add nsw i32 %3, 96
  %5 = icmp slt i32 50, %3
  br i1 %5, label %6, label %9

6:                                                ; preds = %2
  %7 = sub nsw i32 %3, 50
  %8 = mul nsw i32 96, %3
  br label %12

9:                                                ; preds = %2
  %10 = add nsw i32 %3, 50
  %11 = mul nsw i32 96, %3
  br label %12

12:                                               ; preds = %9, %6
  %.0 = phi i32 [ %7, %6 ], [ %10, %9 ]
  %13 = sub nsw i32 50, 96
  %14 = add nsw i32 %13, %.0
  ret i32 0
}

attributes #0 = { noinline nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 8.0.1-7 (tags/RELEASE_801/final)"}
