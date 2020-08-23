/*
 *  The scanner definition for COOL ASTs
 */
import java_cup.runtime.Symbol;


class ASTLexer implements java_cup.runtime.Scanner {
	private final int YY_BUFFER_SIZE = 512;
	private final int YY_F = -1;
	private final int YY_NO_STATE = -1;
	private final int YY_NOT_ACCEPT = 0;
	private final int YY_START = 1;
	private final int YY_END = 2;
	private final int YY_NO_ANCHOR = 4;
	private final int YY_BOL = 128;
	private final int YY_EOF = 129;

    StringBuffer buf = new StringBuffer();
    int line() { return yyline; }
	private java.io.BufferedReader yy_reader;
	private int yy_buffer_index;
	private int yy_buffer_read;
	private int yy_buffer_start;
	private int yy_buffer_end;
	private char yy_buffer[];
	private int yyline;
	private boolean yy_at_bol;
	private int yy_lexical_state;

	ASTLexer (java.io.Reader reader) {
		this ();
		if (null == reader) {
			throw (new Error("Error: Bad input stream initializer."));
		}
		yy_reader = new java.io.BufferedReader(reader);
	}

	ASTLexer (java.io.InputStream instream) {
		this ();
		if (null == instream) {
			throw (new Error("Error: Bad input stream initializer."));
		}
		yy_reader = new java.io.BufferedReader(new java.io.InputStreamReader(instream));
	}

	private ASTLexer () {
		yy_buffer = new char[YY_BUFFER_SIZE];
		yy_buffer_read = 0;
		yy_buffer_index = 0;
		yy_buffer_start = 0;
		yy_buffer_end = 0;
		yyline = 0;
		yy_at_bol = true;
		yy_lexical_state = YYINITIAL;
	}

