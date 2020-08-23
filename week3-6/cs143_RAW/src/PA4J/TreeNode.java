/*
Copyright (c) 2000 The Regents of the University of California.
All rights reserved.

Permission to use, copy, modify, and distribute this software for any
purpose, without fee, and without written agreement is hereby granted,
provided that the above copyright notice and the following two
paragraphs appear in all copies of this software.

IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY PARTY FOR
DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT
OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE UNIVERSITY OF
CALIFORNIA HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

THE UNIVERSITY OF CALIFORNIA SPECIFICALLY DISCLAIMS ANY WARRANTIES,
INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
ON AN "AS IS" BASIS, AND THE UNIVERSITY OF CALIFORNIA HAS NO OBLIGATION TO
PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
*/

import java.io.PrintStream;

/** The base class for all AST nodes. 

    After lexical analysis and parsing, a Cool program is represented
    internally by the Cool compiler as an abstract syntax tree.  The
    project comes with a definition of Cool abstract syntax trees (ASTs)
    built in.  The AST package is by far the largest piece of code in
    the base system and requires the most time to learn.  The learning
    process is made more complex because the AST code is generated
    automatically from a specification in the file
    <code>cool-tree.aps</code>.  While the generated code is quite simple
    and regular in structure, it is also devoid of comments.  This
    section serves as the documentation for the AST package.

    <h3>Phyla and Constructors</h3>

    The AST data type provides a class for representing each type of
    Cool expression. There is a class for <code>let</code> expressions,
    another class of <code>+</code> expressions, and so on.  Objects of
    these classes are nodes in Cool abstract syntax trees.  For example,
    an expression <code>e1 + e2</code> is represented by a
    <code>+</code> expression object, which has two subtrees: one for
    the tree representing the expression <code>e1</code> and one for
    the tree representing the expression <code>e2</code>.

    <p>

    The Cool abstract syntax is specified in a language called APS.  In
    APS terminology, the various kinds of abstract syntax tree nodes
    (<code>let</code>, <code>+</code>, etc.) are called
    <em>constructors</em>.  (Don't confuse this use of the term
    "constructor" with Java constructors; while similar, this is a
    slightly different meaning taken from functional languages that
    predates Java.)  The form of the AST is described by a set of
    <em>phyla</em>.  Each phylum has one or more constructors.

    <p>

    Phyla are really just types.  That is, instead of having one large
    group of undifferentiated constructors, the constructors are grouped
    together according to function, so that, for example, the
    constructors for expression ASTs are distinguished from the
    constructors for class ASTs.  The phyla are defined at the beginning
    of <code>cool-tree.aps</code>:

<pre>
  module COOL begin phylum
  Program;

  phylum Class_;
  phylum Classes = LIST[Class_];

  phylum Feature;
  phylum Features = LIST[Feature];

  phylum Formal;
  phylum Formals = LIST[Formal];

  phylum Expression;
  phylum Expressions = LIST[Expression];

  phylum Case;
  phylum Cases = LIST[Case];
</pre>

    From the definition it can be seen that there are two distinct kinds
    of phyla: "normal" phyla and list phyla.  "Normal" phyla each
    have associated constructors; list phyla have a fixed set of list
    operations.

    <p>

    Each constructor takes typed arguments and returns a typed result.
    The types may either be phyla or any ordinary Java type.  In fact,
    the phyla declarations are themselves compiled into Java class
    declarations by an APS compiler.  A sample constructor definition is
  
<pre>
  constructor class_(name : AbstractSymbol; 
                     parent : AbstractSymbol;
                     features : Features; 
                     filename : AbstractSymbol) : Class_;
</pre>

    This declaration specifies that the <code>class_</code> constructor
    takes four arguments: an <code>AbstractSymbol</code> (a type
    identifier) for the class name, an <code>AbstractSymbol</code>
    (another type identifier) for the parent class, a
    <code>Features</code>, and an <code>AbstractSymbol</code> for the
    filename in which the class definition occurs.  (the name
    <code>class_</code> is chosen to avoid a conflict with the Java
    keyword <b>class</b>.)  The phylum <code>Features</code> is defined
    to be a list of <code>Feature</code>'s by the declaration 

<pre>
    phylum Features = LIST[Feature];
</pre>

    See <a href="ListNode.html">ListNode</a> for a description of the
    operations defined on AST lists.

    <p>

    To invoke the class constructor, you allocate a new node object
    supplying it with the right arguments, e.g. <code>new
    class_(...)</code>.  In <code>cool.cup</code> there is the following
    example of a use of the <code>class_</code> constructor:

<pre>
class	::= CLASS TYPEID:n INHERITS TYPEID:p LBRACE optional_feature_list:f RBRACE SEMI
        {: RESULT = new class_(curr_lineno(), n, p, f, curr_filename()); :}
</pre>

    Allocating a new <code>class_</code> object, builds a
    <codeClass_</code> tree node with the four arguments as children.
    Because the phyla (types) of the arguments are declared, the Java
    type checker enforces that the <code>class_</code> constructor is
    applied only to arguments of the appropriate type.  See Section 6.5
    of the "Tour of Cool Support Code" and <code>cool-tree.aps</code> to
    learn the definitions of the other constructors. (Comments
    in <code>cool-tree.aps</code> begin with two hyphens "--".)

    <p>

    NOTE: there is a real danger of getting confused because the same
    names are used repeatedly for different entities in different
    contexts.  In the example just above, small variations of the name
    <em>class</em> are used for a terminal (<code>CLASS</code>), a
    non-terminal (<code>class</code>), a constructor
    (<code>class_</code>), and a phylum (<code>Class_</code>).  These
    uses are all distinct and mean different things. Most uses are
    distinguished consistently by capitalization, but a few are not.
    When reading the code it is important to keep in mind the role of
    each symbol.

    <h3>The AST Class Hierarchy</h3>

    All AST classes are derived from this class
    (<em>TreeNode</em>). (The list classes are actually derived from <a
    href="ListNode.html">ListNode</a>, which is a refinement of
    <em>TreeNode</em>.)  All of the lists are lists of
    <em>TreeNode</em>s.

    <p>

    The <em>TreeNode</em> class definition contains everything needed in
    an abstract syntax tree node except information specific to
    particular constructors.  

    <p>

    Each of the constructors is a class derived from the appropriate
    phyla.  

    <h3>Class Members</h3>

    Each class definition of the tree package comes with a number of
    members.  Some of the member functions are discussed below.  This
    section describes the data members and some more (but not all) of
    the rest of the functions, as well as how to add new members to the
    classes.

    Each constructor has data members defined for each component of that
    constructor. The name of the member is the name of the field in the
    constructor, and it is only visible to member functions of the
    constructor's class or derived classes.  For example, the
    <code>class_</code> constructor has four data members:

<pre>
Symbol name;
Symbol parent;
Features features;
Symbol filename;
</pre>

Here is a complete use of one member:

<pre>
 class class_ extends Class_ {
     ...
     AbstractSymbol getParent() { return parent; }
     ...
 }

 ...

 Class_ c;
 AbstractSymbol p;

 c = new class(lineno, AbstractTable.idtable.add_string("Foo",3),
                       AbstractTable.idtable.add_string("Bar"), 
		       new Features(lineno),
		       AbstractTable.stringtable.add_string("filename"));
 p = c->get_parent();  // Sets p to the symbol for "Bar"

 ...
</pre>

    It will be useful in writing a Cool compiler to extend the AST with
    new functions such as <code>getParent()</code>. Simply modify the
    <code>cool-tree.java</code> file to add functions to the class of
    the appropriate phylum or constructor.

    <h3>Tips on Using the Tree Package</h3>

    There are a few common errors people make using a tree package.

    <ul>

    <li>The tree package implements an abstract data type.  Violating
    the interface (e.g., by unwarranted casting, etc.)  invites disaster.
    Stick to the interface as it is defined.  When adding new members to
    the class declarations, be careful that those members do not perturb
    the interface for the existing functions.

    <li>The value <code>null</code> is not a valid component of any AST.
    Never use <code>null</code> as an argument to a constructor.

    <li>The tree package functions perform checks to ensure that trees
    are used correctly.  If something bad is detected, the function
    <code>Utilities.fatalError()</code> is invoked to terminate
    execution and a stack trace is printed to the console.

    </ul>

*/
abstract class TreeNode {
    /** line in the source file from which this node came. */
    protected int lineNumber;
    
