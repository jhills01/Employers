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
/* Copyright (C) 1991-2020 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <https://www.gnu.org/licenses/>.  */
/* This header is separate from features.h so that the compiler can
   include it implicitly at the start of every compilation.  It must
   not itself include <features.h> or any other header that includes
   <features.h> because the implicit include comes before any feature
   test macros that may be defined in a source file before it first
   explicitly includes a system header.  GCC knows the name of this
   header in order to preinclude it.  */
/* glibc's intent is to support the IEC 559 math functionality, real
   and complex.  If the GCC (4.9 and later) predefined macros
   specifying compiler intent are available, use them to determine
   whether the overall intent is to support these features; otherwise,
   presume an older compiler has intent to support these features and
   define these macros by default.  */
/* wchar_t uses Unicode 10.0.0.  Version 10.0 of the Unicode Standard is
   synchronized with ISO/IEC 10646:2017, fifth edition, plus
   the following additions from Amendment 1 to the fifth edition:
   - 56 emoji characters
   - 285 hentaigana
   - 3 additional Zanabazar Square characters */
/* 
 * bitAnd - x&y using only ~ and | 
 *   Example: bitAnd(6, 5) = 4
 *   Legal ops: ~ |
 *   Max ops: 8
 *   Rating: 1
 */
int bitAnd(int x, int y) {
    
    // return 2;
    
    //SOLUTION:
    // 1) The first step is to negate both x and y. 
    // 2) Next, we OR that result. 
    //    Since we negated both x and y previously, any resultant bit that is 0 means both corresponding bits in original x and y values was a 1.
    //    And for & operators, only 1b & 1b gives a 1 bit. The rest are 0s. 
    // 3) Thus, we simply negate result of the OR operator to get an & result.
    return ~((~x) | (~y));
}
/* 
 * bitNor - ~(x|y) using only ~ and & 
 *   Example: bitNor(0x6, 0x5) = 0xFFFFFFF8
 *   Legal ops: ~ &
 *   Max ops: 8
 *   Rating: 1
 */
int bitNor(int x, int y) {
    //return 2;

    //SOLUTION:
    // 1) Negate both x and y.
    // 2) Finally, we & the results of 1) together. 
    //    By &'ing the negated version of x and y, we ensure that the only bits to be 1 in the result are bits which were originally 0 in both x and y
    return ((~x) & (~y));
}
/* 
 * copyLSB - set all bits of result to least significant bit of x
 *   Example: copyLSB(5) = 0xFFFFFFFF, copyLSB(6) = 0x00000000
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int copyLSB(int x) {
    //return 2;

    //SOLUTION:
    // 1) "x & 1"  turns all bits in x to 0 unless the least significant bit of x is 1. In this case, only the least remains, all else are 0.
    // 2) Negating result from 1) will turn all bits to 1 except possibly the least significant bit
    // 3) If after 3), the least significant bit is 0, then adding 1 will turn all bits to 1.
    //    Otherwise, if all bits are 1, adding 1 will turn all bits in x to 0.
    return ~(x & 1) + 1;
}
/* 
 * evenBits - return word with all even-numbered bits set to 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Rating: 1
 */
int evenBits(void) {
    //return 2;

    //SOLUTION:
    // 1) Easy. A byte with even bits is represented in hex by 0x55. 
    // 2) Then, just shift 0x55 to the correct byte location in an integer and XOR with the others. 
    return ((0x55 << 24) ^ (0x55 << 16) ^ (0x55 << 8) ^ 0x55);
    //return ((0xaa << 24) ^ (0xaa << 16) ^ (0xaa << 8) ^ 0xaa);
}
/* 
 * logicalShift - shift x to the right by n, using a logical shift
 *   Can assume that 0 <= n <= 31
 *   Examples: logicalShift(0x87654321,4) = 0x08765432
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3 
 */
int logicalShift(int x, int n) {
    
    //return 2;

    //SOLUTION
    // 1) Isolate the most significant bit of x with "(x >> 31) << 31"
    // 2) Then, shift the result from 1) by "n - 1"
    // 3) finally, XOR result from 2) with x shifted by n to perform the unsigned right shift properly
    int y = (x >> 31) << 31;
    y = y >> n;
    y = y << 1;
    return (x >> n) ^ y;
}
/* 
 * bang - Compute !x without using !
 *   Examples: bang(3) = 0, bang(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4 
 */
