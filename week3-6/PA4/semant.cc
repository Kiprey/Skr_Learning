//   This file implements the semantic checks for Cool.  There are three
//   passes:
// 
//   Pass 1: This is not a true pass, as only the classes are inspected.
//   The inheritance graph is built and checked for errors.  There are
//   two "sub"-passes: check that classes are not redefined and inherit
//   only from defined classes, and check for cycles in the inheritance
//   graph.  Compilation is halted if an error is detected between the
//   sub-passes.
//
//   Pass 2: Symbol tables are built for each class.  This step is done
//   separately because methods and attributes have global
//   scope---therefore, bindings for all methods and attributes must be
//   known before type checking can be done.
// 
//   Pass 3: The inheritance graph---which is known to be a tree if
//   there are no cycles---is traversed again, starting from the root
//   class Object.  For each class, each attribute and method is
//   typechecked.  Simultaneously, identifiers are checked for correct
//   definition/use and for multiple definitions.  An invariant is
//   maintained that all parents of a class are checked before a class
//   is checked.


#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "semant.h"
#include "utilities.h"

extern int semant_debug;
extern char *curr_filename;

extern int node_lineno;

//////////////////////////////////////////////////////////////////////
//
// Symbols
//
// For convenience, a large number of symbols are predefined here.
// These symbols include the primitive type and method names, as well
// as fixed names used by the runtime system.
//
//////////////////////////////////////////////////////////////////////
static Symbol 
       arg,
       arg2,
       Bool,
       concat,
       cool_abort,
       copy,
       Int,
       in_int,
       in_string,
       IO,
       isProto,
       length,
       Main,
       main_meth,
       No_class,
       No_type,
       Object,
       out_int,
       out_string,
       prim_slot,
       self,
       SELF_TYPE,
       Str,
       str_field,
       substr,
       type_name,
       val;
//
// Initializing the predefined symbols.
//
static void initialize_constants(void)
{
  arg         = idtable.add_string("arg");
  arg2        = idtable.add_string("arg2");
  Bool        = idtable.add_string("Bool");
  concat      = idtable.add_string("concat");
  cool_abort  = idtable.add_string("abort");
  copy        = idtable.add_string("copy");
  Int         = idtable.add_string("Int");
  in_int      = idtable.add_string("in_int");
  in_string   = idtable.add_string("in_string");
  IO          = idtable.add_string("IO");
  isProto     = idtable.add_string("isProto");
  length      = idtable.add_string("length");
  Main        = idtable.add_string("Main");
  main_meth   = idtable.add_string("main");
//   _no_class is a symbol that can't be the name of any 
//   user-defined class.
  No_class    = idtable.add_string("_no_class");
  No_type     = idtable.add_string("_no_type");
  Object      = idtable.add_string("Object");
  out_int     = idtable.add_string("out_int");
  out_string  = idtable.add_string("out_string");
  prim_slot   = idtable.add_string("_prim_slot");
  self        = idtable.add_string("self");
  SELF_TYPE   = idtable.add_string("SELF_TYPE");
  Str         = idtable.add_string("String");
  str_field   = idtable.add_string("_str_field");
  substr      = idtable.add_string("substr");
  type_name   = idtable.add_string("type_name");
  val         = idtable.add_string("_val");
}

///////////////////////////////////////////////////////////////////////
//
//  Enviroment functions.
//
//  The Cool type checking rules require four structures to typecheck a 
//  class X.  These four items are encapsulated in an Environment:
//
//     a) a mapping from method names to method definitions for X
//     b) a mapping from variable (local and attribute) names to
//         definitions in X
//     c) a mapping from method names and class names to defintions
//         for methods of classes other than X
//     d) the self class (X)
//
//     c) is realized using a class_table, which contains a mapping
//        from class names to InheritanceNodes (and thereby to Environments)
//        for all classes.
//
//////////////////////////////////////////////////////////////////////

Environment::Environment(ClassTableP ct, InheritanceNodeP sc): 
   method_table(*(new SymbolTable<Symbol, method_class>())),
   var_table(*(new SymbolTable<Symbol, Entry>())),
   class_table(ct),
   self_class(sc)
{ 
  method_table.enterscope();
  var_table.enterscope();
  var_table.addid(self,SELF_TYPE);  // self : SELF_TYPE in all environments
}

Environment::Environment(SymbolTable<Symbol, method_class> mt,
			 SymbolTable<Symbol, Entry>       vt,
			 ClassTableP ct,
			 InheritanceNodeP sc):
		  method_table(mt), // copies the method_table
		  var_table(vt),    // copies the var_table
                  class_table(ct),
		  self_class(sc)
{ 
  // push a new scope for each of the method/variable tables,
  // so that new methods/variables will not conflict with existing ones.
  method_table.enterscope();
  var_table.enterscope();
}

