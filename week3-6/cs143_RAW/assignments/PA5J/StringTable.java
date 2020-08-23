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

class StringTable extends AbstractTable {
    /** Creates a new StringSymbol object.
     * 
     * @see StringSymbol
     * */
    protected AbstractSymbol getNewSymbol(String s, int len, int index) {
	return new StringSymbol(s, len, index);
    }

    /** Generates code for all string constants in the string table.  
     * @param stringclasstag the class tag for String
     * @param s the output stream
     * */
    public void codeStringTable(int stringclasstag, PrintStream s) {
	StringSymbol sym = null;
	for (int i = tbl.size() - 1; i >= 0; i--) {
	    try {
		sym = (StringSymbol)tbl.elementAt(i);
	    } catch (ArrayIndexOutOfBoundsException ex) {
		Utilities.fatalError("Unexpected exception: " + ex);
	    }
	    sym.codeDef(stringclasstag, s);
	}
    }
}