	private boolean yy_eof_done = false;
	private final int STRING = 1;
	private final int YYINITIAL = 0;
	private final int yy_state_dtrans[] = {
		0,
		139
	};
	private void yybegin (int state) {
		yy_lexical_state = state;
	}
	private int yy_advance ()
		throws java.io.IOException {
		int next_read;
		int i;
		int j;

		if (yy_buffer_index < yy_buffer_read) {
			return yy_buffer[yy_buffer_index++];
		}

		if (0 != yy_buffer_start) {
			i = yy_buffer_start;
			j = 0;
			while (i < yy_buffer_read) {
				yy_buffer[j] = yy_buffer[i];
				++i;
				++j;
			}
			yy_buffer_end = yy_buffer_end - yy_buffer_start;
			yy_buffer_start = 0;
			yy_buffer_read = j;
			yy_buffer_index = j;
			next_read = yy_reader.read(yy_buffer,
					yy_buffer_read,
					yy_buffer.length - yy_buffer_read);
			if (-1 == next_read) {
				return YY_EOF;
			}
			yy_buffer_read = yy_buffer_read + next_read;
		}

		while (yy_buffer_index >= yy_buffer_read) {
			if (yy_buffer_index >= yy_buffer.length) {
				yy_buffer = yy_double(yy_buffer);
			}
			next_read = yy_reader.read(yy_buffer,
					yy_buffer_read,
					yy_buffer.length - yy_buffer_read);
			if (-1 == next_read) {
				return YY_EOF;
			}
			yy_buffer_read = yy_buffer_read + next_read;
		}
		return yy_buffer[yy_buffer_index++];
	}
	private void yy_move_end () {
		if (yy_buffer_end > yy_buffer_start &&
		    '\n' == yy_buffer[yy_buffer_end-1])
			yy_buffer_end--;
		if (yy_buffer_end > yy_buffer_start &&
		    '\r' == yy_buffer[yy_buffer_end-1])
			yy_buffer_end--;
	}
	private boolean yy_last_was_cr=false;
	private void yy_mark_start () {
		int i;
		for (i = yy_buffer_start; i < yy_buffer_index; ++i) {
			if ('\n' == yy_buffer[i] && !yy_last_was_cr) {
				++yyline;
			}
			if ('\r' == yy_buffer[i]) {
				++yyline;
				yy_last_was_cr=true;
			} else yy_last_was_cr=false;
		}
		yy_buffer_start = yy_buffer_index;
	}
	private void yy_mark_end () {
		yy_buffer_end = yy_buffer_index;
	}
	private void yy_to_mark () {
		yy_buffer_index = yy_buffer_end;
		yy_at_bol = (yy_buffer_end > yy_buffer_start) &&
		            ('\r' == yy_buffer[yy_buffer_end-1] ||
		             '\n' == yy_buffer[yy_buffer_end-1] ||
		             2028/*LS*/ == yy_buffer[yy_buffer_end-1] ||
		             2029/*PS*/ == yy_buffer[yy_buffer_end-1]);
	}
	private java.lang.String yytext () {
		return (new java.lang.String(yy_buffer,
			yy_buffer_start,
			yy_buffer_end - yy_buffer_start));
	}
	private int yylength () {
		return yy_buffer_end - yy_buffer_start;
	}
	private char[] yy_double (char buf[]) {
		int i;
		char newbuf[];
		newbuf = new char[2*buf.length];
		for (i = 0; i < buf.length; ++i) {
			newbuf[i] = buf[i];
		}
		return newbuf;
	}
	private final int YY_E_INTERNAL = 0;
	private final int YY_E_MATCH = 1;
	private java.lang.String yy_error_string[] = {
		"Error: Internal error.\n",
		"Error: Unmatched input.\n"
	};
	private void yy_error (int code,boolean fatal) {
		java.lang.System.out.print(yy_error_string[code]);
		java.lang.System.out.flush();
		if (fatal) {
			throw new Error("Fatal Error.\n");
		}
	}
	private int[][] unpackFromString(int size1, int size2, String st) {
		int colonIndex = -1;
		String lengthString;
		int sequenceLength = 0;
		int sequenceInteger = 0;

		int commaIndex;
		String workString;

		int res[][] = new int[size1][size2];
		for (int i= 0; i < size1; i++) {
			for (int j= 0; j < size2; j++) {
				if (sequenceLength != 0) {
					res[i][j] = sequenceInteger;
					sequenceLength--;
					continue;
				}
				commaIndex = st.indexOf(',');
				workString = (commaIndex==-1) ? st :
					st.substring(0, commaIndex);
				st = st.substring(commaIndex+1);
				colonIndex = workString.indexOf(':');
				if (colonIndex == -1) {
					res[i][j]=Integer.parseInt(workString);
					continue;
				}
				lengthString =
					workString.substring(colonIndex+1);
				sequenceLength=Integer.parseInt(lengthString);
				workString=workString.substring(0,colonIndex);
				sequenceInteger=Integer.parseInt(workString);
				res[i][j] = sequenceInteger;
				sequenceLength--;
			}
		}
		return res;
	}
	private int yy_acpt[] = {
		/* 0 */ YY_NOT_ACCEPT,
		/* 1 */ YY_NO_ANCHOR,
		/* 2 */ YY_NO_ANCHOR,
		/* 3 */ YY_NO_ANCHOR,
		/* 4 */ YY_NO_ANCHOR,
		/* 5 */ YY_NO_ANCHOR,
		/* 6 */ YY_NO_ANCHOR,
		/* 7 */ YY_NO_ANCHOR,
		/* 8 */ YY_NO_ANCHOR,
		/* 9 */ YY_NO_ANCHOR,
		/* 10 */ YY_NO_ANCHOR,
		/* 11 */ YY_NO_ANCHOR,
		/* 12 */ YY_NO_ANCHOR,
		/* 13 */ YY_NO_ANCHOR,
		/* 14 */ YY_NO_ANCHOR,
		/* 15 */ YY_NO_ANCHOR,
		/* 16 */ YY_NO_ANCHOR,
		/* 17 */ YY_NO_ANCHOR,
		/* 18 */ YY_NO_ANCHOR,
		/* 19 */ YY_NO_ANCHOR,
		/* 20 */ YY_NO_ANCHOR,
		/* 21 */ YY_NO_ANCHOR,
		/* 22 */ YY_NO_ANCHOR,
		/* 23 */ YY_NO_ANCHOR,
		/* 24 */ YY_NO_ANCHOR,
		/* 25 */ YY_NO_ANCHOR,
		/* 26 */ YY_NO_ANCHOR,
		/* 27 */ YY_NO_ANCHOR,
		/* 28 */ YY_NO_ANCHOR,
		/* 29 */ YY_NO_ANCHOR,
		/* 30 */ YY_NO_ANCHOR,
		/* 31 */ YY_NO_ANCHOR,
		/* 32 */ YY_NO_ANCHOR,
		/* 33 */ YY_NO_ANCHOR,
		/* 34 */ YY_NO_ANCHOR,
		/* 35 */ YY_NO_ANCHOR,
		/* 36 */ YY_NO_ANCHOR,
		/* 37 */ YY_NO_ANCHOR,
		/* 38 */ YY_NO_ANCHOR,
		/* 39 */ YY_NO_ANCHOR,
		/* 40 */ YY_NO_ANCHOR,
		/* 41 */ YY_NO_ANCHOR,
		/* 42 */ YY_NO_ANCHOR,
		/* 43 */ YY_NO_ANCHOR,
		/* 44 */ YY_NO_ANCHOR,
		/* 45 */ YY_NO_ANCHOR,
		/* 46 */ YY_NO_ANCHOR,
		/* 47 */ YY_NO_ANCHOR,
		/* 48 */ YY_NO_ANCHOR,
		/* 49 */ YY_NOT_ACCEPT,
		/* 50 */ YY_NO_ANCHOR,
		/* 51 */ YY_NO_ANCHOR,
		/* 52 */ YY_NOT_ACCEPT,
		/* 53 */ YY_NOT_ACCEPT,
		/* 54 */ YY_NOT_ACCEPT,
		/* 55 */ YY_NOT_ACCEPT,
		/* 56 */ YY_NOT_ACCEPT,
		/* 57 */ YY_NOT_ACCEPT,
		/* 58 */ YY_NOT_ACCEPT,
		/* 59 */ YY_NOT_ACCEPT,
		/* 60 */ YY_NOT_ACCEPT,
		/* 61 */ YY_NOT_ACCEPT,
		/* 62 */ YY_NOT_ACCEPT,
		/* 63 */ YY_NOT_ACCEPT,
		/* 64 */ YY_NOT_ACCEPT,
		/* 65 */ YY_NOT_ACCEPT,
		/* 66 */ YY_NOT_ACCEPT,
		/* 67 */ YY_NOT_ACCEPT,
		/* 68 */ YY_NOT_ACCEPT,
		/* 69 */ YY_NOT_ACCEPT,
		/* 70 */ YY_NOT_ACCEPT,
		/* 71 */ YY_NOT_ACCEPT,
		/* 72 */ YY_NOT_ACCEPT,
		/* 73 */ YY_NOT_ACCEPT,
		/* 74 */ YY_NOT_ACCEPT,
		/* 75 */ YY_NOT_ACCEPT,
		/* 76 */ YY_NOT_ACCEPT,
		/* 77 */ YY_NOT_ACCEPT,
		/* 78 */ YY_NOT_ACCEPT,
		/* 79 */ YY_NOT_ACCEPT,
		/* 80 */ YY_NOT_ACCEPT,
		/* 81 */ YY_NOT_ACCEPT,
		/* 82 */ YY_NOT_ACCEPT,
		/* 83 */ YY_NOT_ACCEPT,
		/* 84 */ YY_NOT_ACCEPT,
		/* 85 */ YY_NOT_ACCEPT,
		/* 86 */ YY_NOT_ACCEPT,
		/* 87 */ YY_NOT_ACCEPT,
		/* 88 */ YY_NOT_ACCEPT,
		/* 89 */ YY_NOT_ACCEPT,
		/* 90 */ YY_NOT_ACCEPT,
		/* 91 */ YY_NOT_ACCEPT,
		/* 92 */ YY_NOT_ACCEPT,
		/* 93 */ YY_NOT_ACCEPT,
		/* 94 */ YY_NOT_ACCEPT,
		/* 95 */ YY_NOT_ACCEPT,
		/* 96 */ YY_NOT_ACCEPT,
		/* 97 */ YY_NOT_ACCEPT,
		/* 98 */ YY_NOT_ACCEPT,
		/* 99 */ YY_NOT_ACCEPT,
		/* 100 */ YY_NOT_ACCEPT,
		/* 101 */ YY_NOT_ACCEPT,
		/* 102 */ YY_NOT_ACCEPT,
		/* 103 */ YY_NOT_ACCEPT,
		/* 104 */ YY_NOT_ACCEPT,
		/* 105 */ YY_NOT_ACCEPT,
		/* 106 */ YY_NOT_ACCEPT,
		/* 107 */ YY_NOT_ACCEPT,
		/* 108 */ YY_NOT_ACCEPT,
		/* 109 */ YY_NOT_ACCEPT,
		/* 110 */ YY_NOT_ACCEPT,
		/* 111 */ YY_NOT_ACCEPT,
		/* 112 */ YY_NOT_ACCEPT,
		/* 113 */ YY_NOT_ACCEPT,
		/* 114 */ YY_NOT_ACCEPT,
		/* 115 */ YY_NOT_ACCEPT,
		/* 116 */ YY_NOT_ACCEPT,
		/* 117 */ YY_NOT_ACCEPT,
		/* 118 */ YY_NOT_ACCEPT,
		/* 119 */ YY_NOT_ACCEPT,
		/* 120 */ YY_NOT_ACCEPT,
		/* 121 */ YY_NOT_ACCEPT,
		/* 122 */ YY_NOT_ACCEPT,
		/* 123 */ YY_NOT_ACCEPT,
		/* 124 */ YY_NOT_ACCEPT,
		/* 125 */ YY_NOT_ACCEPT,
		/* 126 */ YY_NOT_ACCEPT,
		/* 127 */ YY_NOT_ACCEPT,
		/* 128 */ YY_NOT_ACCEPT,
		/* 129 */ YY_NOT_ACCEPT,
		/* 130 */ YY_NOT_ACCEPT,
		/* 131 */ YY_NOT_ACCEPT,
		/* 132 */ YY_NOT_ACCEPT,
		/* 133 */ YY_NOT_ACCEPT,
		/* 134 */ YY_NOT_ACCEPT,
		/* 135 */ YY_NOT_ACCEPT,
		/* 136 */ YY_NOT_ACCEPT,
		/* 137 */ YY_NOT_ACCEPT,
		/* 138 */ YY_NOT_ACCEPT,
		/* 139 */ YY_NOT_ACCEPT,
		/* 140 */ YY_NOT_ACCEPT,
		/* 141 */ YY_NOT_ACCEPT,
		/* 142 */ YY_NOT_ACCEPT,
		/* 143 */ YY_NOT_ACCEPT,
		/* 144 */ YY_NOT_ACCEPT,
		/* 145 */ YY_NOT_ACCEPT,
		/* 146 */ YY_NOT_ACCEPT,
		/* 147 */ YY_NOT_ACCEPT,
		/* 148 */ YY_NOT_ACCEPT,
		/* 149 */ YY_NOT_ACCEPT,
		/* 150 */ YY_NOT_ACCEPT,
		/* 151 */ YY_NOT_ACCEPT,
		/* 152 */ YY_NOT_ACCEPT,
		/* 153 */ YY_NOT_ACCEPT,
		/* 154 */ YY_NOT_ACCEPT,
		/* 155 */ YY_NOT_ACCEPT,
		/* 156 */ YY_NOT_ACCEPT
	};
	private int yy_cmap[] = unpackFromString(1,130,
"36:8,37:2,1,36,37,1,36:18,37,36,34,3,36:4,31,32,36:6,2:10,30,36:6,33:26,36," +
"35,36:2,4,36,9,19,11,17,14,18,8,16,21,29,23,12,10,20,7,5,26,6,13,15,24,25,2" +
"7,28,22,33,36:5,0:2")[0];