//
// Most "new" environments are duplicates of a parent class environment
// with the self class replaced.  An small but important point is that
// the method table and var table structures are copied, so that 
// additions to the new environment have no effect on the original.
//
EnvironmentP Environment::clone_Environment(InheritanceNodeP n) 
{ return new Environment(method_table,var_table,class_table,n); }

ostream& Environment::semant_error()
{ return class_table->semant_error(); }

ostream& Environment::semant_error(tree_node *t) 
{ return class_table->semant_error(self_class->get_filename(),t); }

InheritanceNodeP Environment::lookup_class(Symbol s) 
{ return class_table->probe(s); }

void Environment::method_add(Symbol s, method_class *m) 
{ method_table.addid(s,m); }

method_class *Environment::method_lookup(Symbol s) 
{ return method_table.lookup(s); }

method_class *Environment::method_probe(Symbol s) 
{ return method_table.probe(s); }

void Environment::method_enterscope() { method_table.enterscope(); }
void Environment::method_exitscope() { method_table.exitscope(); }

void Environment::var_add(Symbol s, Symbol v) 
{ var_table.addid(s,v); }

Symbol Environment::var_lookup(Symbol s) 
{ return var_table.lookup(s); }

Symbol Environment::var_probe(Symbol s) 
{ return var_table.probe(s); }

void Environment::var_enterscope() { var_table.enterscope(); }
void Environment::var_exitscope() { var_table.exitscope(); }

Symbol Environment::get_self_type() 
{ return self_class->get_name(); }

////////////////////////////////////////////////////////////////////
//
// semant_error is an overloaded function for reporting errors
// during semantic analysis.  There are three versions:
//
//    ostream& ClassTable::semant_error()                
//
//    ostream& ClassTable::semant_error(Class_ c)
//       print line number and filename for `c'
//
//    ostream& ClassTable::semant_error(Symbol filename, tree_node *t)  
//       print a line number and filename 
//       (line number is extracted from tree_node)
//
///////////////////////////////////////////////////////////////////

ostream& ClassTable::semant_error(Class_ c)
{                                                             
  return semant_error(c->get_filename(),c);
}    

ostream& ClassTable::semant_error(Symbol filename, tree_node *t)
{
  error_stream << filename << ":" << t->get_line_number() << ": ";
  return semant_error();
}

ostream& ClassTable::semant_error()                  
{                                                 
    semant_errors++;                            
    return error_stream;
} 

/////////////////////////////////////////////////////////////////////
// 
// The inheritance graph.
//
// The following functions allow nodes of the inheritance graph to
// be created, and parent nodes to be set/retrieved from nodes.
//
////////////////////////////////////////////////////////////////////
 

InheritanceNode::InheritanceNode(Class_ nd, 
				 Inheritable istatus,
		                 Basicness bstatus):
				 // Use of the copy constructor is important
				 // here.
				 class__class((const class__class &) *nd),
				 parentnd(NULL),
				 children(NULL),
				 inherit_status(istatus),
				 basic_status(bstatus),
				 reach_status(UnReachable),
				 env(NULL)
{ }

void InheritanceNode::set_parentnd(InheritanceNodeP p)
{
  assert(parentnd == NULL);
  assert(p != NULL);
  parentnd = p;
}

InheritanceNodeP InheritanceNode::get_parentnd()
{
  return parentnd;
}

////////////////////////////////////////////////////////////////////
//
// ClassTable::ClassTable
//
// The ClassTable constructor initializes a symbol table mapping
// class names to inheritance graph nodes.  The constructor also
// installs the basic classes.
//
// Cool has five basic classes: 
//   Object:     The root of the hierarchy; all objects are Objects.
//   IO:         A class for simple string and integer input/output.
//   Int:        Integers
//   Bool:       Booleans
//   String:     Strings
//
//   User-defined classes may inherit from Object and IO, but the
//   Int, Bool, and String classes cannot be inherited.
//
/////////////////////////////////////////////////////////////////////
ClassTable::ClassTable(Classes classes) : nds(NULL), 
                                          semant_errors(0),
                                          error_stream(cerr)
{
  enterscope();             // initially the symbol table is empty
  install_basic_classes();  // predefined basic classes
  if (semant_debug)  cerr << "Installed basic classes." << endl;
  install_classes(classes); // user defined classes
  if (semant_debug) 
    { cerr << "Installed user-defined classes" << endl; dump(); }
  check_improper_inheritance();  // check for simple inheritance mistakes
  if (semant_debug) { cerr << "Checked for simple inheritance errors." << endl; }
  if (errors()) return;

  build_inheritance_tree(); // build the full inheritance tree
  if (semant_debug) { cerr << "Built inheritance tree." << endl; }
  root()->mark_reachable(); // find all classes reachable from root class
  if (semant_debug) { cerr << "Marked reachable classes." << endl; }
  check_for_cycles();       // check that it really is a tree
  if (semant_debug) { cerr << "Checked for cycles." << endl; }
  if (errors()) return;

  build_feature_tables();   // build symbol tables of features for each class
  if (semant_debug) { cerr << "Built feature tables." << endl; }
  check_main();             // check for Main class and main method
  if (semant_debug) { cerr << "Checked Main class and method." << endl; }
  root()->type_check_features(); // type check all expressions
}

