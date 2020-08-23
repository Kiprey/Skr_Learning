#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "cool-io.h"  //includes iostream
#include "cool-tree.h"
#include "cgen_gc.h"

extern int optind;            // for option processing
extern char *out_filename;    // name of output assembly
extern Program ast_root;             // root of the abstract syntax tree
FILE *ast_file = stdin;       // we read the AST from standard input
extern int ast_yyparse(void); // entry point to the AST parser

int cool_yydebug;     // not used, but needed to link with handle_flags
char *curr_filename;

void handle_flags(int argc, char *argv[]);

int main(int argc, char *argv[]) {
  int firstfile_index;

  handle_flags(argc,argv);
  firstfile_index = optind;

  if (!out_filename && optind < argc) {   // no -o option
      char *dot = strrchr(argv[optind], '.');
      if (dot) *dot = '\0'; // strip off file extension
      out_filename = new char[strlen(argv[optind])+8];
      strcpy(out_filename, argv[optind]);
      strcat(out_filename, ".s");
  }

  // 
  // Don't touch the output file until we know that earlier phases of the
  // compiler have succeeded.
  //
  ast_yyparse();

  if (out_filename) {
      ofstream s(out_filename);
      if (!s) {
	  cerr << "Cannot open output file " << out_filename << endl;
	  exit(1);
      }
      ast_root->cgen(s);
  } else {
      ast_root->cgen(cout);
  }
}

