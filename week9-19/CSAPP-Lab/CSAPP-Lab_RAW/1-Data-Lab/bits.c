/* 
 * CS:APP Data Lab 
 * 
 * <Please put your name and userid here>
 * 
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting if the shift amount
     is less than 0 or greater than 31.


EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implement floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants. You can use any arithmetic,
logical, or comparison operations on int or unsigned data.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operations (integer, logical,
     or comparison) that you are allowed to use for your implementation
     of the function.  The max operator count is checked by dlc.
     Note that assignment ('=') is not counted; you may use as many of
     these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */


#endif
//1
/* 
 * bitXor - x^y using only ~ and & 
 *   Example: bitXor(4, 5) = 1
 *   Legal ops: ~ &
 *   Max ops: 14
 *   Rating: 1
 */
int bitXor(int x, int y) {
  // 实现位的异或运算
  // 0^0 = 1^1 = 0  0^1 = 1^0 = 1
  // a^b = (a & ~b) | (~a & b)
  // a|b = ~((~a)&(~b))
  int t1 = x & (~y);
  int t2 = (~x) & y;
  int result = (~t1)&(~t2);
  return ~result;
}
/* 
 * tmin - return minimum two's complement integer 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmin(void) {
  // 返回2的补码中最小整数
  // 0x80000000
  return 1 << 31;

}
//2
/*
 * isTmax - returns 1 if x is the maximum, two's complement number,
 *     and 0 otherwise 
 *   Legal ops: ! ~ & ^ | +
 *   Max ops: 10
 *   Rating: 1
 */
int isTmax(int x) {
  // 如果x是2的补码中的最大整数，则返回1,否则返回0
  // 补码中的最大整数为0x7fffffff

  // 如果x == 0x7fffffff,则x+1 == 0x80000000
  // 此时(x+1)+(x+1) == 0
  // 需要注意的是，当(x+1) == 0 或者 (x+1) == 0x80000000，(x+1)+(x+1)都等于0
  // 所以要将x+1 == 0这种情况排除, 使其返回0
  // ! 将0转为1或1转为0, !!将非零数转为1
  return !(x + 1 + x + 1) & (!!(x + 1));
}
/* 
 * allOddBits - return 1 if all odd-numbered bits in word set to 1
 *   where bits are numbered from 0 (least significant) to 31 (most significant)
 *   Examples allOddBits(0xFFFFFFFD) = 0, allOddBits(0xAAAAAAAA) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int allOddBits(int x) {
  // 如果单词中所有奇数位都设置为1，则返回1。其中位的编号从0（最低有效）到31（最高有效）
  // 当 x | 0x55555555 == 0xffffffff时则返回1。(0x55 == 0b 0101 0101)
  // 0x55555555 == (0x55 << 0) | (0x55 << 8) | (0x55 << 16) | (0x55 << 24)
  // ~0xffffffff == 0, !0 == 1
  int a = 0x55;
  int b = (a << 0) | (a << 8) | (a << 16) | (a << 24);
  return !~(x | b);
}
/* 
 * negate - return -x 
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int negate(int x) {
  // 取反加一即得负数
  return ~x + 1;
}
//3
/* 
 * isAsciiDigit - return 1 if 0x30 <= x <= 0x39 (ASCII codes for characters '0' to '9')
 *   Example: isAsciiDigit(0x35) = 1.
 *            isAsciiDigit(0x3a) = 0.
 *            isAsciiDigit(0x05) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 3
 */
int isAsciiDigit(int x) {
  // 用减法，判断 x-0x30 >= 0 && 0x39 - x >= 0
  int cond1 = x + (~0x30 + 1); // x - 0x30
  int cond2 = 0x39 + (~x + 1); // 0x39 - x
  // 最后判断两个中间值的符号位全为0即可返回1
  return !(cond1 >> 31 | cond2 >> 31);
}
/* 
 * conditional - same as x ? y : z 
 *   Example: conditional(2,4,5) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int conditional(int x, int y, int z) {
  // y与z之间肯定用 或运算，因为只要一边的值为0,其结果就是另外一边的值
  // 而在x与y, x与z之间肯定用 与运算
  // 如果运算使得
  //    1.x == 0时返回0; 否则返回0xffffffff
  //    2.x == 0时返回0xffffffff; 否则返回0
  // 我们可以利用 0 - 1 = 0xffffffff, 1 - 1 = 0这条特性来返回正确的值 
  int cond1 = (!x + (~1 + 1)) & y;
  int cond2 = (!!x + (~1 + 1)) & z;

  return cond1 | cond2;
}
/* 
 * isLessOrEqual - if x <= y  then return 1, else return 0 
 *   Example: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLessOrEqual(int x, int y) {
  // y-x >= 0 则return 1
  // 注意: 当x y异号时，直接使用y-x可能会造成溢出
  int ySign = (y >> 31) & 1;
  int isSignDiff = ((x >> 31) ^ ySign) & 1;
  int sumSign = ((y + (~x + 1)) >> 31) & 1;
  // 当isSignDiff == 0时，取 !sumSign   ==> (isSignDiff - 1) & !sumSign
  // 当isSignDiff == 1时, 取 !ySign     ==> -isSignDiff & !ySign
  int result = ((isSignDiff + (~1 + 1)) & !sumSign) | ((~isSignDiff + 1) & !ySign);
  return result;
}
//4
/* 
 * logicalNeg - implement the ! operator, using all of 
 *              the legal operators except !
 *   Examples: logicalNeg(3) = 0, logicalNeg(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4 
 */