void ClassTable::install_basic_classes()
{

// The tree package uses these globals to annotate the classes built below.
  node_lineno = 0;
  Symbol filename = stringtable.add_string("<basic class>");

//
// A few special class names are installed in the lookup table but not
// the class list.  Thus, these classes exist, but are not part of the
// inheritance hierarchy.
// No_class serves as the parent of Object and the other special classes.
// SELF_TYPE is the self class; it cannot be redefined or inherited.
// prim_slot is a class known to the code generator.
//
  addid(No_class,
	new InheritanceNode(class_(No_class,No_class,nil_Features(),filename),
			    CanInherit,
			    Basic));
  addid(SELF_TYPE,
	new InheritanceNode(class_(SELF_TYPE,No_class,nil_Features(),filename),
			    CantInherit,
			    Basic));
  addid(prim_slot,
	new InheritanceNode(class_(prim_slot,No_class,nil_Features(),filename),
			    CantInherit,
			    Basic));

// 
// The Object class has no parent class. Its methods are
//        cool_abort() : Object    aborts the program
//        type_name() : Str        returns a string representation of class name
//        copy() : SELF_TYPE       returns a copy of the object
//
// There is no need for method bodies in the basic classes---these
// are already built in to the runtime system.
//

  Class_ Object_class =
    class_(Object, 
	   No_class,
	   append_Features(
           append_Features(
           single_Features(method(cool_abort, nil_Formals(), Object, no_expr())),
           single_Features(method(type_name, nil_Formals(), Str, no_expr()))),
           single_Features(method(copy, nil_Formals(), SELF_TYPE, no_expr()))),
	   filename);

  install_class(new InheritanceNode(Object_class, CanInherit, Basic));

// 
// The IO class inherits from Object. Its methods are
//        out_string(Str) : SELF_TYPE          writes a string to the output
//        out_int(Int) : SELF_TYPE               "    an int    "  "     "
//        in_string() : Str                    reads a string from the input
//        in_int() : Int                         "   an int     "  "     "
//

  Class_ IO_class = 
     class_(IO, 
            Object,
            append_Features(
            append_Features(
            append_Features(
            single_Features(method(out_string, single_Formals(formal(arg, Str)),
                        SELF_TYPE, no_expr())),
            single_Features(method(out_int, single_Formals(formal(arg, Int)),
                        SELF_TYPE, no_expr()))),
            single_Features(method(in_string, nil_Formals(), Str, no_expr()))),
            single_Features(method(in_int, nil_Formals(), Int, no_expr()))),
	    filename);

  install_class(new InheritanceNode(IO_class, CanInherit, Basic));

//
// The Int class has no methods and only a single attribute, the
// "val" for the integer. 
//

  Class_ Int_class =  
      class_(Int, 
	     Object,
	     single_Features(attr(val, prim_slot, no_expr())),
	     filename);

  install_class(new InheritanceNode(Int_class, CantInherit, Basic));

//
// Bool also has only the "val" slot.
//
  
  Class_ Bool_class = 
      class_(Bool, Object, single_Features(attr(val, prim_slot, no_expr())),filename);

  install_class(new InheritanceNode(Bool_class, CantInherit, Basic));

//
// The class Str has a number of slots and operations:
//       val                                  the length of the string
//       str_field                            the string itself
//       length() : Int                       returns length of the string
//       concat(arg: Str) : Str               performs string concatenation
//       substr(arg: Int, arg2: Int): Str     substring selection
//       

  Class_ Str_class = 
      class_(Str, 
	     Object,
             append_Features(
             append_Features(
             append_Features(
             append_Features(
             single_Features(attr(val, Int, no_expr())),
            single_Features(attr(str_field, prim_slot, no_expr()))),
            single_Features(method(length, nil_Formals(), Int, no_expr()))),
            single_Features(method(concat, 
				   single_Formals(formal(arg, Str)),
				   Str, 
				   no_expr()))),
	    single_Features(method(substr, 
				   append_Formals(single_Formals(formal(arg, Int)), 
						  single_Formals(formal(arg2, Int))),
				   Str, 
				   no_expr()))),
	     filename);

   install_class(new InheritanceNode(Str_class, CantInherit, Basic));

// Some debugging
//    Object_class->dump_with_types(cerr, 0);
//    IO_class->dump_with_types(cerr, 0);
//    Int_class->dump_with_types(cerr, 0);
//    Bool_class->dump_with_types(cerr, 0);
//    Str_class->dump_with_types(cerr, 0);
}

