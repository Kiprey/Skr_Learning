//
// See copyright.h for copyright notice and limitation of liability
// and disclaimer of warranty provisions.
//
#include "copyright.h"

//                                                                           ##
// This file defines all of the extensions of the cool AST for the           ##
// course compiler.  It is necessary for students to have access to          ##
// some of the generic utilities (in particular, the includes and            ##
// and the declaration of dumptype pure virtual functions) but not           ##
// others which reveal implementation details of the course compiler.        ##
//                                                                           ##
// To allow the students version of this file to be generated automatically, ##
// the following elaborate scheme is used.  Lines of this file that are      ##
// to be discarded in the student version are marked with ##.
// Any changes to this file should preserve this convention.                 ##
//
// The following include files must come first.

#ifndef COOL_TREE_HANDCODE_H
#define COOL_TREE_HANDCODE_H

#include "tree.h"
#include "cool.h"
#include "stringtab.h"
#include "symtab.h"                                   /*   ##  */
#define yylineno curr_lineno
extern int yylineno;

inline Boolean copy_Boolean(Boolean b) {return b; }
inline void assert_Boolean(Boolean) {}
inline void dump_Boolean(ostream& stream, int padding, Boolean b)
	{ stream << pad(padding) << (int) b << "\n"; }

void dump_Symbol(ostream& stream, int padding, Symbol b);
void assert_Symbol(Symbol b);
Symbol copy_Symbol(Symbol b);

typedef char* Register;                            /* ## */

class Program_class;
typedef Program_class *Program;
class Class__class;
typedef Class__class *Class_;
class Feature_class;
typedef Feature_class *Feature;
class Formal_class;
typedef Formal_class *Formal;
class Expression_class;
typedef Expression_class *Expression;
class Case_class;
typedef Case_class *Case;

typedef list_node<Class_> Classes_class;
typedef Classes_class *Classes;
typedef list_node<Feature> Features_class;
typedef Features_class *Features;
typedef list_node<Formal> Formals_class;
typedef Formals_class *Formals;
typedef list_node<Expression> Expressions_class;
typedef Expressions_class *Expressions;
typedef list_node<Case> Cases_class;
typedef Cases_class *Cases;

class SymTables;                                     //##
typedef SymTables *SymTabP;                          //##
                                                     //##
class InheritanceNode;                               //##
typedef InheritanceNode *InheritanceNodeP;           //##
                                                     //##
class Environment;                                   //##
typedef Environment *EnvironmentP;                   //##
                                                     //##
class CalcTemp;                                      //##
typedef CalcTemp *CalcTempP;                         //##
                                                     //##
class CgenNode;                                      //##
typedef CgenNode *CgenNodeP;                         //##
                                                     //##
class CgenEnvironment;                               //##
typedef CgenEnvironment *CgenEnvironmentP;           //##
                                                     //##
class CgenEnvClassLevel;                             //##
typedef CgenEnvClassLevel *CgenEnvClassLevelP;       //##
                                                     //##
#define Program_EXTRAS                          \
virtual InheritanceNodeP semant() = 0; /* ## */ \
virtual void dump_with_types(ostream&, int) = 0; 
// virtual void cgen(ostream&) = 0;       /* ## */ 



#define program_EXTRAS                          \
InheritanceNodeP semant();            /* ## */  \
void dump_with_types(ostream&, int);            
//void cgen(ostream&);                  /* ## */

#define Class__EXTRAS                   \
virtual Symbol get_name() = 0;  		\
virtual Symbol get_parent() = 0;    	\
virtual Features get_features() = 0;    \
virtual Symbol get_filename() = 0;      \
virtual void dump_with_types(ostream&,int) = 0; 


#define class__EXTRAS                           \
Symbol get_name()   { return name; }            \
Symbol get_parent() { return parent; }          \
Features get_features() { return features; }    \
Symbol get_filename() { return filename; }      \
void dump_with_types(ostream&,int);                    