	private int yy_rmap[] = unpackFromString(1,157,
"0,1:2,2,3,1:4,4,1:38,5,4,6,5,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,2" +
"3,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,4" +
"8,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,7" +
"3,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,9" +
"8,99,100,101,102,103,104,105,106,107,108,109,110,111")[0];

	private int yy_nxt[][] = unpackFromString(112,38,
"1,2,3,49,52,4:25,5,6,7,4,8,-1:2,2,-1:40,3,-1:37,4,-1,4:26,-1:3,4,-1:6,9,-1:" +
"37,48,-1:37,43:13,44,43:2,45,46,47,43:3,51,43:13,-1:5,53,-1,54,-1,55,56,57," +
"58,59,60,61,-1,62,63,64,65,66,-1:22,140,-1:5,67,-1:44,68,-1:31,69,-1,70,-1:" +
"36,141,-1:9,71,-1:20,72,-1:4,73,-1:32,152,-1:6,74,10,-1:37,75,-1:8,76,-1:39" +
",11,-1:33,77,-1:36,78,-1:23,79,-1:36,142,80,-1:4,143,-1:32,81,-1:6,82,-1:36" +
",83,-1:6,84,-1:41,86,-1:42,87,-1:21,88,-1:39,89,-1:34,12,-1:35,91,-1:9,92,-" +
"1:26,93,-1:43,13,-1:10,14,-1:17,144,-1:2,95,-1:47,15,-1:23,96,-1:45,97,-1:1" +
"1,153,-1:18,98,-1:38,100,-1:34,101,-1:41,16,-1:18,17,-1:35,102,-1:27,18,-1:" +
"30,103,-1:42,19,-1:38,154,-1:44,104,-1:22,20,-1:47,147,-1:26,21,-1:49,22,-1" +
":33,105,-1:29,23,-1:47,106,-1:33,107,-1:31,149,-1:42,155,-1:47,109,-1:29,24" +
",-1:39,111,112,-1:29,148,-1:36,156,-1:39,114,-1:42,25,-1:45,150,-1:25,117,-" +
"1:45,119,-1:31,121,-1:49,26,-1:42,122,-1:31,151,-1:30,27,-1:42,28,-1:34,29," +
"-1:28,30,-1:42,126,-1:39,127,-1:36,31,-1:35,32,-1:41,33,-1:26,128,-1:49,34," +
"-1:30,35,-1:31,130,-1:47,36,-1:34,131,-1:32,37,-1:45,38,-1:40,132,-1:36,39," +
"-1:42,133,-1:29,134,-1:29,135,-1:41,136,-1:43,137,-1:33,138,-1:42,40,-1:21," +
"1,-1,41:32,42,50,41:2,-1:7,85,-1:45,90,-1:31,99,-1:35,145,-1:51,146,-1:27,1" +
"10,-1:46,116,-1:24,115,-1:51,123,-1:25,118,-1:39,125,-1:31,129,-1:39,94,-1:" +
"51,108,-1:27,113,-1:35,120,-1:37,124,-1:28");