// ClassTable::install_class
// ClassTable::install_classes
//
// install_classes enters a list of classes in the symbol table.
// The following possible errors are checked:
//       - a class called SELF_TYPE
//       - redefinition of a basic class
//       - redefinition of another previously defined class
//
void ClassTable::install_class(InheritanceNodeP nd)
{
  Symbol name = nd->get_name();

  if (probe(name))
    {
      InheritanceNodeP old = probe(name);
      if (old->basic())
	    semant_error(nd) << "Redefinition of basic class " << name << "."<< endl;
      else
	    semant_error(nd) << "Class " << name << " was previously defined." 
	  << endl;
      return;
    }

  // The class name is legal, so add it to the list of classes
  // and the symbol table.
  nds = new List<InheritanceNode>(nd,nds);
  addid(name,nd);
}

//
// Install all user-defined classes; all of these classes can
// be inherited and are not basic classes.
//
void ClassTable::install_classes(Classes cs)
{
  for(int i = cs->first(); cs->more(i); i = cs->next(i))
    install_class(new InheritanceNode(cs->nth(i),CanInherit,NotBasic));
}

//
// ClassTable::check_improper_inheritance
//
// This function checks whether the classes in a ClassTable illegally inherit
// from
//  - a CantInherit class
//  - SELF_TYPE
//  - an undefined class
//
// All of these checks are local (do not require traversing the inheritance
// graph).
//
void ClassTable::check_improper_inheritance()
{
  for(List<InheritanceNode> *l = nds; l; l = l->tl())
    {
      InheritanceNodeP c    = l->hd();
      Symbol parent         = c->get_parent();
      InheritanceNode *node = probe(parent);

      if (!node)
	 { semant_error(c) << "Class " << c->get_name() << 
	    " inherits from an undefined class " << parent << "." << endl; 
	  continue; }

      if (!node->inherit())
	semant_error(c) << "Class " << c->get_name() <<
	  " cannot inherit class " << parent << "." << endl;
    }
}  


//
// ClassTable::build_inheritance_tree
//
// For each class node in the inheritance graph, set its parent,
// and add the node to the parent's list of child nodes.
//
void ClassTable::build_inheritance_tree()
{
  for(List<InheritanceNode> *l = nds; l; l = l->tl())
      set_relations(l->hd());
}

//
// ClassTable::set_relations
//
// Takes a InheritanceNode and locates its, and its parent's, inheritance nodes
// via the class table.  Parent and child pointers are added as appropriate.
//
void ClassTable::set_relations(InheritanceNodeP nd)
{
  InheritanceNode *parent_node = probe(nd->get_parent());
  nd->set_parentnd(parent_node);
  parent_node->add_child(nd);
}

//
// This method should be run only after mark_reachable has executed.
// If there are any unreachable classes in the inheritance graph and
// all of the local checks of check_improper_inheritance succeeded,
// then there is a cycle in the inheritance graph.
//
void ClassTable::check_for_cycles()
{
  for(List<InheritanceNode> *l = nds; l; l = l->tl())
    if(!(l->hd()->reachable()))
      semant_error(l->hd()) << "Class " << l->hd()->get_name() << 
	", or an ancestor of " << l->hd()->get_name() << 
	  ", is involved in an inheritance cycle." << endl;

}

void InheritanceNode::add_child(InheritanceNodeP n)
{
  children = new List<InheritanceNode>(n,children);
}

//
// InheritanceNode::mark_reachable()
// Recursively mark all nodes reachable from the argument as Reachable.
// Initally called with Object.
//
// This function is guaranteed to terminate because the subgraph of the
// inheritance hierarchy reachable from Object cannot contain a cycle
// if no local errors are discovered by check_improper_inheritance.
//
void InheritanceNode::mark_reachable()
{
  reach_status = Reachable;      // mark the current node as reachable
  // process the children
  for(List<InheritanceNode> *kids = children; kids; kids = kids->tl())
    kids->hd()->mark_reachable();
}

//////////////////////////////////////////////////////////////////////////
//
// Feature Symbol Tables
//
// The following group of functions recursively walk each feature
// of each class, adding information to the environment for the classes
// about the features.  Errors such as redefining method/attribute names
// within a class are caught here.
//
//////////////////////////////////////////////////////////////////////////

void InheritanceNode::build_feature_tables()
{
  // add each feature of the class to the class symbol table
  for(int i = features->first(); features->more(i); i = features->next(i))
    features->nth(i)->add_to_table(env);

  for(List<InheritanceNode> *l = children; l; l = l->tl())
    {
      // for each child of the current class, we
      l->hd()->copy_env(env);       // copy the parent's environment,
                                    // thus inheriting the parent's features;
      l->hd()->build_feature_tables(); // add the child's features 
    }
}

