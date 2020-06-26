#include <assert.h>
#include <stdio.h>
#include "emit.h"
#include "cool-tree.h"
#include "symtab.h"

enum Basicness     {Basic, NotBasic};
#define TRUE 1
#define FALSE 0

class CgenClassTable;
typedef CgenClassTable *CgenClassTableP;

class CgenNode;
typedef CgenNode *CgenNodeP;

// Environmets are organized as a hierarchy of three classes:
class CgenEnvTopLevel;
typedef CgenEnvTopLevel *CgenEnvTopLevelP;

class CgenEnvClassLevel;
typedef CgenEnvClassLevel *CgenEnvClassLevelP;

class CgenEnvironment;
typedef CgenEnvironment *CgenEnvironmentP;

/////////////////////////////////////////////////////////////////////////
//
//  CgenClassTable
//
//    Every class has an integer tag, which is a depth-first number
// assigned to each class via a dfs of the inheritance graph.  Each
// object of a class contains its tag in the first word of the object;
// the particular numbering strategy (depth-first) for class tags is
// exploited to efficiently implement the type case construct.
//
//    The CgenClass table contains mappings of classes <-> tags,
// as well as a mapping from a class to the greatest tag of any
// child of that class (used in code generation for case).  There
// is also a table of method tables, which desribes for each class C
// and method name f in C the offset in C's dispatch table of the code
// for f.
//
//    The CgenClassTable constructor actually performs code generation.
// The code_* private methods perform pieces of code generation.
//
/////////////////////////////////////////////////////////////////////////////

class CgenClassTable : public SymbolTable<Symbol,CgenNode> {
private:
   List<CgenNode> *nds;
   ostream& str;
   //int stringclasstag;         /*##*/
   //int intclasstag;            /*##*/
   //int boolclasstag;           /*##*/
   int num_classes;              /*##*/
   SymbolTable<Symbol,int>                      class_to_tag_table;              /*##*/
   SymbolTable<Symbol,int>                      class_to_max_child_tag_table;    /*##*/
   SymbolTable<int,StringEntry>                 tag_to_class_table;              /*##*/
   SymbolTable<Symbol,SymbolTable<Symbol,int> > table_of_method_tables;          /*##*/

// The following methods emit code for
// constants and global declarations.

   void code_global_data();
   void code_global_text();
   void code_bools();
   void code_select_gc();
   void code_constants();
   void code_strings();          /*##*/ // 无用，无函数定义
   void code_ints();             /*##*/ // 无用，无函数定义
   void code_class_table();      /*##*/
   void code_object_table();     /*##*/
   void code_class_features();   /*##*/

// The following creates an inheritance graph from
// a list of classes.  The graph is implemented as
// a tree of `CgenNode', and class names are placed
// in the base class symbol table.

   void install_basic_classes();
   void install_class(CgenNodeP nd);
   void install_classes(Classes cs);
   void build_inheritance_tree();
   void set_relations(CgenNodeP nd);
public:
   CgenClassTable(Classes, ostream& str);
   void code();
   CgenNodeP root();
   int assign_tag(Symbol name);              /*##*/
   void set_max_child(Symbol name, int tag); /*##*/
   void add_method_table(Symbol name, SymbolTable<Symbol,int> *method_table); /*##*/
   int last_tag();                           /*##*/
};


/////////////////////////////////////////////////////////////////////////////
//
//  VarBinding
//
//  There are three kinds of variables bindings in Cool: self, attributes,
// and local variables.  For each kind, different code is required for updates
// and references.  For each kind of binding, the offset is an index used
// to fetch the variable from the stack (for a local variable) or from the
// self object (for an attribute).  See the comments in cgen.cc.
//
/////////////////////////////////////////////////////////////////////////////

class VarBinding     /*##*/
{
protected:
  // For LocalBindings, a negative location is a register, a positive location
  // is a stack slot (offset from FP).
  int offset;
public:
  VarBinding(int i);
  virtual ~VarBinding() {}

  //returns the name of the register holding the value (optional_dest, if the
  //var is on the stack, or the register name otherwise).
  virtual char* code_ref(char *optional_dest, ostream&) = 0;

  //Forces the result to be moved to dest (to respect a calling convention).
  virtual void code_ref_force_dest(char *dest, ostream&) = 0;

  virtual void code_update(char *source, ostream&) = 0;

  //For LocalBindings, returns the register that stores the
  //variable, or null if it's on the stack.
  virtual Register get_register() = 0;
};

class AttributeBinding : public VarBinding /*##*/
{ 
 public: 
  AttributeBinding(int i); 
  virtual ~AttributeBinding() {}