    /** Builds a new tree node
     *
     * @param lineNumber the line in the source file from which this node came.
     * */
    protected TreeNode(int lineNumber) {
	this.lineNumber = lineNumber;
    }

    /** Creates a copy of this node.
     *
     * @return a copy of this node
     * */
    public abstract TreeNode copy();

    /** Sets the values of this node object to the values of a given node.
     *
     * @param other the other node
     * @return this node
     * */
    public TreeNode set(TreeNode other) {
	this.lineNumber = other.lineNumber;
	return this;
    }

    /** Retreives the line number from which this node came.
     *
     * @return the line number
     * */
    public int getLineNumber() {
	return lineNumber;
    }

    /** Pretty-prints this node to this output stream.
     *
     * @param out the output stream
     * @param n the number of spaces to indent the output
     * */
    public abstract void dump(PrintStream out, int n);

    /** Copies a boolean value.
     * 
     * This method is used internally by the generated AST classes
     * */
    protected Boolean copy_Boolean(Boolean b) {
	return new Boolean(b.booleanValue());
    }

    /** Copies an AbstractSymbol value.
     * 
     * This method is used internally by the generated AST classes
     * */
    protected AbstractSymbol copy_AbstractSymbol(AbstractSymbol sym) {
	return sym;
    }

    /** Dumps a printable representation of a boolean value.
     * 
     * This method is used internally by the generated AST classes
     * */
    protected void dump_Boolean(PrintStream out, int n, Boolean b) {
	out.print(Utilities.pad(n));
	out.println(b.booleanValue() ? "1" : "0");
    }

    /** Dumps a printable representation of an AbstactSymbol value.
     * 
     * This method is used internally by the generated AST classes
     * */
    protected void dump_AbstractSymbol(PrintStream out, int n, AbstractSymbol sym) {
	out.print(Utilities.pad(n));
	out.println(sym.getString());
    }

    /** Dumps a printable representation of current line number
     * 
     * This method is used internally by the generated AST classes
     * */
    protected void dump_line(PrintStream out, int n) {
	out.println(Utilities.pad(n) + "#" + lineNumber);
    }
    
}
	