void InheritanceNode::type_check_features()
{
  if (semant_debug) { cerr << "Type checking class " << name << endl;}

  for(int i = features->first(); features->more(i); i = features->next(i))
    features->nth(i)->tc(env);

  for(List<InheritanceNode> *l = children; l; l = l->tl())
      l->hd()->type_check_features();
}

//
// Allocate new Environment structure.  Presently used only for the
// root (Object) class; all other classes make a copy of their parent's
// Environment.
//
void InheritanceNode::init_env(ClassTableP ct)
{
  env = new Environment(ct,this);
}

void ClassTable::build_feature_tables()
{
  root()->init_env(this);          // create symbol tables for the root class
  root()->build_feature_tables();  // recursively build feature tables for
                                   // the root class and all descendants.
}

InheritanceNodeP ClassTable::root()
{
  return probe(Object);
}

void method_class::add_to_table(EnvironmentP env)
{
  // 查找cool当前类中是否有重名函数
  if (env->method_probe(name))
    { env->semant_error(this) << "Method " << name << " is multiply defined." 
	<< endl;
      return; }
  // 查找继承类中是否有同名方法（即函数重写）
  method_class *old = env->method_lookup(name);
  if(old)
    {
      if (old->get_return_type() != return_type)
	{ env->semant_error(this) << "In redefined method " << name << 
	    ", return type " << return_type << 
	      " is different from original return type " << 
		old->get_return_type() << "." << endl;
	  return; }
      
      if (old->num_formals() != num_formals())
	{ env->semant_error(this) << 
	    "Incompatible number of formal parameters in redefined method " << 
	      name << "." << endl;
	  return; }
        
      Formals old_formals = old->get_formals();
      for(int i = formals->first(); formals->more(i); i = formals->next(i))
	if (old_formals->nth(i)->get_type_decl() != formals->nth(i)->get_type_decl())
	  { env->semant_error(this) << "In redefined method " << name << 
	      ", parameter type " << formals->nth(i)->get_type_decl() << 
		" is different from original type " << 
		  old_formals->nth(i)->get_type_decl() << endl;
	    return; }
    }
     env->method_add(name,this);
}

void attr_class::add_to_table(EnvironmentP env)
{
  if (name == self)
    { env->semant_error(this) << "'self' cannot be the name of an attribute." 
	<< endl;
      return; }
  // 查找当前类中是否有重名变量
  if (env->var_probe(name))
    { env->semant_error(this) << "Attribute " << name << 
	" is multiply defined in class." << endl; 
      return; }
  // 查找祖先类中是否有重名变量
  if (env->var_lookup(name))
    { env->semant_error(this) << "Attribute " << name << 
	" is an attribute of an inherited class." << endl;
      return; }

  env->var_add(name,type_decl);
}

//
// Checks for the existence of the Main class, and that it has access to a
// method main in its inheritance hierarchy.
//
void ClassTable::check_main()
{
  InheritanceNodeP mainclass = probe(Main);
  if(!mainclass)
    semant_error() << "Class Main is not defined." << endl; 
  else
    mainclass->check_main_method();
}

//
// Checks that the class has a main method that takes no arguments.
// The method must be defied *in*, not inherited into, the Main class.
//
void InheritanceNode::check_main_method()
{
  if (!env->method_probe(main_meth))
  {
    env->semant_error(this) << "No 'main' method in class Main." << endl;
    return;
  }
  if (env->method_lookup(main_meth)->num_formals() != 0)
    env->semant_error(this) << 
      "'main' method in class Main should have no arguments." << endl;
}

////////////////////////////////////////////////////////////////////////
//
// Type Operations
//
// type_leq   Is type X <= type Y?
// type_lub   What is the most specific type greater than both X and Y?
//
// These functions are complicated slightly by the handling of SELF_TYPE
// and by taking care to avoid generating multiple error messages for
// undefined classes (which have already been reported by the time
// these functions are used).
//
/////////////////////////////////////////////////////////////////////////

int Environment::type_leq(Symbol subtype, Symbol supertype)
{
  // If one of the classes doesn't exist, return TRUE to cut down on the
  // the number of spurious error messages.  Also provides 
  // No_type < t for any t.
  //
  if(!(lookup_class(supertype) && lookup_class(subtype)))
    return TRUE;
 
  // SELF_TYPE <= SELF_TYPE
  if (subtype == SELF_TYPE && supertype == SELF_TYPE) return TRUE;

  // X is not <= SELF_TYPE if X is not SELF_TYPE
  if (supertype == SELF_TYPE) return FALSE;

  // if the lhs is SELF_TYPE, it is promoted here to the self_type of the class.
  if (subtype == SELF_TYPE) subtype = get_self_type();

  // X <= Y if Y is an ancestor of X in the inheritance hierarchy.

  InheritanceNodeP y = lookup_class(supertype);
  for(InheritanceNodeP x = lookup_class(subtype); x; x = x->get_parentnd())
    if(x == y) return TRUE;

  return FALSE;
}

