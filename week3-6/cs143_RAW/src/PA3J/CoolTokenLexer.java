/*
 *  The scanner definition for COOL token stream
 */
import java_cup.runtime.Symbol;


class CoolTokenLexer implements java_cup.runtime.Scanner {
	private final int YY_BUFFER_SIZE = 512;
	private final int YY_F = -1;
	private final int YY_NO_STATE = -1;
	private final int YY_NOT_ACCEPT = 0;
	private final int YY_START = 1;
	private final int YY_END = 2;
	private final int YY_NO_ANCHOR = 4;
	private final int YY_BOL = 128;
	private final int YY_EOF = 129;

    int curr_lineno() {
	return line;
    }
    private AbstractSymbol filename;
    AbstractSymbol curr_filename() {
	return filename;
    }
    int prevstate;
    int line;
    StringBuffer buf = new StringBuffer();
	private java.io.BufferedReader yy_reader;
	private int yy_buffer_index;
	private int yy_buffer_read;
	private int yy_buffer_start;
	private int yy_buffer_end;
	private char yy_buffer[];
	private boolean yy_at_bol;
	private int yy_lexical_state;

	CoolTokenLexer (java.io.Reader reader) {
		this ();
		if (null == reader) {
			throw (new Error("Error: Bad input stream initializer."));
		}
		yy_reader = new java.io.BufferedReader(reader);
	}

	CoolTokenLexer (java.io.InputStream instream) {
		this ();
		if (null == instream) {
			throw (new Error("Error: Bad input stream initializer."));
		}
		yy_reader = new java.io.BufferedReader(new java.io.InputStreamReader(instream));
	}

	private CoolTokenLexer () {
		yy_buffer = new char[YY_BUFFER_SIZE];
		yy_buffer_read = 0;
		yy_buffer_index = 0;
		yy_buffer_start = 0;
		yy_buffer_end = 0;
		yy_at_bol = true;
		yy_lexical_state = YYINITIAL;
	}