	public java_cup.runtime.Symbol next_token ()
		throws java.io.IOException {
		int yy_lookahead;
		int yy_anchor = YY_NO_ANCHOR;
		int yy_state = yy_state_dtrans[yy_lexical_state];
		int yy_next_state = YY_NO_STATE;
		int yy_last_accept_state = YY_NO_STATE;
		boolean yy_initial = true;
		int yy_this_accept;

		yy_mark_start();
		yy_this_accept = yy_acpt[yy_state];
		if (YY_NOT_ACCEPT != yy_this_accept) {
			yy_last_accept_state = yy_state;
			yy_mark_end();
		}
		while (true) {
			if (yy_initial && yy_at_bol) yy_lookahead = YY_BOL;
			else yy_lookahead = yy_advance();
			yy_next_state = YY_F;
			yy_next_state = yy_nxt[yy_rmap[yy_state]][yy_cmap[yy_lookahead]];
			if (YY_EOF == yy_lookahead && true == yy_initial) {

    return new Symbol(ASTConstants.EOF); 
			}
			if (YY_F != yy_next_state) {
				yy_state = yy_next_state;
				yy_initial = false;
				yy_this_accept = yy_acpt[yy_state];
				if (YY_NOT_ACCEPT != yy_this_accept) {
					yy_last_accept_state = yy_state;
					yy_mark_end();
				}
			}
			else {
				if (YY_NO_STATE == yy_last_accept_state) {
					throw (new Error("Lexical Error: Unmatched Input."));
				}
				else {
					yy_anchor = yy_acpt[yy_last_accept_state];
					if (0 != (YY_END & yy_anchor)) {
						yy_move_end();
					}
					yy_to_mark();
					switch (yy_last_accept_state) {
					case 1:
						
					case -2:
						break;
					case 2:
						{}
					case -3:
						break;
					case 3:
						{ return new Symbol(ASTConstants.INT_CONST,
					    AbstractTable.inttable.addString(yytext())); }
					case -4:
						break;
					case 4:
						{ return new Symbol(ASTConstants.ID,
					    AbstractTable.idtable.addString(yytext())); }
					case -5:
						break;
					case 5:
						{ return new Symbol(ASTConstants.COLON); }
					case -6:
						break;
					case 6:
						{ return new Symbol(ASTConstants.LPAREN); }
					case -7:
						break;
					case 7:
						{ return new Symbol(ASTConstants.RPAREN); }
					case -8:
						break;
					case 8:
						{ buf.setLength(0); yybegin(STRING);}
					case -9:
						break;
					case 9:
						{ return new Symbol(ASTConstants.LINENO, 
					    new Integer(yytext().substring(1))); }
					case -10:
						break;
					case 10:
						{ return new Symbol(ASTConstants.LT); }
					case -11:
						break;
					case 11:
						{ return new Symbol(ASTConstants.EQ); }
					case -12:
						break;
					case 12:
						{ return new Symbol(ASTConstants.MUL); }
					case -13:
						break;
					case 13:
						{ return new Symbol(ASTConstants.LET); }
					case -14:
						break;
					case 14:
						{ return new Symbol(ASTConstants.LEQ); }
					case -15:
						break;
					case 15:
						{ return new Symbol(ASTConstants.SUB); }
					case -16:
						break;
					case 16:
						{ return new Symbol(ASTConstants.NEG); }
					case -17:
						break;
					case 17:
						{ return new Symbol(ASTConstants.NEW); }
					case -18:
						break;
					case 18:
						{ return new Symbol(ASTConstants.INT); }
					case -19:
						break;
					case 19:
						{ return new Symbol(ASTConstants.PLUS); }
					case -20:
						break;
					case 20:
						{ return new Symbol(ASTConstants.ATTR); }
					case -21:
						break;
					case 21:
						{ return new Symbol(ASTConstants.COMP); }
					case -22:
						break;
					case 22:
						{ return new Symbol(ASTConstants.COND); }
					case -23:
						break;
					case 23:
						{ return new Symbol(ASTConstants.LOOP); }
					case -24:
						break;
					case 24:
						{ return new Symbol(ASTConstants.BOOL); }
					case -25:
						break;
					case 25:
						{ return new Symbol(ASTConstants.CLASS); }
					case -26:
						break;
					case 26:
						{ return new Symbol(ASTConstants.BLOCK); }
					case -27:
						break;
					case 27:
						{ return new Symbol(ASTConstants.OBJECT); }
					case -28:
						break;
					case 28:
						{ return new Symbol(ASTConstants.ASSIGN); }
					case -29:
						break;
					case 29:
						{ return new Symbol(ASTConstants.METHOD); }
					case -30:
						break;
					case 30:
						{ return new Symbol(ASTConstants.STR); }
					case -31:
						break;
					case 31:
						{ return new Symbol(ASTConstants.DIVIDE); }
					case -32:
						break;
					case 32:
						{ return new Symbol(ASTConstants.FORMAL); }
					case -33:
						break;
					case 33:
						{ return new Symbol(ASTConstants.BRANCH); }
					case -34:
						break;
					case 34:
						{ return new Symbol(ASTConstants.ISVOID); }
					case -35:
						break;
					case 35:
						{ return new Symbol(ASTConstants.PROGRAM); }
					case -36:
						break;
					case 36:
						{ return new Symbol(ASTConstants.TYPCASE); }
					case -37:
						break;
					case 37:
						{ return new Symbol(ASTConstants.NO_EXPR); }
					case -38:
						break;
					case 38:
						{ return new Symbol(ASTConstants.NO_TYPE); }
					case -39:
						break;
					case 39:
						{ return new Symbol(ASTConstants.DISPATCH); }
					case -40:
						break;
					case 40:
						{ return new Symbol(ASTConstants.STATIC_DISPATCH);}
					case -41:
						break;
					case 41:
						{ buf.append(yytext()); }
					case -42:
						break;
					case 42:
						{ yybegin(YYINITIAL);
                          return new Symbol(ASTConstants.STR_CONST, 
					    AbstractTable.stringtable.addString(buf.toString())); }
					case -43:
						break;
					case 43:
						{ buf.append(yytext().substring(1)); }
					case -44:
						break;
					case 44:
						{ buf.append('\t'); }
					case -45:
						break;
					case 45:
						{ buf.append('\f'); }
					case -46:
						break;
					case 46:
						{ buf.append('\b'); }
					case -47:
						break;
					case 47:
						{ buf.append('\n'); }
					case -48:
						break;
					case 48:
						{ buf.append((char)Integer.parseInt(yytext().substring(2), 16)); }
					case -49:
						break;
					case 50:
						{ buf.append(yytext()); }
					case -50:
						break;
					case 51:
						{ buf.append(yytext().substring(1)); }
					case -51:
						break;
					default:
						yy_error(YY_E_INTERNAL,false);
					case -1:
					}
					yy_initial = true;
					yy_state = yy_state_dtrans[yy_lexical_state];
					yy_next_state = YY_NO_STATE;
					yy_last_accept_state = YY_NO_STATE;
					yy_mark_start();
					yy_this_accept = yy_acpt[yy_state];
					if (YY_NOT_ACCEPT != yy_this_accept) {
						yy_last_accept_state = yy_state;
						yy_mark_end();
					}
				}
			}
		}
	}
}