// 返回type1和type2共同的祖先
Symbol Environment::type_lub(Symbol type1, Symbol type2)
{
  // 
  // The order of tests in the this procedure is important.
  //
  // 1. If one type is undefined (i.e., No_class), return the other
  // 2. If both types are SELF_TYPE, return SELF_TYPE
  // 3. If either is SELF_TYPE, convert it to the type of the class
  // 4. Find the least common ancestor in the inheritance graph.
  //
  if (!lookup_class(type1)) return type2;  // if either type is undefined,
  if (!lookup_class(type2)) return type1;  // return the other. 

  if (type1 == type2)     return type1;   // SELF_TYPE u SELF_TYPE = SELF_TYPE
  if (type1 == SELF_TYPE) type1 = get_self_type();
  if (type2 == SELF_TYPE) type2 = get_self_type();
  
  InheritanceNodeP nd;
  for(nd = lookup_class(type1); 
      !type_leq(type2,nd->get_name()); 
      nd = nd->get_parentnd())
      ;
    return nd->get_name(); 
}


///////////////////////////////////////////////////////////////////////////////
//
//  Type Checking Features
//
//  For each class of expression, there is a tc method to typecheck it.
//  The tc methods make use of the environments previously constructred
//  for each class.  This code parallels the structure of the type
//  inference rules in the CoolAid very closely.
//
///////////////////////////////////////////////////////////////////////////////
void attr_class::tc(EnvironmentP env)
{
  if (! env->lookup_class(type_decl))
    env->semant_error(this) << "Class " << type_decl << " of attribute " << name
      << " is undefined." << endl;

  if (! env->type_leq(init->tc(env), type_decl))
    env->semant_error(this) << "Inferred type " << init->get_type() <<
      " of initialization of attribute " << name << 
	" does not conform to declared type " << type_decl << "." << endl;

}

void method_class::tc(EnvironmentP env)
{
  env->var_enterscope();

  for(int i = formals->first(); formals->more(i); i = formals->next(i))
    formals->nth(i)->install_formal(env);

  if (! env->lookup_class(return_type))
    env->semant_error(this) << "Undefined return type " << return_type << 
      " in method " << name << "." << endl;

  if(! env->type_leq(expr->tc(env), return_type))
     env->semant_error(this) << "Inferred return type " << expr->get_type() <<
       " of method " << name << " does not conform to declared return type " <<
	 return_type << "." << endl;

  env->var_exitscope();
}

void formal_class::install_formal(EnvironmentP env)
{
  if (type_decl == SELF_TYPE)
    env->semant_error(this) << "Formal parameter " << name << 
      " cannot have type SELF_TYPE." << endl;
  else
    { if (! env->lookup_class(type_decl))
	env->semant_error(this) << "Class " << type_decl << 
	  " of formal parameter " << name << " is undefined." << endl; };

  if (name == self)
    { env->semant_error(this) << 
	"'self' cannot be the name of a formal parameter." << endl;
      return; }

  if(env->var_probe(name))
    { env->semant_error(this) << "Formal parameter " << name << 
	" is multiply defined." << endl;
      return; }

  env->var_add(name,type_decl);
}

Symbol int_const_class::tc(EnvironmentP)
{
  type = Int;
  return Int;
}

Symbol bool_const_class::tc(EnvironmentP)
{
  type = Bool;
  return Bool;
}
 
Symbol string_const_class::tc(EnvironmentP)
{
  type = Str;
  return Str;
}
 
Symbol plus_class::tc(EnvironmentP env)
{
  e1->tc(env);
  e2->tc(env);

  if ((e1->get_type() != Int) || (e2->get_type() != Int))
    env->semant_error(this) << "non-Int arguments: " << e1->get_type() << " + "
      << e2->get_type() << endl;

  type = Int;
  return Int;
}

Symbol sub_class::tc(EnvironmentP env)
{
  e1->tc(env);
  e2->tc(env);

  if ((e1->get_type() != Int) || (e2->get_type() != Int))
    env->semant_error(this) << "non-Int arguments: " << e1->get_type() << " - "
      << e2->get_type() << endl;

  type = Int;
  return Int;
}
 

Symbol mul_class::tc(EnvironmentP env)
{
  e1->tc(env);
  e2->tc(env);

  if ((e1->get_type() != Int) || (e2->get_type() != Int))
    env->semant_error(this) << "non-Int arguments: " << e1->get_type() << " * "
      << e2->get_type() << endl;

  type = Int;
  return Int;
}

Symbol divide_class::tc(EnvironmentP env)
{
  e1->tc(env);
  e2->tc(env);

  if ((e1->get_type() != Int) || (e2->get_type() != Int))
    env->semant_error(this) << "non-Int arguments: " << e1->get_type() << " / "
      << e2->get_type() << endl;

  type = Int;
  return Int;
}
 
