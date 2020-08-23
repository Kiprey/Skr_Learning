/*
 *  The scanner definition for COOL.
 */
%option noyywrap
/*
 *  Stuff enclosed in %{ %} in the first section is copied verbatim to the
 *  output, so headers and global definitions are placed here to be visible
 * to the code in the file.  Don't remove anything that was here initially
 */
%{
#include <cool-parse.h>
#include <stringtab.h>
#include <utilities.h>

/* The compiler assumes these identifiers. */
#define yylval cool_yylval
#define yylex  cool_yylex

/* Max size of string constants */
#define MAX_STR_CONST 1025
#define YY_NO_UNPUT   /* keep g++ happy */

extern FILE *fin; /* we read from this file */

/* define YY_INPUT so we read from the FILE fin:
 * This change makes it possible to use this scanner in
 * the Cool compiler.
 */
#undef YY_INPUT
#define YY_INPUT(buf,result,max_size) \
	if ( (result = fread( (char*)buf, sizeof(char), max_size, fin)) < 0) \
		YY_FATAL_ERROR( "read() in flex scanner failed");

char string_buf[MAX_STR_CONST]; /* to assemble string constants */
char *str_buf_ptr;

extern int curr_lineno;
extern int verbose_flag;

extern YYSTYPE cool_yylval;

/*
 *  Add Your own definitions here
 */
// 嵌套注释的个数
int quote_comment_num = 0;
// 恢复初始状态并返回错误
#define ERROR_AND_INITIAL \
  BEGIN(INITIAL); \
  return ERROR;
// 检查字符串长度是否过大
#define CHK_STR_LEN \
  if (str_buf_ptr - string_buf >= MAX_STR_CONST) \
  { \
    char ch;\
    cool_yylval.error_msg = "String constant too long"; \
    while((ch = yyinput()) != '\"' && ch != EOF) \
      continue; \
    ERROR_AND_INITIAL   \
  } 
%}
 /* start condition */
%x QUOTE_COMMENT
%x DASH_COMMENT
%x STRING
/*
 * Define names for regular expressions here.
 */

CLASS     (?i:class)
ELSE      (?i:else)
FI        (?i:fi)
IF        (?i:if)
IN        (?i:in)
INHERITS  (?i:inherits)
LET       (?i:let)
LOOP      (?i:loop)
POOL      (?i:pool)
THEN      (?i:then)
WHILE     (?i:while)
CASE      (?i:case)
ESAC      (?i:esac)
OF        (?i:of)
NEW       (?i:new)
ISVOID    (?i:isvoid)
NOT       (?i:not)
TRUE      (t(?i:rue))
FALSE     (f(?i:alse))

DIGIT     [0-9]
INT       {DIGIT}+
LETTER    [a-zA-Z]
ID        ({LETTER}|{DIGIT}|_)
TYPEID    [A-Z]{ID}*
OBJID     [a-z]{ID}*

WHITESPACE  [\ \t\b\f\r\v]*
SINGLE_OPERATOR      [\<\=\+/\-\*\.~\,;\:\(\)@\{\}]

%% 

 /*
  *  Nested comments
  */
"--"                        { BEGIN(DASH_COMMENT); }
<DASH_COMMENT>.             {}
<DASH_COMMENT><<EOF>>       { BEGIN(INITIAL); }
<DASH_COMMENT>\n            { BEGIN(INITIAL); curr_lineno++; }

<QUOTE_COMMENT>"*)"         {
  if (!--quote_comment_num)
    BEGIN(INITIAL); 
}
<QUOTE_COMMENT,INITIAL>"(*" { 
  if(!quote_comment_num++)
    BEGIN(QUOTE_COMMENT); 
}
<QUOTE_COMMENT><<EOF>>      {
  cool_yylval.error_msg = "EOF in comment"; 
  ERROR_AND_INITIAL
}
<QUOTE_COMMENT>\n           { curr_lineno++; }
<QUOTE_COMMENT>.            {}

<INITIAL>"*)"               {
  cool_yylval.error_msg = "Unmatched *)"; 
  return ERROR; 
}