#define Feature_EXTRAS                                        \
virtual void dump_with_types(ostream&,int) = 0; 			  \
virtual Symbol get_name() = 0;                  /* ## */      \
virtual void tc(EnvironmentP) = 0;              /* ## */      \
virtual void add_to_table(EnvironmentP) = 0;    /* ## */      
//virtual void layout_feature(CgenNodeP) = 0;     /* ## */      
//virtual void code_method(ostream&,CgenEnvClassLevelP) = 0; /* ## */ 


#define Feature_SHARED_EXTRAS                                       \
void dump_with_types(ostream&,int);    										\
Symbol get_name() { return name; }                       /* ## */           \
void tc(EnvironmentP);                                   /* ## */           \
void add_to_table(EnvironmentP);                         /* ## */           
//void layout_feature(CgenNodeP);                          /* ## */           
//void code_method(ostream&, CgenEnvClassLevelP);          /* ## */           


#define method_EXTRAS                                    /* ## */           \
Formals get_formals()     { return formals; }            /* ## */           \
Symbol  get_return_type() { return return_type; }        /* ## */           \
int num_formals()         { return formals->len(); }     /* ## */           \
Formal  sel_formal(int i) { return formals->nth(i); }    /* ## */
//int calc_temps();                                        /* ## */           

#define attr_EXTRAS                                      /* ## */           \
Symbol get_type_decl() { return type_decl; }             /* ## */           
//CalcTempP calc_temps(CalcTempP);                         /* ## */           
//void   code_init(ostream&, CgenEnvironmentP);            /* ## */


#define Formal_EXTRAS                              \
virtual Symbol get_type_decl() = 0;                /* ## */ \
virtual Symbol get_name()      = 0;                /* ## */ \
virtual void install_formal(EnvironmentP env) = 0; /* ## */ \
virtual void dump_with_types(ostream&,int) = 0;


#define formal_EXTRAS                           \
Symbol get_type_decl() { return type_decl; }    /* ## */ \
Symbol get_name()      { return name; }         /* ## */ \
void install_formal(EnvironmentP env);          /* ## */ \
void dump_with_types(ostream&,int);


#define Case_EXTRAS                             \
virtual void dump_with_types(ostream& ,int) = 0;		\
virtual Symbol get_name() = 0;                  /* ## */ \
virtual Symbol get_type_decl() = 0;             /* ## */ \
virtual Expression get_expr() = 0;              /* ## */ \
virtual Symbol tc(EnvironmentP) = 0;            /* ## */ 
//virtual CalcTempP calc_temps(CalcTempP) = 0;    /* ## */ 
//virtual void code(ostream&, CgenEnvironmentP,            
//		  int, Register) = 0;           /* ## */ 


#define branch_EXTRAS                                   \
void dump_with_types(ostream& ,int);						\
Symbol get_name() { return name; }                    /* ## */ \
Symbol get_type_decl() { return type_decl; }          /* ## */  \
Expression get_expr()  { return expr; }               /* ## */  \
Symbol tc(EnvironmentP env) { return expr->tc(env); } /* ## */  
//CalcTempP calc_temps(CalcTempP);                      /* ## */  
//void code(ostream&, CgenEnvironmentP, int, Register); /* ## */ 


#define Expression_EXTRAS                    \
virtual Symbol tc(EnvironmentP) = 0;         /* ## */ \
virtual int no_code() { return 0; }          /* ## */ \
Symbol type;                                 \
Symbol get_type() { return type; }           \
Expression set_type(Symbol s) { type = s; return this; } \
virtual void dump_with_types(ostream&,int) = 0;  \
void dump_type(ostream&, int);               \
Expression_class() { type = (Symbol) NULL; } 
//virtual CalcTempP calc_temps(CalcTempP) = 0; /* ## */ 
//virtual Register code(ostream&, CgenEnvironmentP, Register) = 0;  /* ## */ 


#define Expression_SHARED_EXTRAS           \
void dump_with_types(ostream&,int); 			\
Symbol tc(EnvironmentP);                   /* ## */ 
//CalcTempP calc_temps(CalcTempP);           /* ## */ 
//Register code(ostream&, CgenEnvironmentP, Register);     /* ## */ 


#define no_expr_EXTRAS        /* ## */ \
int no_code() { return 1; }   /* ## */

#endif