  Register code_ref(char *optional_dest, ostream&);
  void code_ref_force_dest(char *dest, ostream& os) { code_ref(dest, os); }
  void code_update(char *source, ostream&);
  Register get_register() { return NULL; }
};

class SelfBinding : public VarBinding /*##*/
{ 
public: 
  SelfBinding(); 
  virtual ~SelfBinding() {}

  Register code_ref(char *optional_dest, ostream&);
  void code_ref_force_dest(char *dest, ostream& os) { code_ref(dest, os); }
  void code_update(char *source, ostream&);
  Register get_register() { return NULL; }
};

class LocalBinding : public VarBinding /*##*/
{ 
protected:
  // 当前的环境
  CgenEnvironmentP env;
public: 
  LocalBinding(int i, CgenEnvironmentP env); 
  virtual ~LocalBinding() {}

  Register code_ref(char *optional_dest, ostream&);
  // 之所以是force，是因为变量可能在stack中，也可能在register中
  // 所以需要进一步move到指定的位置上
  void code_ref_force_dest(char *dest, ostream& os);
  void code_update(char *source, ostream&);
  Register get_register();
};

//
// Method bindings are denoted just by a pair (class name, method name).
//
class MethodBinding     /*##*/
{
private:
  Symbol method_name;
  Symbol class_name;
public:
  MethodBinding(Symbol mn, Symbol cn);
  void code_ref(ostream&);
};

////////////////////////////////////////////////////////////////////////////
//
//   CgenNode contains all the information needed to generate code for
// a single class.  This includes:
//
//   - Inheritance graph information; the parent and children CgenNodes
//     of the class.
//
//   - Method information, which includes the layout of the dispatch
//     table for the class and the number of temporaries each method
//     will require (i.e., the size of the stack frame for the method).
//
//   - Object layout information; which (contiguous) slots in the
//     object the class attributes occupy.
//
//   - Symbol tables for attributes/variables, and initialization
//     information for attributes.
//
//  There are many methods for computing the above information and
//  generating various pieces of code (method bodies, dispatch tables,
//  initialization code, etc.)
//
///////////////////////////////////////////////////////////////////////////

class CgenNode : public class__class {
private: 
   CgenNodeP parentnd;                        // Parent of class
   List<CgenNode> *children;                  // Children of class
   Basicness basic_status;                    // `Basic' if class is basic
                                              // `NotBasic' otherwise
   int class_tag;       /*##*/        // class tag of the class
   int max_child;       /*##*/        // largest class tag of any child
      // method information
   int num_methods;        /*##*/
   SymbolTable<Symbol,int>          method_name_to_offset_table;        /*##*/   
   SymbolTable<Symbol,int>          method_name_to_numtemps_table;      /*##*/
   SymbolTable<int,MethodBinding>   method_offset_to_binding_table;     /*##*/

   // attribute and local variable information
   int first_attribute;       /*##*/
   int num_attributes;        /*##*/

   SymbolTable<Symbol,VarBinding> var_binding_table;        /*##*/
   SymbolTable<int,attr_class>    attribute_init_table;     /*##*/
   SymbolTable<int,Entry>         attribute_proto_table;    /*##*/

   // The global symbol table
   CgenClassTableP class_table;                    /*##*/

   void code_disptable_ref(ostream&);              /*##*/
   void code_protoobj_ref(ostream&);               /*##*/
   void code_init_ref(ostream&);                   /*##*/
   void code_method(ostream&, CgenEnvClassLevelP); /*##*/

public:
   CgenNode(Class_ c,
            Basicness bstatus,
            CgenClassTableP class_table);

   void add_child(CgenNodeP child);
   List<CgenNode> *get_children() { return children; }
   void set_parentnd(CgenNodeP p);
   CgenNodeP get_parentnd() { return parentnd; }
   int basic() { return (basic_status == Basic); }

   void init(int,                                                 /*##*/
             SymbolTable<Symbol,int>            method_name_to_offset_table,
             SymbolTable<int,MethodBinding>     methd_offset_to_binding_table,
             int                                first_attribute,
             SymbolTable<Symbol,VarBinding>     var_binding_table,
             SymbolTable<int,Entry>             attribute_proto_table);

   void assign_tags(CgenClassTable c);                            /*##*/
   void layout_method(Symbol mname, int numtemps);                /*##*/
   void layout_attribute(Symbol name, attr_class *a, int init);   /*##*/
   void code_prototype_object(ostream&);                          /*##*/
   void code_methods(ostream&, CgenEnvTopLevelP);                 /*##*/
   void code_init(ostream&, CgenEnvTopLevelP);                    /*##*/
   void code_dispatch_table(ostream&);                            /*##*/
   int get_tag();                                                 /*##*/
   int get_max_child();                                           /*##*/
};

