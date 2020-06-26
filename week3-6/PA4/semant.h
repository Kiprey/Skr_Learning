#ifndef SEMANT_H_
#define SEMANT_H_

#include <assert.h>
#include "cool-io.h"  //includes iostream; for cout, <<
#include "cool-tree.h"
#include "stringtab.h"
#include "symtab.h"
#include "list.h"

#define TRUE 1
#define FALSE 0

// These are globally defined to prevent compiler warnings about
// anonymous enumerated types.
enum Inheritable   {CanInherit, CantInherit};
enum Basicness     {Basic, NotBasic};
enum Reachability  {Reachable, UnReachable};

//
// The major types in this file (Environment, ClassTable, InheritanceNode)
// cross-reference each other, so all must be declared first to
// allow forward references.
//
class Environment;
typedef Environment *EnvironmentP;
class ClassTable;
typedef ClassTable *ClassTableP;
class InheritanceNode;
typedef InheritanceNode *InheritanceNodeP;

//
// The environment of a COOL  expression can be completely characterized
// by 
//    1. A symbol table describing method bindings.
//    2. A symbol table describing attribute/local variable bindings.
//    3. A symbol table of all classes.
//    4. The class in which the expression occurs (the class of SELF_TYPE)
//
class Environment
{
 private:
    SymbolTable<Symbol, method_class> method_table;
    SymbolTable<Symbol, Entry>  var_table;
    ClassTableP class_table;
    Class_      self_class;

  public:
    Environment(ClassTableP ct, InheritanceNodeP sc);
    Environment(SymbolTable<Symbol, method_class> mt,
		SymbolTable<Symbol, Entry>  vt,
		ClassTableP ct,
		InheritanceNodeP sc);
    EnvironmentP clone_Environment(InheritanceNodeP n);


    // class table operations
    //
    // Standard ways to report errors.
    //
    ostream& semant_error();
    ostream& semant_error(tree_node *t);
    
    InheritanceNodeP lookup_class(Symbol s);

    // method table operations 
    void method_add(Symbol s, method_class *m);
    method_class *method_lookup(Symbol s);
    method_class *method_probe(Symbol s);
    void method_enterscope();
    void method_exitscope();

    // attribute table operations
    void var_add(Symbol s, Symbol typ);
    Symbol var_lookup(Symbol s);
    Symbol var_probe(Symbol s);
    void var_enterscope();
    void var_exitscope();

    // type operations
    Symbol get_self_type();
    int type_leq(Symbol subtype, Symbol supertype);
    Symbol type_lub(Symbol t1, Symbol t2);
};


// A node of the inheritance graph is a Cool class with associated info:
//     1. parent in the inheritance graph
//     2. children " "       "        "
//     3. can inherit/can't inherit from this class
//     4. basic/not basic class
//     5. this class is reachable/unreachable from the Object class
//           via the "inherits from" relation
//     6. a type checking environment
//
class InheritanceNode : public class__class
{
 private:
  InheritanceNodeP parentnd;
  List<InheritanceNode> *children;
  Inheritable  inherit_status;
  Basicness    basic_status;
  Reachability reach_status;
  EnvironmentP env;
  
 public:
  InheritanceNode(Class_ c, Inheritable istatus, Basicness bstatus);
  int basic() { return (basic_status == Basic); }
  int inherit() { return (inherit_status == CanInherit); }
  void mark_reachable();
  int reachable() { return (reach_status == Reachable); }
  void add_child(InheritanceNodeP child);
  List<InheritanceNode> *get_children() { return children; }
  void set_parentnd(InheritanceNodeP p);
  InheritanceNodeP get_parentnd();
  //
  // The type checking environment of class X is established by copying 
  // the environment of X's parent and setting setting the self class to be 
  // X.
  //
  void copy_env(EnvironmentP e) { env = e->clone_Environment(this); }
  void build_feature_tables();
  //
  // For the root Object class, a fresh environment structure is created.
  //
  void init_env(ClassTableP ct);
  void type_check_features();
  void check_main_method();
  method_class *method_lookup(Symbol s) { return env->method_lookup(s); }
};


class ClassTable : public SymbolTable<Symbol, InheritanceNode>
{
//
// Much of the type checker is built into the construction
// of the class table.
//
private:
  List<InheritanceNode> *nds;
  int semant_errors;             // counts the number of semantic errors
  void install_basic_classes();
  void install_class(InheritanceNodeP nd);
  void install_classes(Classes cs);
  void check_improper_inheritance();
  void build_inheritance_tree();
  void set_relations(InheritanceNodeP nd);
  void check_for_cycles();
  void build_feature_tables();
  void check_main();
  ostream& error_stream;

public:
  ClassTable(Classes);
  int errors() { return semant_errors; }
  InheritanceNodeP root();
  ostream& semant_error();
  ostream& semant_error(Class_ c);
  ostream& semant_error(Symbol filename, tree_node *t);
};



#endif