int bang(int x) {
    
    //return 2;

    //SOLUTION:
    // 1) Perform arithmetic right shift ("x" is a signed type) for both "x", and negative x in 2's complement (i.e.,  ), 31 bits
    //    This will fill all 32 bits of y and z with the left-most bit value (0 or 1) of "x" and "~x + 1", respectively.
    // 2) For all integers besides 0, y and z will have opposite values 
    //    (i.e., either y will be all 0s and z all 1s, or vice versa)
    //    This means that "( y | z ) + 1" will always produce all 0s as bits, and thus return 0.
    //    Conversely, if x == 0, then y and z will all 0s regardless. This means "( y | z )" will always be all 0s, 
    //    ... and thus, "( y | z ) + 1" will be 000...0001, and thus always return 1. 
    int y = x >> 31; //
    int z = (~x + 1) >> 31; // "~x + 1" == negative "x" in 2's complement binary notation.
    //printf("y: 0x%08x\n", y);
    //printf("z: 0x%08x\n", z);
    return ( y | z ) + 1;
}
/* 
 * leastBitPos - return a mask that marks the position of the
 *               least significant 1 bit. If x == 0, return 0
 *   Example: leastBitPos(96) = 0x20
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2 
 */
int leastBitPos(int x) {
    //return 2;

    //SOLUTION:
    // 1) In 2's complement, "x" and "(~x + 1)" have the same lower order 0 bits until the first 1 bit is encountered. 
    //    At the first 1 bit, both "x" and "(~x + 1)" have 1 as the bit. however, as we continue past that bit towards
    //    ... higher order bits, all corresponding bits between "x" and "(~x + 1)" have opposite values.
    //    Thus, "x & (~x + 1)" returns bits where the least significant bit is 1, and all other bits are 0.
    return x & (~x + 1);
}
/* 
 * isNotEqual - return 0 if x == y, and 1 otherwise 
 *   Examples: isNotEqual(5,5) = 0, isNotEqual(4,5) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2
 */
int isNotEqual(int x, int y) {
    //return 2;

    //SOLUTION
    // 1) "x ^ y" == 0 if x == y, or "x ^ y" == "some non-zero integer" if x != y by the nature of XOR
    // 2) Thus, it's a simple matter of double negation to turn 0 back into 0 if x == y, 
    //    ... or turn "some non-zero integer" into 1 if x != y
    return !!(x ^ y);
}
/* 
 * negate - return -x 
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int negate(int x) {
    //return 2;

    //SOLUTION
    // 1) Easy. In 2's complement, "~x + 1" is the definition of -x.
    return (~x + 1);
}
/* 
 * isPositive - return 1 if x > 0, return 0 otherwise 
 *   Example: isPositive(-1) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Rating: 2
 */
int isPositive(int x) {
    //return 2;

    //SOLUTION
    // 1) "x >> 31" creates 32 0 bits if x is positive or 0, or 32 1 bits if x is negative
    // 2) "!x" returns 0 if x is non-zero, or 1 if x == 0
    // 3) Thus, if x == 0, then "!(x >> 31 | !x)" return 0
    //    if x < 0, then "!(x >> 31 | !x)" returns 0
    //    finally, if x > 0, then "!(x >> 31 | !x)" return 1
    return !(x >> 31 | !x);
}
/* 
 * isNonNegative - return 1 if x >= 0, return 0 otherwise 
 *   Example: isNonNegative(-1) = 0.  isNonNegative(0) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2
 */