	private boolean yy_eof_done = false;
	private final int INT = 4;
	private final int BOOL = 5;
	private final int STR = 2;
	private final int STRING = 8;
	private final int YYINITIAL = 0;
	private final int ERR = 3;
	private final int TYPSYM = 6;
	private final int OBJSYM = 7;
	private final int TOKEN = 1;
	private final int yy_state_dtrans[] = {
		0,
		90,
		166,
		167,
		168,
		169,
		175,
		176,
		177
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
		/* 49 */ YY_NO_ANCHOR,
		/* 50 */ YY_NO_ANCHOR,
		/* 51 */ YY_NO_ANCHOR,
		/* 52 */ YY_NO_ANCHOR,
		/* 53 */ YY_NO_ANCHOR,
		/* 54 */ YY_NO_ANCHOR,
		/* 55 */ YY_NO_ANCHOR,
		/* 56 */ YY_NO_ANCHOR,
		/* 57 */ YY_NO_ANCHOR,
		/* 58 */ YY_NO_ANCHOR,
		/* 59 */ YY_NO_ANCHOR,
		/* 60 */ YY_NO_ANCHOR,
		/* 61 */ YY_NO_ANCHOR,
		/* 62 */ YY_NO_ANCHOR,
		/* 63 */ YY_NO_ANCHOR,
		/* 64 */ YY_NO_ANCHOR,
		/* 65 */ YY_NO_ANCHOR,
		/* 66 */ YY_NO_ANCHOR,
		/* 67 */ YY_NO_ANCHOR,
		/* 68 */ YY_NO_ANCHOR,
		/* 69 */ YY_NO_ANCHOR,
		/* 70 */ YY_NOT_ACCEPT,
		/* 71 */ YY_NO_ANCHOR,
		/* 72 */ YY_NO_ANCHOR,
		/* 73 */ YY_NO_ANCHOR,
		/* 74 */ YY_NO_ANCHOR,
		/* 75 */ YY_NO_ANCHOR,
		/* 76 */ YY_NO_ANCHOR,
		/* 77 */ YY_NO_ANCHOR,
		/* 78 */ YY_NO_ANCHOR,
		/* 79 */ YY_NO_ANCHOR,
		/* 80 */ YY_NO_ANCHOR,
		/* 81 */ YY_NOT_ACCEPT,
		/* 82 */ YY_NO_ANCHOR,
		/* 83 */ YY_NO_ANCHOR,
		/* 84 */ YY_NO_ANCHOR,
		/* 85 */ YY_NOT_ACCEPT,
		/* 86 */ YY_NO_ANCHOR,
		/* 87 */ YY_NO_ANCHOR,
		/* 88 */ YY_NOT_ACCEPT,
		/* 89 */ YY_NO_ANCHOR,
		/* 90 */ YY_NOT_ACCEPT,
		/* 91 */ YY_NO_ANCHOR,
		/* 92 */ YY_NOT_ACCEPT,
		/* 93 */ YY_NO_ANCHOR,
		/* 94 */ YY_NOT_ACCEPT,
		/* 95 */ YY_NO_ANCHOR,
		/* 96 */ YY_NOT_ACCEPT,
		/* 97 */ YY_NO_ANCHOR,
		/* 98 */ YY_NOT_ACCEPT,
		/* 99 */ YY_NO_ANCHOR,
		/* 100 */ YY_NOT_ACCEPT,
		/* 101 */ YY_NO_ANCHOR,
		/* 102 */ YY_NOT_ACCEPT,
		/* 103 */ YY_NO_ANCHOR,
		/* 104 */ YY_NOT_ACCEPT,
		/* 105 */ YY_NO_ANCHOR,
		/* 106 */ YY_NOT_ACCEPT,
		/* 107 */ YY_NO_ANCHOR,
		/* 108 */ YY_NOT_ACCEPT,
		/* 109 */ YY_NO_ANCHOR,
		/* 110 */ YY_NOT_ACCEPT,
		/* 111 */ YY_NO_ANCHOR,
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
		/* 156 */ YY_NOT_ACCEPT,
		/* 157 */ YY_NOT_ACCEPT,
		/* 158 */ YY_NOT_ACCEPT,
		/* 159 */ YY_NOT_ACCEPT,
		/* 160 */ YY_NOT_ACCEPT,
		/* 161 */ YY_NOT_ACCEPT,
		/* 162 */ YY_NOT_ACCEPT,
		/* 163 */ YY_NOT_ACCEPT,
		/* 164 */ YY_NOT_ACCEPT,
		/* 165 */ YY_NOT_ACCEPT,
		/* 166 */ YY_NOT_ACCEPT,
		/* 167 */ YY_NOT_ACCEPT,
		/* 168 */ YY_NOT_ACCEPT,
		/* 169 */ YY_NOT_ACCEPT,
		/* 170 */ YY_NOT_ACCEPT,
		/* 171 */ YY_NOT_ACCEPT,
		/* 172 */ YY_NOT_ACCEPT,
		/* 173 */ YY_NOT_ACCEPT,
		/* 174 */ YY_NOT_ACCEPT,
		/* 175 */ YY_NOT_ACCEPT,
		/* 176 */ YY_NOT_ACCEPT,
		/* 177 */ YY_NOT_ACCEPT,
		/* 178 */ YY_NOT_ACCEPT,
		/* 179 */ YY_NOT_ACCEPT,
		/* 180 */ YY_NOT_ACCEPT,
		/* 181 */ YY_NOT_ACCEPT,
		/* 182 */ YY_NOT_ACCEPT,
		/* 183 */ YY_NOT_ACCEPT,
		/* 184 */ YY_NOT_ACCEPT,
		/* 185 */ YY_NOT_ACCEPT,
		/* 186 */ YY_NOT_ACCEPT,
		/* 187 */ YY_NOT_ACCEPT,
		/* 188 */ YY_NOT_ACCEPT,
		/* 189 */ YY_NOT_ACCEPT,
		/* 190 */ YY_NOT_ACCEPT,
		/* 191 */ YY_NOT_ACCEPT,
		/* 192 */ YY_NOT_ACCEPT,
		/* 193 */ YY_NOT_ACCEPT,
		/* 194 */ YY_NOT_ACCEPT,
		/* 195 */ YY_NOT_ACCEPT,
		/* 196 */ YY_NOT_ACCEPT,
		/* 197 */ YY_NOT_ACCEPT,
		/* 198 */ YY_NO_ANCHOR,
		/* 199 */ YY_NOT_ACCEPT,
		/* 200 */ YY_NOT_ACCEPT,
		/* 201 */ YY_NOT_ACCEPT,
		/* 202 */ YY_NOT_ACCEPT
	};
	private int yy_cmap[] = unpackFromString(1,130,
"47:9,48,1,48:2,1,47:18,48,47,8,2,47:3,26,38,39,30,27,35,29,33,28,58:4,59:4," +
"3:2,37,36,32,31,47:2,40,11,44,9,24,13,14,23,17,15,46,55,10,55,16,20,21,55,1" +
"8,12,19,55,25,22,55,45,55,47,56,47:2,43,47,5,57,55:2,7,52,55:5,53,6,4,55:3," +
"50,54,49,51,55:5,42,47,41,34,47,0:2")[0];

