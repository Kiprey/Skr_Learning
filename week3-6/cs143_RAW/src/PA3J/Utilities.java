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
import java_cup.runtime.Symbol;

class Utilities {
    // change this to true to enable table checking
    private static final boolean checkTables = false;

    // sm: fixed an off-by-one error here; code assumed there were 80 spaces, but
    // in fact only 79 spaces were there; I've made it 80 now
    //                                         1         2         3         4         5         6         7
    //                               01234567890123456789012345678901234567890123456789012345678901234567890123456789
    private static String padding = "                                                                                "; // 80 spaces for padding
    
    /** Prints error message and exits 
     *
     * @param msg the error message
     * */
    public static void fatalError(String msg) {
	(new Throwable(msg)).printStackTrace();
	System.exit(1);
    }

    /** Prints an appropritely escaped string
     * 
     * @param str the output stream
     * @param s the string to print
     * */
    public static void printEscapedString(PrintStream str, String s) {
	for (int i = 0; i < s.length(); i++) {
	    char c = s.charAt(i);
	    switch (c) {
	    case '\\': str.print("\\\\"); break;
	    case '\"': str.print("\\\""); break;
	    case '\n': str.print("\\n"); break;
	    case '\t': str.print("\\t"); break;
	    case '\b': str.print("\\b"); break;
	    case '\f': str.print("\\f"); break;
	    default:
		if (c >= 0x20 && c <= 0x7f) {
		    str.print(c);
		} else {
		    String octal = Integer.toOctalString(c);
		    str.print('\\');
		    switch (octal.length()) {
		    case 1:
			str.print('0');
		    case 2:
			str.print('0');
		    default:
			str.print(octal);
		    }
		}
	    }
	}
    }

    /** Returns a string representation for a token
     *
     * @param s the token
     * @return the string representation
     * */
    public static String tokenToString(Symbol s) {
	switch (s.sym) {
	case TokenConstants.CLASS:      return("CLASS");
	case TokenConstants.ELSE:       return("ELSE");
	case TokenConstants.FI:         return("FI");
	case TokenConstants.IF:         return("IF");
	case TokenConstants.IN:         return("IN");
	case TokenConstants.INHERITS:   return("INHERITS");
	case TokenConstants.LET:        return("LET");  
	case TokenConstants.LOOP:       return("LOOP"); 
	case TokenConstants.POOL:       return("POOL"); 
	case TokenConstants.THEN:       return("THEN"); 
	case TokenConstants.WHILE:      return("WHILE"); 
	case TokenConstants.ASSIGN:     return("ASSIGN");
	case TokenConstants.CASE:       return("CASE");  
	case TokenConstants.ESAC:       return("ESAC");  
	case TokenConstants.OF:         return("OF");    
	case TokenConstants.DARROW:     return("DARROW");
	case TokenConstants.NEW:        return("NEW");   
	case TokenConstants.STR_CONST:  return("STR_CONST");
	case TokenConstants.INT_CONST:  return("INT_CONST");
	case TokenConstants.BOOL_CONST: return("BOOL_CONST");
	case TokenConstants.TYPEID:     return("TYPEID"); 
	case TokenConstants.OBJECTID:   return("OBJECTID");
	case TokenConstants.ERROR:      return("ERROR"); 
	case TokenConstants.error:      return("ERROR"); 
	case TokenConstants.LE:         return("LE");    
	case TokenConstants.NOT:        return("NOT");   
	case TokenConstants.ISVOID:     return("ISVOID");
	case TokenConstants.PLUS:       return("'+'");
	case TokenConstants.DIV:        return("'/'");
	case TokenConstants.MINUS:      return("'-'");
	case TokenConstants.MULT:       return("'*'");
	case TokenConstants.EQ:         return("'='");
	case TokenConstants.LT:         return("'<'");
	case TokenConstants.DOT:        return("'.'");
	case TokenConstants.NEG:        return("'~'");
	case TokenConstants.COMMA:      return("','");
	case TokenConstants.SEMI:       return("';'");
	case TokenConstants.COLON:      return("':'");
	case TokenConstants.LPAREN:     return("'('");
	case TokenConstants.RPAREN:     return("')'");
	case TokenConstants.AT:         return("'@'");
	case TokenConstants.LBRACE:     return("'{'");
	case TokenConstants.RBRACE:     return("'}'");
	case TokenConstants.EOF:        return("EOF");
	default:                        return("<Invalid Token: " + s.sym + ">");
	}
    }

    /** Prints a token to stderr
     *
     * @param s the token
     * */
    public static void printToken(Symbol s) {
	System.err.print(tokenToString(s));

	String val = null;

	switch (s.sym) {
	case TokenConstants.BOOL_CONST:
	    System.err.print(" = " + s.value);
	    break;
	case TokenConstants.INT_CONST:
	    val = ((AbstractSymbol)s.value).getString();
	    System.err.print(" = " + val);
	    if (checkTables) {
		AbstractTable.inttable.lookup(val);
	    }
	    break;
	case TokenConstants.TYPEID:
	case TokenConstants.OBJECTID:
	    val = ((AbstractSymbol)s.value).getString();
	    System.err.print(" = " + val);
	    if (checkTables) {
		AbstractTable.idtable.lookup(val);
	    }
	    break;
	case TokenConstants.STR_CONST: 
	    val = ((AbstractSymbol)s.value).getString();
	    System.err.print(" = \"");
	    printEscapedString(System.err, val);
	    System.err.print("\"");
	    if (checkTables) {
		AbstractTable.stringtable.lookup(val);
	    }
	    break;
	case TokenConstants.ERROR:
	    System.err.print(" = \"");
	    printEscapedString(System.err, s.value.toString());
	    System.err.print("\"");
	    break;
	}
	System.err.println("");
    }

    /** Dumps a token to the specified stream
     *
     * @param s the token
     * @param str the stream
     * */
    public static void dumpToken(PrintStream str, int lineno, Symbol s) {
	str.print("#" + lineno + " " + tokenToString(s));

	String val = null;

	switch (s.sym) {
	case TokenConstants.BOOL_CONST:
	    str.print(" " + s.value);
	    break;
	case TokenConstants.INT_CONST:
	    val = ((AbstractSymbol)s.value).getString();
	    str.print(" " + val);
	    if (checkTables) {
		AbstractTable.inttable.lookup(val);
	    }
	    break;
	case TokenConstants.TYPEID:
	case TokenConstants.OBJECTID:
	    val = ((AbstractSymbol)s.value).getString();
	    str.print(" " + val);
	    if (checkTables) {
		AbstractTable.idtable.lookup(val);
	    }
	    break;
	case TokenConstants.STR_CONST: 
	    val = ((AbstractSymbol)s.value).getString();
	    str.print(" \"");
	    printEscapedString(str, val);
	    str.print("\"");
	    if (checkTables) {
		AbstractTable.stringtable.lookup(val);
	    }
	    break;
	case TokenConstants.ERROR:
	    str.print(" \"");
	    printEscapedString(str, s.value.toString());
	    str.print("\"");
	    break;
	}

	str.println("");
    }

    /** Returns the specified amount of space padding 
     *
     * @param n the amount of padding
     * */
    public static String pad(int n) {
	if (n > 80) return padding;
	if (n < 0) return "";
	return padding.substring(0, n);
    }
}


	    
	

