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

/** An example of using a symbol table. */
class SymtabExample {
    public static void main(String args[]) {
	// crate a new symbol table; entries will be name/age pairs
	SymbolTable map = new SymbolTable();

	// create some names
	AbstractSymbol fred = AbstractTable.stringtable.addString("Fred");
	AbstractSymbol mary = AbstractTable.stringtable.addString("Mary");
	AbstractSymbol miguel = AbstractTable.stringtable.addString("Miguel");
	
	map.enterScope();

	// add a couple of entries mapping name to age.
	// note the second argument must be a pointer to an integer
	map.addId(fred, new Integer(22));
	map.addId(mary, new Integer(25));

	// add a scope, add more names:
	map.enterScope();
	map.addId(miguel, new Integer(35));
	map.addId(mary, new Integer(23));

	// check whether Fred is in the current scope; predicate is false
	System.out.println((map.probe(fred) != null) ? "Yes" : "No");
	
	// check whether Mary is in any scope; predicate is true
	System.out.println((map.lookup(mary) != null) ? "Yes" : "No");
	
	// print age of most-closely-nested Mary
	System.out.println(map.lookup(mary));

	// check whether Miguel is in the current scope; predicate is true
	System.out.println((map.probe(miguel) != null) ? "Yes" : "No");

	// leave a scope
	map.exitScope();

	// print age of most-closely-nested Mary
	System.out.println(map.lookup(mary));

	// check whether Fred is in the current scope; predicate is now true
	System.out.println((map.probe(fred) != null) ? "Yes" : "No");

	// check whether Miguel is in any scope; predicate is now false
	System.out.println((map.lookup(miguel) != null) ? "Yes" : "No");
    }	
}

 
