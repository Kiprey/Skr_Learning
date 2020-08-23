README file for Programming Assignment 5  (C++ edition)
=======================================================

Your directory should now contain the following files:

 Makefile
 README
 ast-lex.cc	      -> [cool root]/src/PA5/ast-lex.cc
 ast-parse.cc	      -> [cool root]/src/PA5/ast-parse.cc
 cgen-phase.cc	      -> [cool root]/src/PA5/cgen-phase.cc
 cgen.cc
 cgen.h
 cgen_supp.cc
 cool-tree.cc	      -> [cool root]/src/PA5/cool-tree.cc
 cool-tree.handcode.h
 dumptype.cc	      -> [cool root]/src/PA5/dumptype.cc
 emit.h
 example.cl
 handle_flags.cc      -> [cool root]/src/PA5/handle_flags.cc
 mycoolc	      -> [cool root]/src/PA5/mycoolc*
 stringtab.cc	      -> [cool root]/src/PA5/stringtab.cc
 tree.cc	      -> [cool root]/src/PA5/tree.cc
 utilities.cc	      -> [cool root]/src/PA5/utilities.cc
 *.d

The include (.h) files for this assignment can be found in 
[cool root]/include/PA5

	The Makefile contains targets for compiling and running your
	program, as well as handing it in. DO NOT MODIFY.

	The README contains this info. Part of the assignment is to
	fill the README with the write-up for your project. You should
	explain design decisions, explain why your code is correct,
	and why your test cases are adequate. It is part of the
	assignment to clearly and concisely explain things in text as
	well as to comment your code. Just edit this file.

	In example.cl you should write a correct Cool program which
	tests as many aspects of the code generator as possible. It
	should pass your code generator, and running spim on the
	generated output should run the program correctly.

	cgen.cc is the skeleton file for the code generator. This
	skeleton is much larger than the ones for previous
	assignments.  The skeleton provides three components of the
	code generator:

		- functions to build the inheritance graph; (we supply
		  this in case you didn't get this working for PA4)

		- functions to emit global data and constants;
		 
		- functions to emit spim instructions; (we supply
		  these last two to save you some tedious coding)

	You should work to understand this code, and it will help you
        write the rest of the code generator.

	cgen.h is the header file for the code generator. Feel free to
	add anything you need.

	cgen_supp.cc is general support code for the code generator.
	You can add functions as you see fit, but do not modify the 3
	functions:

		o byte_mode
		o ascii_mode
		o emit_string_constant

	emit.h defines a bunch of macros which should come in handy
	when you actually want to emit code. Feel free to change
	anything here to suit your needs.

        cgen-phase.cc supplies the driver for the compiler. You should
        not modify this code.  The script mycoolc can pass any of the
        standard flags to the code generator; for this assignment, -c
        (code generator debug) may be useful as it sets a global
        variable cgen_debug to true (1).  If you want your code
        generator to print debug information when the option is set,
        write your debug code in the following format:

	      if (cgen_debug)
	      {
		...
	      }

	symtab.h contains a symbol table implementation. You may
        modify this file if you'd like.  To do so, remove the link and
        copy `[course dir]/include/PA5/symtab.h' to your local
        directory.

	cool.h contains definitions that are used by almost all parts
	of the compiler. DO NOT MODIFY.

        tree.h contains the base class `tree_node' of the AST nodes.

        cool-tree.cc, cool-tree.h, and cool-tree.handcode.h contain
        the declarations and definitions of code generation AST nodes.
        You may add fields as you wish, but you shouldn't modify any
        of the methods already there except `code', which you may
        modify or remove as you wish.  Note that cool-tree.handcode.h
        is different from the same file in PA4---this file contains
        function definitions needed for the cgen.cc skeleton.  You may
        modify cool-tree.h or cool-tree.handcode.h, but don't modify
        cool-tree.cc.  Place all method definitions in cgen.cc

	stringtab.h contains functions to manipulate the string table.
	DO NOT MODIFY.

	dumptype.cc contains functions for printing out an abstract
	syntax tree.  DO NOT MODIFY.

        *.d dependency files, generated automatically by the makefile.

Instructions
------------

	To compile your compiler and code generator type:

	% gmake cgen

	To test your compiler, type:

	% ./mycoolc [-o output filename] <file1.cl> <file2.cl> ...

	This command parses all the cool files given on the command
	line, passes them through the semantic checker, and then hands
	the program AST to your code generator.

	To run your compiler on the file example.cl, type:

	% gmake dotest

	To run the produced code:

	% [cool root]/bin/spim -file file1.s
      /* or the output filename you chose */
	
	If you change architectures you must issue

	% gmake clean

	when you switch from one type of machine to the other.  If at
	some point you get weird errors from the linker, you probably
	forgot this step.

	GOOD LUCK!

---8<------8<------8<------8<---cut here---8<------8<------8<------8<---

Write-up for PA5
----------------

