README file for Programming Assignment 4 (Java edition)
======================================================

Your directory should now contain the following files:

 ASTConstants.java    -> [cool root]/src/PA4J/ASTConstants.java
 ASTLexer.java	      -> [cool root]/src/PA4J/ASTLexer.java
 ASTParser.java	      -> [cool root]/src/PA4J/ASTParser.java
 AbstractSymbol.java  -> [cool root]/src/PA4J/AbstractSymbol.java
 AbstractTable.java   -> [cool root]/src/PA4J/AbstractTable.java
 BoolConst.java       -> [cool root]/src/PA3J/BoolConst.java
 ClassTable.java
 Flags.java	      -> [cool root]/src/PA4J/Flags.java
 IdSymbol.java	      -> [cool root]/src/PA4J/IdSymbol.java
 IdTable.java	      -> [cool root]/src/PA4J/IdTable.java
 IntSymbol.java	      -> [cool root]/src/PA4J/IntSymbol.java
 IntTable.java	      -> [cool root]/src/PA4J/IntTable.java
 ListNode.java	      -> [cool root]/src/PA4J/ListNode.java
 Makefile
 README
 Semant.java	      -> [cool root]/src/PA4J/Semant.java
 StringSymbol.java    -> [cool root]/src/PA4J/StringSymbol.java
 StringTable.java     -> [cool root]/src/PA4J/StringTable.java
 SymbolTable.java     -> [cool root]/src/PA4J/SymbolTable.java
 SymtabExample.java   -> [cool root]/src/PA4J/SymtabExample.java
 TokenConstants.java  -> [cool root]/src/PA4J/TokenConstants.java
 TreeConstants.java
 TreeNode.java	      -> [cool root]/src/PA4J/TreeNode.java
 Utilities.java	      -> [cool root]/src/PA4J/Utilities.java
 bad.cl
 cool-tree.aps	      -> [cool root]/src/PA4J/cool-tree.aps
 cool-tree.java
 good.cl

	The Makefile contains targets for compiling and running your
	program. DO NOT MODIFY.

	The README contains this info. Part of the assignment is to fill
	the README with the write-up for your project. You should
	explain design decisions, explain why your code is correct, and
	why your test cases are adequate. It is part of the assignment
	to clearly and concisely explain things in text as well as to
	comment your code.  Just edit this file.

	good.cl and bad.cl test a few features of the semantic checker.
	You should add tests to ensure that good.cl exercises as many
	legal semantic combinations as possible and that bad.cl
	exercises as many kinds of semantic errors as possible.

	cool-tree.aps contains the definitions for the tree language
	which you use to construct the abstract syntax tree (AST). This
	file is provided for your reference.  DO NOT MODIFY.

        TreeNode.java and ListNode.java contain definitions used by the
        tree package. DO NOT MODIFY.  

        cool-tree.java specifies and gives an implementation of Cool ASTs
        (see the README for PA3 and the "Cool Tour").  In this
        assignment, you will need to add functions to the AST classes to
        store, fetch, and compute information about the AST.

   	You should NOT remove any definitions that are already present
   	in cool-tree.java.  These function and data members are required
   	for the system to function properly.

        You should add any fields and methods to the classes you need to
        perform semantic analysis.  You will need to add, for example,
        methods which traverse the expressions of the tree and implement
        the type-checking rules.  The entry point to the semantic
        analyzer is program.semant().

	ClassTable.java implements a placeholder class for a few useful
	methods.  Feel free to extend it to implement some real data
	strucutre.

	TreeConstants.java defined some useful symbol constants.  You
	may add some of your own, if you wish.

	ASTLexer.java, ASTParser.java, and ASTConstants.java implement a
	lexer and a parser for reading text representation of ASTs from
	console in the format produced by the parser phase. DO NOT
	MODIFY.

        Semant.java contains a driver to test the analyzer.  The main
        method reads an AST in text form from standard input, parses it,
        and then produces a type-annotated AST on standard output.  The
        script mycoolc can pass any of the standard flags to the
        semantic analyzer as well; for this assignment, -s (semantic
        analysis debug) may be useful as it sets a static variable
        Flags.semant_debug to "true".  If you want your semantic checker
        to print debug information when the option is set, write your
        debug code in the following format:

	      if (Flags.semant_debug)
	      {
		...
	      }

	semant_debug is provided as a convenience. You don't need to use
	the debugging flags if you don't want to. DON'T MODIFY
	Semant.java

	SymbolTable.java contains a symbol table implementation. Read
	the comments in the file and look at the example in
	SymtabExample.java.  You are not required to use this code, but
	you may find it useful. DO NOT MODIFY.

Instructions
------------

	To compile the example use of the symbol table, type

	% gmake symtab-example

	This creates a shell script to run the symbol table example.

	To compile your semantic analyzer program type:

	% gmake semant

	To test your semantic checker, type:

	% mysemant foo.cl

	mysemant is a version of mycoolc that omits code generation.
	mysemant parses all the cool files given on the command line and
	builds a single abstract syntax tree containing all class
	definitions appearing in the input files. Your semantic checker
	is then called on this abstract syntax tree.  If there are no
	errors, the program produces a type-annotated abstract syntax
	tree as output.

	To run your checker on the files good.cl and bad.cl type:

	% gmake dotest

	If you think your semantic checker is correct and behaves like
	the one we wrote, you can try to run mycoolc using your checker,
	your parser and also your lexical analyzer if you choose (see
	below for instructions).  Remember if your lexer, parser or
	checker behaves in an unexpected manner, you may get errors
	anywhere.

	If you change architectures you must issue

	% gmake clean

	when you switch from one type of machine to the other.
	If at some point you get weird errors from the linker,	
	you probably forgot this step.

	GOOD LUCK!

---8<------8<------8<------8<---cut here---8<------8<------8<------8<---

Write-up for PA4
----------------