int isNonNegative(int x) {
    //return 2;

    //SOLUTION
    // 1) x >= 0 values will have leading 0 bit. Thus, "x >> 31" creates 32 0 bits
    // 2) likewise, x < 0 values will have a leading 1 bit. Thus, "x >> 31" creates 32 1 bits
    // 3) thus, "!(x >> 31)" will return 1 if x >= 0, or 0 if x < 0.
    return !(x >> 31);
}
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
    //return 2;

    //SOLUTION
    // 1) 
    int low = 0x30;
    int high = 0x3a;
    return !((x + (~low + 1)) >> 31) & (x + (~high + 1)) >> 31;
}
/* 
 * addOK - Determine if can compute x+y without overflow
 *   Example: addOK(0x80000000,0x80000000) = 0,
 *            addOK(0x80000000,0x70000000) = 1, 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3
 */
int addOK(int x, int y) {
    //return 2;

    //SOLUTION
    // 1) In order to determine overflow, the most significant bit of x, y, and their sum needs to be determined
    // 2) In order to return 1, sumMSB != 1 when x and y == 0, OR sumMSB == 0 when x and y == 1. 
    // 3) Otherwise, 0 is returned. 
    // NOTE: MSB == most significant bit
    int xMSB = x >> 31;
    int yMSB = y >> 31;
    int sumMSB = (x + y) >> 31;
    return !!(~sumMSB | xMSB | yMSB) & (sumMSB | ~xMSB | ~yMSB);
}
/* 
 * absVal - absolute value of x
 *   Example: absVal(-1) = 1.
 *   You may assume -TMax <= x <= TMax
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 10
 *   Rating: 4
 */
int absVal(int x) {
    //return 2;

    //SOLUTION
    // 1) The sign bits of x need determined via arithematic right shift 
    // 2) We can then use these sign bits to & with both x and it's 2's complement (~x + 1)
    //    ... which returns x when x > 0, or the 2's complement of x when x < 0
    int signBits = x >> 31;
    return (~signBits & x) | (signBits & (~x + 1));
}
/* 
 * isNonZero - Check whether x is nonzero using
 *              the legal operators except !
 *   Examples: isNonZero(3) = 1, isNonZero(0) = 0
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 10
 *   Rating: 4 
 */
int isNonZero(int x) {
    //return 2;

    //SOLUTION
    // 1) If x is non-zero, then ((x | (~x + 1)) >> 31) will produce -1, or 32 1 bits. 
    //    When we & this with 1, we get 1
    // 2) If x is zero, then ((x | (~x + 1)) >> 31) will produce 0, or 32 0 bits. 
    //    When we & this with 1, we get 0
    return ((x | (~x + 1)) >> 31) & 1;
}
/* 
 * byteSwap - swaps the nth byte and the mth byte
 *  Examples: byteSwap(0x12345678, 1, 3) = 0x56341278
 *            byteSwap(0xDEADBEEF, 0, 2) = 0xDEEFBEAD
 *  You may assume that 0 <= n <= 3, 0 <= m <= 3
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 25
 *  Rating: 2
 */
int byteSwap(int x, int n, int m) {
    //return 2;

    //SOLUTION
    int y = 0;
    // 1) First, we convert n and m integer byte values to integer bit values by left shifting by 3
    //    ... which is the same as multiplying by 8
    n = n << 3; 
    m = m << 3; 
    // 2) Then we swap the nth and mth byte using the newly converted n and m
    y = 0xff & ((x >> n) ^ (x >> m)); 
    x = x ^ (y << n);
    x = x ^ (y << m);
    return x;
}
/* 
 * bitMask - Generate a mask consisting of all 1's 
 *   lowbit and highbit
 *   Examples: bitMask(5,3) = 0x38
 *   Assume 0 <= lowbit <= 31, and 0 <= highbit <= 31
 *   If lowbit > highbit, then mask should be all 0's
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int bitMask(int highbit, int lowbit) {
    //return 2;

    //SOLUTION
    // 1) The best approach is to create two sub-masks based on the highbit and lowbit values. 
    //    We can do this by left shifting allOnes by "highbits + 1" bits, and then left shifting allOnes by "lowbit" bits. 
    // 2) XOR then makes it easy to isolate the desired bit range, making them all 1s. 
    //    Note though that we must & the result with lowMask to accomodate when lowbit > highbit.
    int allOnes = ~0;
    int highMask = (allOnes << highbit) << 1;
    int lowMask = allOnes << lowbit;
    return (highMask ^ lowMask) & lowMask;
}
