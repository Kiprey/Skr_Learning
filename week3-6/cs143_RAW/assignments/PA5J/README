README file for Programming Assignment 5  (Java edition)
=======================================================

Your directory should now contain the following files:

 ASTConstants.java    -> [cool root]/src/PA5J/ASTConstants.java
 ASTLexer.java	      -> [cool root]/src/PA5J/ASTLexer.java
 ASTParser.java	      -> [cool root]/src/PA5J/ASTParser.java
 AbstractSymbol.java  -> [cool root]/src/PA5J/AbstractSymbol.java
 AbstractTable.java   -> [cool root]/src/PA5J/AbstractTable.java
 BoolConst.java
 Cgen.java	      -> [cool root]/src/PA5J/Cgen.java
 CgenClassTable.java
 CgenNode.java
 CgenSupport.java
 ClassTable.java      -> [cool root]/src/PA5J/ClassTable.java
 Flags.java	      -> [cool root]/src/PA5J/Flags.java
 IdSymbol.java	      -> [cool root]/src/PA5J/IdSymbol.java
 IdTable.java	      -> [cool root]/src/PA5J/IdTable.java
 IntSymbol.java
 IntTable.java	      -> [cool root]/src/PA5J/IntTable.java
 ListNode.java	      -> [cool root]/src/PA5J/ListNode.java
 Makefile
 README
 StringSymbol.java
 StringTable.java     -> [cool root]/src/PA5J/StringTable.java
 SymbolTable.java     -> [cool root]/src/PA5J/SymbolTable.java
 SymtabExample.java   -> [cool root]/src/PA5J/SymtabExample.java
 TokenConstants.java  -> [cool root]/src/PA5J/TokenConstants.java
 TreeConstants.java
 TreeNode.java        -> [cool root]/src/PA5J/TreeNode.java
 Utilities.java	      -> [cool root]/src/PA5J/Utilities.java
 cool-tree.aps	      -> [cool root]/src/PA5J/cool-tree.aps
 cool-tree.java
 example.cl

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

	CgenClassTable.java CgenNode.java are the skeleton files for
	the code generator. These skeletons are much larger than the
	ones for previous assignments.  The skeletons provides three
	components of the code generator:

		- functions to build the inheritance graph; (we supply
		  this in case you didn't get this working for PA4)

		- functions to emit global data and constants;
		 
	You should work to understand this code, and it will help you
        write the rest of the code generator.

        TreeNode.java and ListNode.java contain definitions used by
        the tree package. DO NOT MODIFY.

        cool-tree.java specifies and give an implementation of Cool
        ASTs (see the README for PA3 and the "Cool Tour").  In this
        assignment, you will need to complete the code generating
        methods for all Cool expression nodes.

	BoolConst.java, IntSymbol.java, and StringSymbol.java are used
        to manage constants in Cool programs.  You will need to
        complete the method for generating constant definitions in
        each of these classes.

	CgenSupport.java is general support code for the code
	generator.  You can add functions and constants as you see
	fit, but do not modify anything that's already there.  This
	class also serves as a placeholder for a bunch of constants as
	well as functions to emit spim instructions (we supply these
	to save you some tedious coding).

	TreeConstants.java defined some useful symbol constants.  You
	may add some of your own, if you wish.

	ASTLexer.java, ASTParser.java, and ASTConstants.java implement
	a lexer and a parser for reading text representation of ASTs
	from console in the format produced by the parser phase. DO
	NOT MODIFY.

	Cgen.java supplies the driver for the compiler. You should not
        modify this code.  The script mycoolc can pass any of the
        standard flags to the code generator; for this assignment, -c
        (code generator debug) may be useful as it sets a global
        variable cgen_debug to true (1).  If you want your code
        generator to print debug information when the option is set,
        write your debug code in the following format:

	      if (cgen_debug)
	      {
		...
	      }

	SymbolTable.java contains a symbol table implementation. Read
	the comments in the file and look at the example in
	SymtabExample.java.  You are not required to use this code,
	but you may find it useful. DO NOT MODIFY.

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

	% [cool root]/bin/spim -file file1.s  /* or the output filename you chose */

	If you change architectures you must issue

	% gmake clean

	when you switch from one type of machine to the other.  If at
	some point you get weird errors from the linker, you probably
	forgot this step.

	GOOD LUCK!

---8<------8<------8<------8<---cut here---8<------8<------8<------8<---

Write-up for PA5
----------------

