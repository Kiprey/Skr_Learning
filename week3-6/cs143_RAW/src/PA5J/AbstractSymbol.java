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

/** String table entry
 *
 * There are three kinds of string table entries:
 * <ul>
 *   <li>a true string,
 *   <li>a string representation of an identifier, and 
 *   <li>a string representation of an integer.
 * </ul>
 *
 * Having separate tables is convenient for code generation.  Different
 * data definitions are generated for string constants (StringSymbol)
 * and integer constants (IntSymbol).  Identifiers (IdSymbol) don't
 * produce static data definitions.
 * <p>
 *
 * codeDef and codeRef (defined by subclasses) are used by the code to
 * produce definitions and references (respectively) to constants.
 *
 * @see AbstractTable
 * */
abstract class AbstractSymbol {
    /** The stored string */
    protected String str;

    /** The index of this entry in the string table */
    protected int index;

    /** Constructs a new table entry.
     * 
     * @param str the entry string 
     * @param len the length of the str prefix that is acually used
     * @param index the table index
     * */
    public AbstractSymbol(String str, int len, int index) {
	this.str = str.length() == len ? str : str.substring(0, len);
	this.index = index;
    }

    /** Tests if the string argument is equal to the string in this symbol.
     *
     * @param str the string to compare
     * @return true if the strings are equal
     * */
    public boolean equalString(String str, int len) {
	String other = str.length() == len ? str : str.substring(0, len);
	return this.str.equals(other);
    }

    /** Tests if the index argument is equal to the index of this symbol.
     *
     * It is only meaningful to compare indecies of symbols from the
     * same string table.
     * 
     * @param index the index to compare
     * @return true if the indecies are equal
     * */
    public boolean equalsIndex(int index) {
	return this.index == index;
    }

    /** Tests if two symbols are equal.
     *
     * Symbol equality is equivalent to equality of their indecies, so it
     * is only meaningful to compare symbols that came from the same
     * string table.
     *
     * @param another the other symbol
     * @return true if the symbols are equal
     * */
    public boolean equals(Object another) {
	return (another instanceof AbstractSymbol) && 
	    ((AbstractSymbol)another).index == this.index;
    }

    /** Returns the string representation of this symbol. */
    public String getString() {
	return str;
    }

    /** Returns a printable representation of this symbol. */
    public String toString() {
	return str;
    }

    /** Returns a copy of this symbol */
    public abstract Object clone();
}