	private int yy_rmap[] = unpackFromString(1,203,
"0,1,2,3,4,1,3,5,1:2,6,1:38,3,1,3,1,3,7,3,1:2,3,8,3,9,3,1:7,10,1:9,11,12,13," +
"14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38," +
"39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63," +
"64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88," +
"89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,11" +
"0,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,128,1" +
"29,130,131,132,133")[0];

	private int yy_nxt[][] = unpackFromString(134,60,
"1,2,3,71:45,2,71:11,-1:61,2,-1:46,2,-1:14,4,70,-1:53,4:2,-1:3,4,-1:54,4:2,-" +
"1:19,12,-1:57,183,-1,202,-1:43,54,-1:54,54:2,-1:3,59:5,-1,59:17,-1:17,59:4," +
"-1:2,59:7,-1,59:3,-1:3,61:5,-1,61:17,-1:17,61:4,-1:2,61:7,-1,61:3,-1:5,81,-" +
"1:112,178:2,-1:6,85,-1:63,92,94,-1:98,170,-1:11,64:2,65,64:44,66,64:2,67,64" +
":4,68,80,64,-1:7,88,-1:65,7,-1:6,96,-1:44,171,-1:55,88,-1:6,5,-1:39,88,-1:2" +
"3,180,-1:47,1,2,6,72:6,82,86,89,91,93,95,97,99,72:2,101,103,105,107,72,109," +
"72,111,72:17,198,72:3,2,72:11,-1:19,98,-1:51,197,-1:58,194,-1,179,-1:5,182," +
"-1:53,129,-1:62,8,-1:64,130,-1:51,100,-1,9,-1,10,-1:61,131,-1:54,102,-1:6,1" +
"04,-1:64,185,-1:51,106,-1:27,108,-1:36,13,-1:51,11,-1:29,110,-1:34,14,-1:60" +
",181,-1:52,135,-1:63,112,-1:63,136,-1:49,196,-1:94,186,-1:40,113,114,115,11" +
"6,117,118,119,120,121,122,123,124,125,126,127,128,-1:32,138,-1:70,15,-1:59," +
"16,-1:59,17,-1:59,18,-1:59,19,-1:59,20,-1:59,21,-1:59,22,-1:59,23,-1:59,24," +
"-1:59,25,-1:59,26,-1:59,27,-1:59,28,-1:59,29,-1:59,30,-1:46,31,-1:67,32,-1:" +
"81,142,-1:29,33,-1:55,34,-1:70,143,-1:55,35,-1:56,190,-1:56,36,-1:59,145,-1" +
":67,146,-1:53,37,-1:70,148,-1:45,191,-1:68,38,-1:56,149,-1:57,39,-1:66,153," +
"-1:82,154,-1:32,40,-1:67,41,-1:50,192,-1:68,42,-1:54,156,-1:62,43,-1:46,157" +
",-1:66,158,-1:58,160,-1:64,161,-1:51,162,-1:59,44,-1:71,45,-1:51,164,-1:62," +
"46,-1:59,47,-1:52,165,-1:66,48,-1:40,1,2,49,73:5,50,73:39,2,73:11,1,2,51,74" +
":5,52,74:39,2,74:11,1,2,53,54,75:44,2,75:9,54:2,1,2,55,76:45,2,83,76:2,87,7" +
"6:7,-1:51,172,-1:61,173,-1:13,56,-1:106,174,-1:12,57,-1:52,1,2,58,77,59:4,7" +
"7,59:17,77:18,59:3,77,2,59:7,77,59,77:2,1,2,60,78,61:4,78,61:17,78:18,61:3," +
"78,2,61:7,78,61,78:2,1,-1,62,79:5,63,79:47,84,79:3,-1:58,69:2,-1:11,133,-1:" +
"60,184,-1:67,137,-1:57,134,-1:54,188,-1:61,141,-1:64,144,-1:52,189,-1:56,14" +
"7,-1:67,150,-1:50,152,-1:65,151,-1:64,155,-1:58,159,-1:52,163,-1:59,132,-1:" +
"67,187,-1:57,139,-1:53,140,-1:67,195,-1:55,193,-1:63,199,-1:48,200,-1:93,20" +
"1,-1:16");

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

