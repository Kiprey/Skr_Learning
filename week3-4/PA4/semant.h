#ifndef SEMANT_H_
#define SEMANT_H_

#include <assert.h>
#include <iostream>  
#include "cool-tree.h"
#include "stringtab.h"
#include "symtab.h"

#define TRUE 1
#define FALSE 0

enum Basicness     {Basic, NotBasic};

class ClassTable;
typedef ClassTable *ClassTableP;

class Environment
{
private:
  // 可能这里是一个classTable?
  SymbolTable<Symbol, method_class> method_table;
  SymbolTable<Symbol, Entry> val_table; // this + 1
  ostream& error_stream; // 
  int semant_errors;
public:
  Environment() : error_stream(cerr) // TODO
  {}
  Environment(ClassTable*, InheritanceNode*) // TODO
  {}
  int get_self_type()  // TODO
  {}
  int lookup_class(Symbol s) // TODO
  {}
  int type_leq(Symbol s1, Symbol s2) // TODO
  {}
  int type_lub(Symbol s1, Symbol s2) // TODO
  {}


  ostream& semant_error()  {
    semant_errors++;                            
    return error_stream;
  }
  ostream& semant_error(tree_node* c)  {  
      

  }    
  ostream& semant_error(Symbol filename, tree_node *t)  {
      error_stream << filename << ":" << t->get_line_number() << ": ";
      return semant_error();
  }
  void method_enterscope() {
    method_table.enterscope();
  }
  void method_add(Symbol s, method_class* c) {
    method_table.addid(s, c);
  }
  method_class* method_probe(Symbol s) {
    return method_table.probe(s);
  }
  method_class* method_lookup(Symbol s) {
    return method_table.lookup(s);
  }
  void method_exitscope() {
    method_table.exitscope();
  }
  void var_enterscope() {
    val_table.enterscope();
  }
  void var_add(Symbol s1, Symbol s2) {
    val_table.addid(s1, s2);
  }
  Symbol var_probe(Symbol s) {
    return val_table.probe(s);
  }
  Symbol* var_lookup(Symbol s){
    return val_table.lookup(s);
  }
  void var_exitscope() {
    val_table.exitscope();
  }
};

// This is a structure that may be used to contain the semantic
// information such as the inheritance graph.  You may use it or not as
// you like: it is only here to provide a container for the supplied
// methods.

class InheritanceNode;
typedef InheritanceNode *InheritanceNodeP;

class InheritanceNode : public class__class{
private:
  Environment * env; // this + 11
  InheritanceNodeP parentnd;                        // Parent of class    // this + 6
  List<InheritanceNode> *children;                  // Children of class  // this + 7
  Basicness basic_status;                    // `Basic' if class is basic
                                              // `NotBasic' otherwise
public:
  InheritanceNode(Class_ nd, Basicness bstatus, ClassTableP ct) :
    class__class((const class__class &) *nd),
    parentnd(NULL),
    children(NULL),
    basic_status(bstatus)
  {}
  ~InheritanceNode(){
    delete env;
  };
  void add_child(InheritanceNodeP n)
  {
    children = new List<InheritanceNode>(n,children); // n : 0; tl : 1
  }
  void set_parentnd(InheritanceNodeP p)
  {
    assert(parentnd == NULL);
    assert(p != NULL);
    parentnd = p;
  }
  List<InheritanceNode> *get_children() { return children; } 
  InheritanceNodeP get_parentnd() { return parentnd; } 
  void init_env(ClassTable *classtable){
    env = new Environment(classtable, this);
  }
  void build_feature_tables() // TODO
  {

  }
  int mark_reachable() // TODO
  {
    // TODO 总感觉少了什么， 没有需要mark的变量
    for(List<InheritanceNode>* j = children; j != NULL; j = j->tl())
      j->hd()->mark_reachable();
  }
  int check_main_method() // TODO
  {
    if(env->method_probe(main_meth))
    {
      if(0 /* // TODO */)
        env->semant_error(this) << "\'main\' method in class Main should have no arguments.";
    }
    else
      env->semant_error(this) << "No 'main' method in class Main.";
    
    // "'main' method in class Main should have no arguments.
    // "No 'main' method in class Main."
  };
  int type_check_features() // TODo
  {

  }

};


class ClassTable : public SymbolTable<Symbol, InheritanceNode>{
private:
  List<InheritanceNode> *nds; // this + 1

  int semant_errors;
  void install_basic_classes();
  ostream& error_stream;

public:
  ClassTable(Classes);
  int errors() { return semant_errors; }
  ostream& semant_error();
  ostream& semant_error(Class_ c);
  ostream& semant_error(Symbol filename, tree_node *t);


  // *********************************************
  InheritanceNodeP root()
  {
    return probe(Object);
  }
  void install_class(InheritanceNodeP nd)
  {
    Symbol name = nd->get_name();

    if (probe(name))
      return;

    // The class name is legal, so add it to the list of classes
    // and the symbol table.
    nds = new List<InheritanceNode>(nd,nds);
    addid(name,nd);
  }

  void install_classes(Classes cs)
  {
    for(int i = cs->first(); cs->more(i); i = cs->next(i))
      install_class(new InheritanceNode(cs->nth(i), NotBasic, this));
  }

  void build_inheritance_tree()
  {
    for(List<InheritanceNode> *l = nds; l; l = l->tl())
        set_relations(l->hd());
  }

  void set_relations(InheritanceNodeP nd)
  {
    InheritanceNode *parent_node = probe(nd->get_parent());
    nd->set_parentnd(parent_node);
    parent_node->add_child(nd);
  }

  void build_feature_tables()  {
    root()->init_env(this);
    root()->build_feature_tables();
  }

  void check_main(){
    if(probe(Main))
      if(nds != NULL)
        nds->hd()->check_main_method();
    else
      semant_error() << "Class Main is not defined." << endl;
  }

  void check_for_cycles()
  { 
    for(List<InheritanceNode>* i = nds; i; i = nds->tl())
      for(InheritanceNodeP j = i->hd(); j; j = j->get_parentnd())
        if(i->hd()->get_name() == j->get_name())
          semant_error(i->hd()) << "Class " << i->hd()->get_name() << ", or an ancestor of " 
            << i->hd()->get_name() << ", is involved in an inheritance cycle." << endl;
  }

  ClassTable check_improper_inheritance() 
  {
    if(nds != NULL)
    {
      while(1)
      {
        Class_ tmpClass = nds->hd(); 
        if(probe(tmpClass->get_name())) // TODO
          semant_error(tmpClass) << "Class " << tmpClass->get_name() << " cannot inherit class " << "xx" << "." << endl;
        else
          semant_error(tmpClass) << "Class " << tmpClass->get_name() << " inherits from an undefined class ";
      }
    }
  }

};





#endif