Symbol neg_class::tc(EnvironmentP env)
{
  e1->tc(env);

  if(e1->get_type() != Int)
    env->semant_error(this) << "Argument of '~' has type " << e1->get_type()
      << " instead of Int." << endl;

  type = Int;
  return Int;
}

Symbol lt_class::tc(EnvironmentP env)
{
  e1->tc(env);
  e2->tc(env);

  if ((e1->get_type() != Int) || (e2->get_type() != Int))
    env->semant_error(this) << "non-Int arguments: " << e1->get_type() << " < "
      << e2->get_type() << endl;

  type = Bool;
  return Bool;
}
 
Symbol leq_class::tc(EnvironmentP env)
{
  e1->tc(env);
  e2->tc(env);

  if ((e1->get_type() != Int) || (e2->get_type() != Int))
    env->semant_error(this) << "non-Int arguments: " << e1->get_type() << " <= "
      << e2->get_type() << endl;

  type = Bool;
  return Bool;
}

Symbol comp_class::tc(EnvironmentP env)
{
  e1->tc(env);

  if(e1->get_type() != Bool)
    env->semant_error(this) << "Argument of 'not' has type " << e1->get_type()
      << " instead of Bool." << endl; 

  type = Bool;
  return Bool;
}
 
Symbol object_class::tc(EnvironmentP env)			
{
  if (env->var_lookup(name))
    type = env->var_lookup(name);
  else
    {
      env->semant_error(this) << "Undeclared identifier " << name << "." << endl;
      type = Object;
    }
  return type;
}

Symbol no_expr_class::tc(EnvironmentP)
{
  type = No_type;
  return No_type;
}
 
Symbol new__class::tc(EnvironmentP env)
{
  if(env->lookup_class(type_name)) 
    type = type_name;
  else
    {
      env->semant_error(this) << "'new' used with undefined class " << 
	type_name << "." << endl;
      type = Object;
    }
  return type;
}

Symbol isvoid_class::tc(EnvironmentP env)
{
  e1->tc(env);
  type = Bool;
  return Bool;
}

Symbol eq_class::tc(EnvironmentP env)
{
  Symbol t1 = e1->tc(env);
  Symbol t2 = e2->tc(env);
  if ((t1 != t2) &&
     ((t1 == Int)  || (t2 == Int) ||
      (t1 == Bool) || (t2 == Bool) ||
      (t1 == Str)  || (t2 == Str)))
       env->semant_error(this) << "Illegal comparison with a basic type." << endl;
  type = Bool;
  return Bool;
}
 
Symbol let_class::tc(EnvironmentP env)
{

  if (! env->lookup_class(type_decl))
    env->semant_error(this) << "Class " << type_decl << 
      " of let-bound identifier " << identifier << " is undefined." << endl;

  if (! env->type_leq(init->tc(env), type_decl))
    env->semant_error(this) << "Inferred type " << init->get_type() <<
    " of initialization of " << identifier << 
    " does not conform to identifier's declared type " << type_decl << "." << 
      endl;
  env->var_enterscope();

  if(identifier == self)
    env->semant_error(this) << "'self' cannot be bound in a 'let' expression." 
      << endl;
  else
    env->var_add(identifier,type_decl);

  type = body->tc(env);
  env->var_exitscope();
  return type;
}
 
Symbol block_class::tc(EnvironmentP env)
{
  for(int i = body->first(); body->more(i); i = body->next(i))
    type = body->nth(i)->tc(env);
  return type;
}

Symbol assign_class::tc(EnvironmentP env)
{
  if (name == self)
    env->semant_error(this) << "Cannot assign to 'self'." << endl;

  if (! env->var_lookup(name))
    env->semant_error(this) << "Assignment to undeclared variable " << name
      << "." << endl;

  type = expr->tc(env);

  if(! env->type_leq(type, env->var_lookup(name)))
    env->semant_error(this) << "Type " << type << 
      " of assigned expression does not conform to declared type " <<
	env->var_lookup(name) << " of identifier " << name << "." << endl;

  return type;
}