int logicalNeg(int x) {
  // 如果-x的符号位与x的符号位不相同，则x != 0，返回0
  //    int isSignDiff = (((~x + 1) >> 31) ^ (x >> 31)) & 1;
  // 需要注意的是，0x80000000的负数与正数都为0xFFFFFFFF。所以需要额外处理，返回0
  // 因为int范围为（-2147483648 ~ 2147483647）注意-2147483648的相反数会上溢
  // 所以， 要在原来的条件上再添加一个条件： 
  //        如果-x的符号位与x的符号位不相同，并且 *不等于1* ，则x != 0，返回0
  // 符号值为01,10,11均返回0, 否则返回1。这个条件可以使用 或运算
  int isReturnZero = (((~x + 1) >> 31) | (x >> 31)) & 1;
  return (~isReturnZero) & 1;
}
/* howManyBits - return the minimum number of bits required to represent x in
 *             two's complement
 *  Examples: howManyBits(12) = 5           0......0    01100
 *            howManyBits(298) = 10         0.0    0100101010
 *            howManyBits(-5) = 4           1.......1    1011
 *            howManyBits(0)  = 1           0..........0    0
 *            howManyBits(-1) = 1           1..........1    1
 *            howManyBits(0x80000000) = 32      10.........00
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 90
 *  Rating: 4
 */
int howManyBits(int x) {
  /*
    判断传入的数需要多少个二进制来记录
        正数则查找从左边第一个1开始，一直到最右边那一位，的位数，再加上一个符号位
        负数则查找从左边第一个0开始，一直到最右边那一位，的位数，再加上一个符号位
  */

  int bit16, bit8, bit4, bit2, bit1;
  // 注意,如果x为负，则x >> 31为0xffffffff
  int sign = x >> 31;
  // 因为正数记录从左到右第一个1,负数记录第一个0
  // 所以可以对操作数取反，将负数转为正数，正数不变，便于更好的计算
  x ^= sign;
  // 二分查找，先判断高16位有无存在1,并将范围缩小到高16位或低16位
  // 如果高16位存在0,则bit16 == 16,否则等于0
  bit16 = (!!(x >> 16)) << 4;
  x >>= bit16;
  // 查找剩余16位中的高8位是否存在1
  bit8 = (!!(x >> 8)) << 3;
  x >>= bit8;
  // 查找剩余8位中的高4位是否存在1
  bit4 = (!!(x >> 4)) << 2;
  x >>= bit4;
  // 查找剩余4位中的高2位是否存在1
  bit2 = (!!(x >> 2)) << 1;
  x >>= bit2;
  // 查找剩余2位中的高1位是否存在1
  bit1 = (!!(x >> 1)) << 0;
  x >>= bit1;
    // 注意return中要再加上一个符号位
  return bit16 + bit8 + bit4 + bit2 + bit1 + x + 1;
}
//float
/* 
 * floatScale2 - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned floatScale2(unsigned uf) {
  // 求2乘以传入的参数uf
  // 首先获取其exp部分的值
  int exp = (uf >> 23) & 0xff;
  int sign = uf & (1 << 31);
  // 如果传入的是非规格化的值
  if(exp == 0)
    return sign | uf << 1;
  // 如果传入的是无穷大或NaN
  else if(exp == 0xff)
    // 没办法继续乘了，只能返回参数
    return uf;
  exp++;
  // 如果乘2后的结果超出范围（即溢出），则返回无穷大
  if(exp == 0xff)
    return sign | 0x7f800000; // expr全为0, frac全为1
  return sign | (exp << 23) | (uf & 0x7fffff);
}
/* 
 * floatFloat2Int - Return bit-level equivalent of expression (int) f
 *   for floating point argument f.
 *   Argument is passed as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point value.
 *   Anything out of range (including NaN and infinity) should return
 *   0x80000000u.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
int floatFloat2Int(unsigned uf) {
  int exp = (uf >> 23) & 0xff;
  int sign = (uf >> 31) & 1;
  int frac = uf & 0x7fffff;
  int shiftBits = 0;
  // 0比较特殊，先判断0(正负0都算作0)
  if(!(uf & 0x7fffffff))
    return 0;
  // 判断是否为NaN还是无穷大
  if(exp == 0xff)
    return 0x80000000u;
  // 指数减去偏移量，获取到真正的指数
  exp -= 127;
  // 需要注意的是，原来的frac一旦向左移位，其值就一定会小于1，所以返回0
  if(exp < 0)
    return 0;
  // 获取M，注意exp等于127和不等于127的情况是不一样的。当exp != 127时还有一个隐藏的1
  if(exp != 127)
    frac |= (1 << 23);
  // 要移位的位数。注意float的小数点是点在第23位与第22位之间
  shiftBits = 23 - exp;
  // 需要注意一点，如果指数过大，则也返回0x80000000u
  if(shiftBits < 31 - 23)
    return 0x80000000u;
  // 获取真正的结果
  frac >>= shiftBits;
  // 判断符号
  if(sign == 1)
    return ~frac + 1;
  else
    return frac;
}
/* 
 * floatPower2 - Return bit-level equivalent of the expression 2.0^x
 *   (2.0 raised to the power x) for any 32-bit integer x.
 *
 *   The unsigned value that is returned should have the identical bit
 *   representation as the single-precision floating-point number 2.0^x.
 *   If the result is too small to be represented as a denorm, return
 *   0. If too large, return +INF.
 * 
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. Also if, while 
 *   Max ops: 30 
 *   Rating: 4
 */
unsigned floatPower2(int x) {
    // 判断指数是否上溢或者下溢
    int exp = x + 127;
    if(exp > 0xfe)
        return 0x7f800000;
    else if(exp < 0)
        return 0;
    return exp << 23;
}