    return new Symbol(TokenConstants.EOF); 
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
						{ throw new Error("unmatched text in token lexer; line number expected"); }
					case -4:
						break;
					case 4:
						{ 
		        try {
			    line = Integer.parseInt(yytext().substring(1)); 
			} catch (NumberFormatException ex) {
			    ex.printStackTrace(System.err);
			    line = 0;
			}
			yybegin(TOKEN); 
		      }
					case -5:
						break;
					case 5:
						{ buf.setLength(0); prevstate = YYINITIAL; yybegin(STRING); }
					case -6:
						break;
					case 6:
						{ throw new Error("unmatched text in token lexer; token expected"); }
					case -7:
						break;
					case 7:
						{ yybegin(YYINITIAL); return new Symbol(TokenConstants.LE); }
					case -8:
						break;
					case 8:
						{ yybegin(YYINITIAL); return new Symbol(TokenConstants.FI); }
					case -9:
						break;
					case 9:
						{ yybegin(YYINITIAL); return new Symbol(TokenConstants.IF); }
					case -10:
						break;
					case 10:
						{ yybegin(YYINITIAL); return new Symbol(TokenConstants.IN); }
					case -11:
						break;
					case 11:
						{ yybegin(YYINITIAL); return new Symbol(TokenConstants.OF); }
					case -12:
						break;
					case 12:
						{ yybegin(YYINITIAL); return new Symbol(TokenConstants.LET); }
					case -13:
						break;
					case 13:
						{ yybegin(YYINITIAL); return new Symbol(TokenConstants.NEW); }
					case -14:
						break;
					case 14:
						{ yybegin(YYINITIAL); return new Symbol(TokenConstants.NOT); }
					case -15:
						break;
					case 15:
						{ yybegin(YYINITIAL); return new Symbol(TokenConstants.PLUS); }
					case -16:
						break;
					case 16:
						{ yybegin(YYINITIAL); return new Symbol(TokenConstants.DIV); }
					case -17:
						break;
					case 17:
						{ yybegin(YYINITIAL); return new Symbol(TokenConstants.MINUS); }
					case -18:
						break;
					case 18:
						{ yybegin(YYINITIAL); return new Symbol(TokenConstants.MULT); }
					case -19:
						break;
					case 19:
						{ yybegin(YYINITIAL); return new Symbol(TokenConstants.EQ); }
					case -20:
						break;
					case 20:
						{ yybegin(YYINITIAL); return new Symbol(TokenConstants.LT); }
					case -21:
						break;
					case 21:
						{ yybegin(YYINITIAL); return new Symbol(TokenConstants.DOT); }
					case -22:
						break;
					case 22:
						{ yybegin(YYINITIAL); return new Symbol(TokenConstants.NEG); }
					case -23:
						break;
					case 23:
						{ yybegin(YYINITIAL); return new Symbol(TokenConstants.COMMA); }
					case -24:
						break;
					case 24:
						{ yybegin(YYINITIAL); return new Symbol(TokenConstants.SEMI); }
					case -25:
						break;
					case 25:
						{ yybegin(YYINITIAL); return new Symbol(TokenConstants.COLON); }
					case -26:
						break;
					case 26:
						{ yybegin(YYINITIAL); return new Symbol(TokenConstants.LPAREN); }
					case -27:
						break;
					case 27:
						{ yybegin(YYINITIAL); return new Symbol(TokenConstants.RPAREN); }
					case -28:
						break;
					case 28:
						{ yybegin(YYINITIAL); return new Symbol(TokenConstants.AT); }
					case -29:
						break;
					case 29:
						{ yybegin(YYINITIAL); return new Symbol(TokenConstants.RBRACE); }
					case -30:
						break;
					case 30:
						{ yybegin(YYINITIAL); return new Symbol(TokenConstants.LBRACE); }
					case -31:
						break;
					case 31:
						{ yybegin(YYINITIAL); return new Symbol(TokenConstants.CASE); }
					case -32:
						break;
					case 32:
						{ yybegin(YYINITIAL); return new Symbol(TokenConstants.LOOP); }
					case -33:
						break;
					case 33:
						{ yybegin(YYINITIAL); return new Symbol(TokenConstants.ELSE); }
					case -34:
						break;
					case 34:
						{ yybegin(YYINITIAL); return new Symbol(TokenConstants.ESAC); }
					case -35:
						break;
					case 35:
						{ yybegin(YYINITIAL); return new Symbol(TokenConstants.THEN); }
					case -36:
						break;
					case 36:
						{ yybegin(YYINITIAL); return new Symbol(TokenConstants.POOL); }
					case -37:
						break;
					case 37:
						{ yybegin(YYINITIAL); return new Symbol(TokenConstants.CLASS); }
					case -38:
						break;
					case 38:
						{ yybegin(ERR); }
					case -39:
						break;
					case 39:
						{ yybegin(YYINITIAL); return new Symbol(TokenConstants.WHILE); }
					case -40:
						break;
					case 40:
						{ yybegin(YYINITIAL); return new Symbol(TokenConstants.ASSIGN); }
					case -41:
						break;
					case 41:
						{ yybegin(YYINITIAL); return new Symbol(TokenConstants.ISVOID); }
					case -42:
						break;
					case 42:
						{ yybegin(TYPSYM); }
					case -43:
						break;
					case 43:
						{ yybegin(YYINITIAL); return new Symbol(TokenConstants.DARROW); }
					case -44:
						break;
					case 44:
						{ yybegin(YYINITIAL); return new Symbol(TokenConstants.INHERITS); }
					case -45:
						break;
					case 45:
						{ yybegin(OBJSYM); }
					case -46:
						break;
					case 46:
						{ yybegin(STR); }
					case -47:
						break;
					case 47:
						{ yybegin(INT); }
					case -48:
						break;
					case 48:
						{ yybegin(BOOL); }
					case -49:
						break;
					case 49:
						{ throw new Error("unmatched text in token lexer; string constant expected"); }
					case -50:
						break;
					case 50:
						{ prevstate = STR; buf.setLength(0); yybegin(STRING);}
					case -51:
						break;
					case 51:
						{ throw new Error("unmatched text in token lexer; error message expected"); }
					case -52:
						break;
					case 52:
						{ prevstate = ERR; buf.setLength(0); yybegin(STRING);}
					case -53:
						break;
					case 53:
						{ throw new Error("unmatched text in token lexer; integer constant expected"); }
					case -54:
						break;
					case 54:
						{ yybegin(YYINITIAL); 
		        return new Symbol(TokenConstants.INT_CONST,
			                  AbstractTable.inttable.addString(yytext())); }
					case -55:
						break;
					case 55:
						{ throw new Error("unmatched text in token lexer; boolean constant expected"); }
					case -56:
						break;
					case 56:
						{ yybegin(YYINITIAL);
		        return new Symbol(TokenConstants.BOOL_CONST, Boolean.TRUE); }
					case -57:
						break;
					case 57:
						{ yybegin(YYINITIAL);
		        return new Symbol(TokenConstants.BOOL_CONST, Boolean.FALSE); }
					case -58:
						break;
					case 58:
						{ throw new Error("unmatched text in token lexer; type symbol expected"); }
					case -59:
						break;
					case 59:
						{ yybegin(YYINITIAL);
		        return new Symbol(TokenConstants.TYPEID, 
			                  AbstractTable.idtable.addString(yytext())); }
					case -60:
						break;
					case 60:
						{ throw new Error("unmatched text in token lexer; type symbol expected"); }
					case -61:
						break;
					case 61:
						{ yybegin(YYINITIAL);
		        return new Symbol(TokenConstants.OBJECTID, 
			                  AbstractTable.idtable.addString(yytext())); }
					case -62:
						break;
					case 62:
						{ buf.append(yytext()); }
					case -63:
						break;
					case 63:
						{ yybegin(YYINITIAL);
			switch(prevstate) {
			case STR: 
			    return new Symbol(TokenConstants.STR_CONST, 
			                      AbstractTable.stringtable.addString(buf.toString())); 
			case ERR:
			    return new Symbol(TokenConstants.ERROR, 
			                      buf.toString()); 
			case YYINITIAL:
			    filename = 	AbstractTable.stringtable.addString(buf.toString());
			    break;
			default:
			    throw new Error("unknown state");
			}
                      }
					case -64:
						break;
					case 64:
						{ buf.append(yytext().substring(1)); }
					case -65:
						break;
					case 65:
						{ buf.append('\n'); }
					case -66:
						break;
					case 66:
						{ buf.append('\t'); }
					case -67:
						break;
					case 67:
						{ buf.append('\f'); }
					case -68:
						break;
					case 68:
						{ buf.append('\b'); }
					case -69:
						break;
					case 69:
						{ buf.append((char)Integer.parseInt(yytext().substring(1), 8)); }
					case -70:
						break;
					case 71:
						{ throw new Error("unmatched text in token lexer; line number expected"); }
					case -71:
						break;
					case 72:
						{ throw new Error("unmatched text in token lexer; token expected"); }
					case -72:
						break;
					case 73:
						{ throw new Error("unmatched text in token lexer; string constant expected"); }
					case -73:
						break;
					case 74:
						{ throw new Error("unmatched text in token lexer; error message expected"); }
					case -74:
						break;
					case 75:
						{ throw new Error("unmatched text in token lexer; integer constant expected"); }
					case -75:
						break;
					case 76:
						{ throw new Error("unmatched text in token lexer; boolean constant expected"); }
					case -76:
						break;
					case 77:
						{ throw new Error("unmatched text in token lexer; type symbol expected"); }
					case -77:
						break;
					case 78:
						{ throw new Error("unmatched text in token lexer; type symbol expected"); }
					case -78:
						break;
					case 79:
						{ buf.append(yytext()); }
					case -79:
						break;
					case 80:
						{ buf.append(yytext().substring(1)); }
					case -80:
						break;
					case 82:
						{ throw new Error("unmatched text in token lexer; token expected"); }
					case -81:
						break;
					case 83:
						{ throw new Error("unmatched text in token lexer; boolean constant expected"); }
					case -82:
						break;
					case 84:
						{ buf.append(yytext()); }
					case -83:
						break;
					case 86:
						{ throw new Error("unmatched text in token lexer; token expected"); }
					case -84:
						break;
					case 87:
						{ throw new Error("unmatched text in token lexer; boolean constant expected"); }
					case -85:
						break;
					case 89:
						{ throw new Error("unmatched text in token lexer; token expected"); }
					case -86:
						break;
					case 91:
						{ throw new Error("unmatched text in token lexer; token expected"); }
					case -87:
						break;
					case 93:
						{ throw new Error("unmatched text in token lexer; token expected"); }
					case -88:
						break;
					case 95:
						{ throw new Error("unmatched text in token lexer; token expected"); }
					case -89:
						break;
					case 97:
						{ throw new Error("unmatched text in token lexer; token expected"); }
					case -90:
						break;
					case 99:
						{ throw new Error("unmatched text in token lexer; token expected"); }
					case -91:
						break;
					case 101:
						{ throw new Error("unmatched text in token lexer; token expected"); }
					case -92:
						break;
					case 103:
						{ throw new Error("unmatched text in token lexer; token expected"); }
					case -93:
						break;
					case 105:
						{ throw new Error("unmatched text in token lexer; token expected"); }
					case -94:
						break;
					case 107:
						{ throw new Error("unmatched text in token lexer; token expected"); }
					case -95:
						break;
					case 109:
						{ throw new Error("unmatched text in token lexer; token expected"); }
					case -96:
						break;
					case 111:
						{ throw new Error("unmatched text in token lexer; token expected"); }
					case -97:
						break;
					case 198:
						{ throw new Error("unmatched text in token lexer; token expected"); }
					case -98:
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