Symbol dispatch_class::tc(EnvironmentP env)
{
  // Type check the subexpressions first.
  Symbol expr_type = expr->tc(env);  
  if (expr_type == SELF_TYPE) expr_type = env->get_self_type();

  for(int i = actual->first(); actual->more(i); i = actual->next(i))
    actual->nth(i)->tc(env);

  InheritanceNode *nd = env->lookup_class(expr_type);
  if (!nd)
    {
      env->semant_error(this) << "Dispatch on undefined class " << expr_type <<
	"." << endl; 
      type = Object;
      return Object;
    }
  
  method_class *meth = nd->method_lookup(name);

  if(! meth)
  {
    env->semant_error(this) << "Dispatch to undefined method " << name << "."
      << endl;
    type = Object;
    return Object;
  }

  if(actual->len() != meth->num_formals())
    env->semant_error(this) << "Method " << name << 
		      " called with wrong number of arguments." << endl;
  else
    for(int i = actual->first(); actual->more(i); i = actual->next(i))
      if (! env->type_leq(actual->nth(i)->get_type(),
			  meth->sel_formal(i)->get_type_decl()))
	env->semant_error(this) << "In call of method " << name << 
	  ", type " << actual->nth(i)->get_type() <<
	  " of parameter " << meth->sel_formal(i)->get_name() <<
	    " does not conform to declared type " <<
	      meth->sel_formal(i)->get_type_decl() << "." << endl;

  type = (meth->get_return_type() == SELF_TYPE) ? expr->get_type() : 
                                                  meth->get_return_type();
  return type;
}

Symbol static_dispatch_class::tc(EnvironmentP env)
{
  Symbol expr_type = expr->tc(env);
  for(int i = actual->first(); actual->more(i); i = actual->next(i))
    actual->nth(i)->tc(env);
 
  if(type_name == SELF_TYPE)
  {
    env->semant_error(this) << "Static dispatch to SELF_TYPE." << endl;
    type = Object;
    return Object;
  }

  InheritanceNode *nd = env->lookup_class(type_name);

  if(!nd)
  {
    env->semant_error(this) << "Static dispatch to undefined class " << 
      type_name << "." << endl;
    type = Object;
    return Object;
  }

  if(! env->type_leq(expr_type, type_name))
  {
    env->semant_error(this) << "Expression type " << expr_type <<
      " does not conform to declared static dispatch type " << type_name <<
	"." << endl;
    type = Object;
    return Object;
  }

  method_class *meth = nd->method_lookup(name);
 
  if(! meth)
  {
    env->semant_error(this) << "Static dispatch to undefined method " << name 
      << "." << endl;
    type = Object;
    return Object;
  }
 
  if(actual->len() != meth->num_formals())
    env->semant_error(this) << "Method " << name 
      << " invoked with wrong number of arguments." << endl;
  else    
    for(int i = actual->first(); actual->more(i); i = actual->next(i))
      if (! env->type_leq(actual->nth(i)->get_type(),
			  meth->sel_formal(i)->get_type_decl()))
	env->semant_error(this) << "In call of method " << name << 
	  ", type " << actual->nth(i)->get_type() <<
	  " of parameter " << meth->sel_formal(i)->get_name() <<
	    " does not conform to declared type " <<
	      meth->sel_formal(i)->get_type_decl() << "." << endl;

  type = (meth->get_return_type() == SELF_TYPE) ? expr_type : 
                                                  meth->get_return_type();
  return type;
}
 
Symbol cond_class::tc(EnvironmentP env)
{
  if(pred->tc(env) != Bool)
    env->semant_error(this) << "Predicate of 'if' does not have type Bool.\n";
  Symbol then_type = then_exp->tc(env);
  Symbol else_type = else_exp->tc(env);
  type = env->type_lub(then_type, else_type);
  return type;
}
 
Symbol loop_class::tc(EnvironmentP env)
{
  if(pred->tc(env) != Bool)
    env->semant_error(this) << "Loop condition does not have type Bool." << endl;
  body->tc(env);
  type = Object;
  return Object;
}

Symbol typcase_class::tc(EnvironmentP env)
{
  type = No_type;
  expr->tc(env);

  // 判断是否有重复声明
  for (int i=cases->first(); cases->more(i); i = cases->next(i))
  {
    Case c = cases->nth(i);
    for(int j=cases->first(); j<i; j = cases->next(j))
    {
      if(cases->nth(j)->get_type_decl() == c->get_type_decl())
      {
        env->semant_error(c) << "Duplicate branch " << c->get_type_decl() << 
	  " in case statement." << endl;
        break;
      }
    }
    
    env->var_enterscope();
    if (! env->lookup_class(c->get_type_decl()))
      env->semant_error(c) << "Class " << c->get_type_decl() << 
	" of case branch is undefined." << endl;

    if (c->get_name() == self)
      env->semant_error(c) << "'self' bound in 'case'." << endl;
    
    if (c->get_type_decl() == SELF_TYPE)
      env->semant_error(c) << "Identifier " << c->get_name() << 
	" declared with type SELF_TYPE in case branch." << endl;
    
    env->var_add(c->get_name(), c->get_type_decl());

    type = env->type_lub(type, c->tc(env));
    env->var_exitscope();
  }
  return type;
}

                                            
// The function which runs the semantic analyser.
InheritanceNodeP program_class::semant()
{
   initialize_constants();
   ClassTableP classtable = new ClassTable(classes);

   if (classtable->errors()) {
      cerr << "Compilation halted due to static semantic errors." << endl;
      exit(1);
   }

   return classtable->root();
}
