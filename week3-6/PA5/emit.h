///////////////////////////////////////////////////////////////////////
//
//  Assembly Code Naming Conventions:
//
//     Dispatch table            <classname>_dispTab
//     Method entry point        <classname>.<method>
//     Class init code           <classname>_init
//     Abort method entry        <classname>.<method>.Abort
//     Prototype object          <classname>_protObj
//     Integer constant          int_const<Symbol>
//     String constant           str_const<Symbol>
//
///////////////////////////////////////////////////////////////////////

#include "stringtab.h"

#define MAXINT  100000000    
#define WORD_SIZE    4
#define LOG_WORD_SIZE 2     // for logical shifts

// Global names
#define CLASSNAMETAB         "class_nameTab"
#define CLASSOBJTAB          "class_objTab"
#define INTTAG               "_int_tag"
#define BOOLTAG              "_bool_tag"
#define STRINGTAG            "_string_tag"
#define HEAP_START           "heap_start"

// Naming conventions
#define DISPTAB_SUFFIX       "_dispTab"
#define METHOD_SEP           "."
#define CLASSINIT_SUFFIX     "_init"
#define PROTOBJ_SUFFIX       "_protObj"
#define OBJECTPROTOBJ        "Object"PROTOBJ_SUFFIX
#define INTCONST_PREFIX      "int_const"
#define STRCONST_PREFIX      "str_const"
#define BOOLCONST_PREFIX     "bool_const"


#define EMPTYSLOT            0
#define LABEL                ":\n"

#define STRINGNAME (char *) "String"
#define INTNAME    (char *) "Int"
#define BOOLNAME   (char *) "Bool"
#define MAINNAME   (char *) "Main"

//
// information about object headers
//
#define DEFAULT_OBJFIELDS 3
#define TAG_OFFSET 0
#define SIZE_OFFSET 1
#define DISPTABLE_OFFSET 2

#define STRING_SLOTS      1
#define INT_SLOTS         1
#define BOOL_SLOTS        1

#define GLOBAL        "\t.globl\t"
#define ALIGN         "\t.align\t2\n"
#define WORD          "\t.word\t"
/*
//
// register names
//
#define ZERO "$zero"		// Zero register 
#define ACC  "$a0"		// Accumulator 
#define A1   "$a1"		// For arguments to prim funcs 
#define SELF "$s0"		// Ptr to self (callee saves) 
#define T1   "$t1"		// Temporary 1 
#define T2   "$t2"		// Temporary 2 
#define T3   "$t3"		// Temporary 3 
#define SP   "$sp"		// Stack pointer 
#define FP   "$fp"		// Frame pointer 
#define RA   "$ra"		// Return address 
*/

//          /* ## */
typedef char* Register;
//
// register names
//
Register const ZERO = "$zero";		// Zero register 
Register const ACC  = "$a0";		// Accumulator 
Register const A1   = "$a1";		// For arguments to prim funcs 
Register const SELF = "$s0";		// Ptr to self (callee saves) 
Register const T1   = "$t1";		// Temporary 1 
Register const T2   = "$t2";		// Temporary 2 
Register const SP   = "$sp";		// Stack pointer 
Register const FP   = "$fp";		// Frame pointer 
Register const RA   = "$ra";		// Return address 

//
// General-purpose registers for use by the register allocator:
//
// $t0, $t1, $t2, $t3, $t4, $v0, $v1, $a0, $a1, $a2, $gp, $s7 are used by 
// Object.copy, _GenGC_Collect, etc.  So don't use them here. 
// And $s0 is spoken for.
//
// We are also limited by the GC's register mask
//
// These registers are all treated as callee-saves.

int const NUM_REGS = 6;
Register const regNames[NUM_REGS] = 
  {
    "$s1",
    "$s2",
    "$s3",
    "$s4",
    "$s5",
    "$s6"
  };

//returns true if name is one of the registers in regNames.
//If this returns false, the register might be e.g. ACC, which is frequently
//overwritten.
bool isAllocatedReg (Register name);
inline bool regEq(Register r1, Register r2)
  //Assume that r1 and r2 are two of the constants above (in regNames or the 
  //individual constants like ACC), so pointer equality is enough.
  { return r1 == r2; }


//
// Opcodes
//
#define JALR  "\tjalr\t"  
#define JAL   "\tjal\t"                 
#define RET   "\tjr\t$ra\t"

#define SW    "\tsw\t"
#define LW    "\tlw\t"
#define LI    "\tli\t"
#define LA    "\tla\t"

#define MOVE  "\tmove\t"
#define NEG   "\tneg\t"
#define ADD   "\tadd\t"
#define ADDI  "\taddi\t"
#define ADDU  "\taddu\t"
#define ADDIU "\taddiu\t"
#define DIV   "\tdiv\t"
#define MUL   "\tmul\t"
#define SUB   "\tsub\t"
#define SLL   "\tsll\t"
#define BEQZ  "\tbeqz\t"
#define BRANCH   "\tb\t"
#define BEQ      "\tbeq\t"
#define BNE      "\tbne\t"
#define BLEQ     "\tble\t"
#define BLT      "\tblt\t"
#define BGT      "\tbgt\t"


