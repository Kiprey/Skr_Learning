#! /bin/sh
#   1. make用于生成测试用例与seq模拟器，-s 安静模式
#   2. correctness.pl使用更高级别的测试来测试ncopy.ys的正确性
#   3. ./benchmark.pl为运行速度评分
#   4. 最后判断ncopy的y86汇编文件大小是否不超过1k bytes（超过1k字节则ncopy.ys不合格）
make VERSION=full -s && ./correctness.pl && ./benchmark.pl && ../misc/yas ncopy.ys && ./check-len.pl < ncopy.yo