class BoolConst 
{
 private: 
  int val;
 public:
  BoolConst(int);
  void code_def(ostream&, int boolclasstag);
  void code_ref(ostream&) const;
};

///////////////////////////////////////////////////////////////////////////
//
// Code for each method body is generated in an environment that specifies
// the class, layout information for dispatch tables for othe classes,
// and class tags, among other things.  Code generation environments are
// arranged as a hierarchy of three classes, each containing progressively
// more information.  The use of these environments is documented in cgen.cc
//
////////////////////////////////////////////////////////////////////////////

class CgenEnvTopLevel               /*##*/
{
 protected:
  SymbolTable<Symbol,int> *                        class_to_tag_table;
  SymbolTable<Symbol,int> *                        class_to_max_child_tag_table;
  SymbolTable<Symbol,SymbolTable<Symbol,int> > *   table_of_method_tables;
  int num_classes;  // total number of classes in the program
 public:
  CgenEnvTopLevel(SymbolTable<Symbol,int> *cttt,
		  SymbolTable<Symbol,int> *ctmctt,
		  SymbolTable<Symbol,SymbolTable<Symbol,int> > *tomt,
		  int nc) :
		    class_to_tag_table(cttt),
		    class_to_max_child_tag_table(ctmctt),
		    table_of_method_tables(tomt),
                    num_classes(nc)
		      { }
};

class CgenEnvClassLevel : public CgenEnvTopLevel   /*##*/
{
 protected:
  SymbolTable<Symbol,int> *      method_name_to_offset_table;

  // The var_binding_table is updated by code generation as local variable
  // bindings are added and removed.  For this reason, we copy the table
  // structure rather than use a pointer.
  //
  SymbolTable<Symbol,VarBinding> var_binding_table;
  Symbol                         classname; // classname and filename are used in
  Symbol                         filename;  // generating run time error messages.
 public:
  CgenEnvClassLevel(CgenEnvTopLevelP env,
		    SymbolTable<Symbol,int> *mntot,
		    SymbolTable<Symbol,VarBinding> vbt,
		    Symbol cn,
		    Symbol fn) :
		      CgenEnvTopLevel(*env),     // copy top-level environment
		      method_name_to_offset_table(mntot),
		      var_binding_table(vbt),
		      classname(cn),
		      filename(fn)
		    { }   
};

//
// CgenEnvironment is the environment for a method body.  It maintains
// the state of the stack frame as temporaries are added and removed,
// as formal parameters are pushed on the stack for a function call, and
// as labels are generated for jumps.
//
class CgenEnvironment : public CgenEnvClassLevel         /*##*/
{
 private:
  static int next_label;  // labels for jumps in generated code;
                          // static so that labels will be unique across
                          // the entire program.
  int next_formal;        // next available slot for a formal parameter
  int num_temps;          // total # of temporaries needed by method
  int next_temp_location; // next available temporary location
 public:
  CgenEnvironment(CgenEnvClassLevelP env,
		  Formals formals,
		  int num_temporaries);
  
  //
  // operations on global ("top level") tables
  //
  int lookup_tag(Symbol);
  int lookup_child_tag(Symbol);
  int lookup_method(Symbol classname, Symbol methodname);  

  //
  //  variable table operations: lookup, add, enter and exit scopes
  //
  VarBinding *lookup_var(Symbol);
  void add_local(Symbol sym);
  void add_formal(Symbol sym);
  void remove_local();
  int label();

  Register get_register(int offset);  //What register is used for this offset?
  Register get_next_register();       //What register will we allocate next?

  //For use by the function prologue/epilogue
  int get_num_temps();
  int get_register_temps();
  int get_stack_temps();
  
  int get_num_classes() { return num_classes; }
  Symbol get_filename()  { return filename; }
  Symbol get_classname() { return classname; }
};


//
//  CalcTemp implements a simple abstraction to manage the calculation
//  of the number of temporaries required by a method.  The object
//  keeps track of the # of temporaries currently in use,
//  and the maximum # of temporaries ever required.
//
class CalcTemp;
typedef CalcTemp *CalcTempP;

class CalcTemp {           /*##*/
private:
  int num;
  int max;
public:
  CalcTempP add()  { num++; if (num > max) max = num; return this;}
  CalcTempP sub()  { num--; return this; }
  int get_max()    { return max; }
  CalcTemp(): num(0), max(0) { }
};