{WHITESPACE}  {}

 /*
  *  The multiple-character operators.
  */

"=>"	                { return DARROW;}
"<-"                  { return ASSIGN; }
"<="                  { return LE; }

{SINGLE_OPERATOR}     { return *yytext;}

 /*
  * Keywords are case-insensitive except for the values true and false,
  * which must begin with a lower-case letter.
  */

{CLASS}     { return CLASS; }
{FI}        { return FI;    }
{IF}        { return IF;    }
{ELSE}      { return ELSE;  }
{IN}        { return IN;    }
{INHERITS}  { return INHERITS;}
{LET}       { return LET;   }
{LOOP}      { return LOOP;  }
{POOL}      { return POOL;  }
{THEN}      { return THEN;  }
{WHILE}     { return WHILE; }
{CASE}      { return CASE;  }
{ESAC}      { return ESAC;  }
{OF}        { return OF;    }
{NEW}       { return NEW;   }
{ISVOID}    { return ISVOID;}
{NOT}       { return NOT;   }
{NOT}	    	{ return NOT;   }
{TRUE} {
  cool_yylval.boolean = true;
  return BOOL_CONST;
}
{FALSE} {
  cool_yylval.boolean = false;
  return BOOL_CONST;
}
	
 /*
  * Identifiers
  */

{TYPEID} { 
  cool_yylval.symbol = idtable.add_string(yytext); 
  return TYPEID; 
}
{OBJID} { 
  cool_yylval.symbol = idtable.add_string(yytext); 
  return OBJECTID; 
}
{INT}  {
  cool_yylval.symbol = inttable.add_string(yytext); 
  return INT_CONST; 
}


 /*
  *  String constants (C syntax)
  *  Escape sequence \c is accepted for all characters c. Except for 
  *  \n \t \b \f, the result is c.
  * 
  *  https://github.com/jmoyers/cool-lexer/blob/master/cool.flex
  */
"\""   { 
  str_buf_ptr = string_buf;
  BEGIN(STRING); 
};
<STRING>\\[^\0]   {
  char ch = yytext[1];
  if(ch == 'b')
    *str_buf_ptr++ = '\b';
  else if(ch == 't')
    *str_buf_ptr++ = '\t';
  else if(ch == 'n')
    *str_buf_ptr++ = '\n';
  else if(ch == 'f')
    *str_buf_ptr++ = '\f';
  else
    *str_buf_ptr++ = ch;
  CHK_STR_LEN
}

<STRING>"\""    {
  BEGIN(INITIAL);
  *str_buf_ptr++ = '\0';
  cool_yylval.symbol = stringtable.add_string(string_buf);
  return STR_CONST; 
}
<STRING>"\\\n"   {
  *str_buf_ptr++ = '\n'; 
  curr_lineno++;
  CHK_STR_LEN
}
<STRING><<EOF>>  {
  cool_yylval.error_msg = "EOF in string constant";
  ERROR_AND_INITIAL
}
<STRING>"\0"    {
  char ch;
  char isEscaped = false;
  while((ch = yyinput()) != '\n' && ch != EOF)
  {
    if (ch == '\"')
    {
      isEscaped = true;
      break;
    }
  }
  if (isEscaped)
    cool_yylval.error_msg = "String contains escaped null character.";
  else 
    cool_yylval.error_msg = "String contains null character.";
  
  ERROR_AND_INITIAL
}

<STRING>\n      {
  cool_yylval.error_msg = "EOF in string constant";
  curr_lineno++;
  ERROR_AND_INITIAL
}

<STRING>.       {
  *str_buf_ptr++ = *yytext;
  CHK_STR_LEN
}

 /* 如果在字符串范围外发现反斜杠，则报错 */
\\ {
  cool_yylval.error_msg = strdup(yytext);
  return ERROR; 
}

\n { curr_lineno++; }

 /* 如果还有尚未处理的字符（可能是非cool语法的字符），则报错 */
. {
  cool_yylval.error_msg = strdup(yytext);
  return ERROR; 
}

%%