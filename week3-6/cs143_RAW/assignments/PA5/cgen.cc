
//**************************************************************
//
// Code generator SKELETON
//
// Read the comments carefully. Make sure to
//    initialize the base class tags in
//       `CgenClassTable::CgenClassTable'
//
//    Add the label for the dispatch tables to
//       `IntEntry::code_def'
//       `StringEntry::code_def'
//       `BoolConst::code_def'
//
//    Add code to emit everyting else that is needed
//       in `CgenClassTable::code'
//
//
// The files as provided will produce code to begin the code
// segments, declare globals, and emit constants.  You must
// fill in the rest.
//
//**************************************************************

#include "cgen.h"
#include "cgen_gc.h"

extern void emit_string_constant(ostream& str, char *s);

//declared in handle_flags.h:
extern int cgen_debug;  
extern int disable_reg_alloc;

//////////////////////////////////////////////////////////////////////
//
//  Code Generation
//
//  At a high level, code generation consists of:
//
//   1. compute the inheritance graph
//   2. assign tags to all classes in depth-first order
//   3. determine the layout of attributes, temporaries, and dispatch
//      tables for each class
//   4. generate code for global data: constants, dispatch tables,...
//   5. generate code for each feature
//
//   (1) is taken largely from the semantic analyzer.
//
///////////////////////////////////////////////////////////////////////

//
// Three symbols from the semantic analyzer (semant.cc) are used.
// If e : No_type, then no code is generated for e.
// Special code is generated for new SELF_TYPE.
// The name "self" also generates code different from other references.
//
//////////////////////////////////////////////////////////////////////
//
// Symbols
//
// For convenience, a large number of symbols are predefined here.
// These symbols include the primitive type and method names, as well
// as fixed names used by the runtime system.
//
//////////////////////////////////////////////////////////////////////
Symbol 
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

static char *gc_init_names[] =
  { "_NoGC_Init", "_GenGC_Init", "_ScnGC_Init" };
static char *gc_collect_names[] =
  { "_NoGC_Collect", "_GenGC_Collect", "_ScnGC_Collect" };


//  BoolConst is a class that implements code generation for operations
//  on the two booleans, which are given global names here.
BoolConst falsebool(FALSE);
BoolConst truebool(TRUE);

// The following temporary name will not conflict with any
// user-defined names.
#define TEMP1 "_1"          /*##*/

//*********************************************************
//
// Define method for code generation
//
// This is the method called by the compiler driver
// `cgtest.cc'. cgen takes an `ostream' to which the assembly will be
// emmitted, and it passes this and the class list of the
// code generator tree to the constructor for `CgenClassTable'.
// That constructor performs all of the work of the code
// generator.
//
//*********************************************************

void program_class::cgen(ostream &os) 
{
  // spim wants comments to start with '#'
  os << "# start of generated code\n";

  initialize_constants();
  CgenClassTable *codegen_classtable = new CgenClassTable(classes,os);

  os << "\n# end of generated code\n";
}


//////////////////////////////////////////////////////////////////////////////
//
//  emit_* procedures
//
//  emit_X  writes code for operation "X" to the output stream.
//  There is an emit_X for each opcode X, as well as emit_ functions
//  for generating names according to the naming conventions (see emit.h)
//  and calls to support functions defined in the trap handler.
//
//  Register names and addresses are passed as strings.  See `emit.h'
//  for symbolic names you can use to refer to the strings.
//
//////////////////////////////////////////////////////////////////////////////

static void emit_load(char *dest_reg, int offset, char *source_reg, ostream& s)
{
  s << LW << dest_reg << " " << offset * WORD_SIZE << "(" << source_reg << ")" 
    << endl;
}

static void emit_store(char *source_reg, int offset, char *dest_reg, ostream& s)
{
  s << SW << source_reg << " " << offset * WORD_SIZE << "(" << dest_reg << ")"
      << endl;
}

static void emit_load_imm(char *dest_reg, int val, ostream& s)
{ s << LI << dest_reg << " " << val << endl; }

static void emit_load_address(char *dest_reg, char *address, ostream& s)
{ s << LA << dest_reg << " " << address << endl; }

static void emit_partial_load_address(char *dest_reg, ostream& s)
{ s << LA << dest_reg << " "; }

static void emit_load_bool(char *dest, const BoolConst& b, ostream& s)
{
  emit_partial_load_address(dest,s);
  b.code_ref(s);
  s << endl;
}

static void emit_load_string(char *dest, StringEntry *str, ostream& s)
{
  emit_partial_load_address(dest,s);
  str->code_ref(s);
  s << endl;
}

static void emit_load_int(char *dest, IntEntry *i, ostream& s)
{
  emit_partial_load_address(dest,s);
  i->code_ref(s);
  s << endl;
}

static void emit_move(char *dest_reg, char *source_reg, ostream& s) /* ## */
{ 
  if (regEq(dest_reg, source_reg))
  {
    if (cgen_debug) 
    {
      cerr << "    Omitting move from "
          << source_reg << " to " << dest_reg << endl;
      s << "#";
    } 
    // 这里的代码有点有趣，当cgen_debug 为 true时，会输出 "#	move	$a0 $a0"
    // 当不为debug模式时，将不会输出这条注释
    else
      return;
  }
  s << MOVE << dest_reg << " " << source_reg << endl; 
}

static void emit_neg(char *dest, char *src1, ostream& s)
{ s << NEG << dest << " " << src1 << endl; }

static void emit_add(char *dest, char *src1, char *src2, ostream& s)
{ s << ADD << dest << " " << src1 << " " << src2 << endl; }

static void emit_addu(char *dest, char *src1, char *src2, ostream& s)
{ s << ADDU << dest << " " << src1 << " " << src2 << endl; }

static void emit_addiu(char *dest, char *src1, int imm, ostream& s)
{ s << ADDIU << dest << " " << src1 << " " << imm << endl; }

static void emit_binop(char* op, char *dest, char *src1, char *src2,  /* ## */
		       ostream& s)
{ s << op << dest << " " << src1 << " " << src2 << endl; }

static void emit_div(char *dest, char *src1, char *src2, ostream& s)
{ s << DIV << dest << " " << src1 << " " << src2 << endl; }

static void emit_mul(char *dest, char *src1, char *src2, ostream& s)
{ s << MUL << dest << " " << src1 << " " << src2 << endl; }

static void emit_sub(char *dest, char *src1, char *src2, ostream& s)
{ s << SUB << dest << " " << src1 << " " << src2 << endl; }

static void emit_sll(char *dest, char *src1, int num, ostream& s)
{ s << SLL << dest << " " << src1 << " " << num << endl; }

static void emit_jalr(char *dest, ostream& s)
{ s << JALR << "\t" << dest << endl; }

static void emit_jal(char *address,ostream &s)
{ s << JAL << address << endl; }

static void emit_return(ostream& s)
{ s << RET << endl; }

static void emit_copy(ostream& s)                             /*##*/
{ s << JAL << "Object.copy" << endl; }

static void emit_gc_assign(ostream& s)
{ s << JAL << "_GenGC_Assign" << endl; }

static void emit_equality_test(ostream& s)                     /*##*/
{ s << JAL << "equality_test" << endl; }

static void emit_case_abort(ostream& s)                        /*##*/
{ s << JAL << "_case_abort" << endl; }

static void emit_case_abort2(ostream& s)                       /*##*/
{ s << JAL << "_case_abort2" << endl; }

static void emit_dispatch_abort(ostream& s)                   /*##*/
{ s << JAL << "_dispatch_abort" << endl; }

static void emit_disptable_ref(Symbol sym, ostream& s)
{  s << sym << DISPTAB_SUFFIX; }

static void emit_init_ref(Symbol sym, ostream& s)
{ s << sym << CLASSINIT_SUFFIX; }

static void emit_init(Symbol classname, ostream& s)           /* ## */
{
  s << JAL;
  emit_init_ref(classname,s);
  s << endl;
}

static void emit_label_ref(int l, ostream &s)
{ s << "label" << l; }

static void emit_protobj_ref(Symbol sym, ostream& s)
{ s << sym << PROTOBJ_SUFFIX; }

static void emit_method_ref(Symbol classname, Symbol methodname, ostream& s)
{ s << classname << METHOD_SEP << methodname; }

static void emit_label_def(int l, ostream &s)
{
  emit_label_ref(l,s);
  s << ":" << endl;
}

static void emit_beqz(char *source, int label, ostream &s)
{
  s << BEQZ << source << " ";
  emit_label_ref(label,s);
  s << endl;
}

static void emit_beq(char *src1, char *src2, int label, ostream &s)
{
  s << BEQ << src1 << " " << src2 << " ";
  emit_label_ref(label,s);
  s << endl;
}

static void emit_bne(char *src1, char *src2, int label, ostream &s)
{
  s << BNE << src1 << " " << src2 << " ";
  emit_label_ref(label,s);
  s << endl;
}

static void emit_bleq(char *src1, char *src2, int label, ostream &s)
{
  s << BLEQ << src1 << " " << src2 << " ";
  emit_label_ref(label,s);
  s << endl;
}

static void emit_blt(char *src1, char *src2, int label, ostream &s)
{
  s << BLT << src1 << " " << src2 << " ";
  emit_label_ref(label,s);
  s << endl;
}

static void emit_blti(char *src1, int imm, int label, ostream &s)
{
  s << BLT << src1 << " " << imm << " ";
  emit_label_ref(label,s);
  s << endl;
}

static void emit_bgti(char *src1, int imm, int label, ostream &s)
{
  s << BGT << src1 << " " << imm << " ";
  emit_label_ref(label,s);
  s << endl;
}

static void emit_branch(int l, ostream& s)
{
  s << BRANCH;
  emit_label_ref(l,s);
  s << endl;
}

//
// Push a register on the stack. The stack grows towards smaller addresses.
//
static void emit_push(char *reg, ostream& str)
{
  emit_store(reg,0,SP,str);
  emit_addiu(SP,SP,-4,str);
}

//
// Fetch the integer value in an Int object.
// Emits code to fetch the integer value of the Integer object pointed
// to by register source into the register dest
//
static void emit_fetch_int(char *dest, char *source, ostream& s)
{ emit_load(dest, DEFAULT_OBJFIELDS, source, s); }

//
// Emits code to store the integer value contained in register source
// into the Integer object pointed to by dest.
//
static void emit_store_int(char *source, char *dest, ostream& s)
{ emit_store(source, DEFAULT_OBJFIELDS, dest, s); }

// 在这个文件中没有用到该函数
static void emit_test_collector(ostream &s)
{
  emit_push(ACC, s);
  emit_move(ACC, SP, s); // stack end
  emit_move(A1, ZERO, s); // allocate nothing
  s << JAL << gc_collect_names[cgen_Memmgr] << endl;
  emit_addiu(SP,SP,4,s);
  emit_load(ACC,0,SP,s);
}

static void emit_gc_check(char *source, ostream &s)
{
  if (source != (char*)A1) emit_move(A1, source, s);
  s << JAL << "_gc_check" << endl;
}

///////////////////////////////////////////////////////////////////////////////
//
//  function_prologue
//  function_epilogue
//
//  Together these two functions implement the callee-side of the calling
//  convention.  
//
//  A stack frame that needs n temporary variables and puts k of those in 
//  callee-save registers ($s1-$s6) has the following layout:
//    ( If register allocation is disabled, k=0.  Othersize, k = min{n,6}.  )
//
//  ----------------
//  | actual arg 1 |
//  |    ...       |
//  | actual arg n |
//  ---------------- (*)
//  | caller's FP  |
//  |   "     self |
//  |   "      RA  |
//  |   "     $s1  |     (callee-save registers, if needed)
//  |     ...      |
//  |   "     $sk  |
//  ---------------- (**)
//  | temporary k+1|
//  |    ...       |
//  | temporary  n | <- FP
//  ----------------
//
//  The caller is responsible for placing the actual arguments---the portion
//  above (*)---on the stack.  On function entry, the callee:
//
//  (1)  saves the caller's frame pointer,
//       self pointer, and return address (*)-(**),
//
//  (2)  allocates space for the callee's temporaries on the stack,
//       if using a garbage collector, these temporaries are cleared
//       (so that no garbage resides on the stack)
//
//  (3)  sets the callee's self and FP registers.
//
//
//  On function exit, the callee:
//
//  (1) restores the caller's frame pointer, self pointer, and return address
//
//  (2) pops the callee's frame of the stack, including the actual arguments
//
//////////////////////////////////////////////////////////////////////////////

// 在一个函数开始前所要执行的动作
static void function_prologue(CgenEnvironment* env, ostream& s)
{
  int num_temps = env->get_num_temps();
  //reg_temps = k, and stack_temps = n-k in the diagram above
  int reg_temps = env->get_register_temps();
  int stack_temps = env->get_stack_temps();
  assert ( (reg_temps + stack_temps) == num_temps);

  emit_addiu(SP,SP,-(3 + num_temps) * WORD_SIZE,s);   // allocate frame
  emit_store(FP, 3 + num_temps, SP, s);               // save caller's FP
  emit_store(SELF, 2 + num_temps, SP, s);             //  "    "       SELF
  emit_store(RA, 1 + num_temps, SP, s);               //  "    "       RA
  emit_addiu(FP, SP, 4, s);                           // set new FP
  emit_move(SELF, ACC, s);                            // set SELF register

 //Save callee-save registers
 for(int i = 0; i < reg_temps; i++) {
   //The first slot for saving registers is at FP + num_temps - 1. 
   //So the ith register is at FP + num_temps - i - 1
   emit_store(regNames[i], num_temps - i - 1, FP, s);// save caller's $si    
 }
 
  // Clear temporaries for GC
  if (cgen_Memmgr != GC_NOGC)
    for (int i = 0; i < stack_temps; i++)
      emit_store(ZERO, i, FP, s);

  if (cgen_Memmgr_Debug == GC_DEBUG)
    emit_gc_check(SELF, s);
}

// 在一个函数结束前所要执行的动作
static void function_epilogue(CgenEnvironment* env, int num_formals,ostream& s)
{
  int num_temps = env->get_num_temps();

  if (cgen_Memmgr_Debug == GC_DEBUG)
    emit_gc_check(ACC, s);

  //Restore callee-save registers
  int reg_temps = env->get_register_temps();
  for(int i = 0; i < reg_temps; i++) {
    emit_load(regNames[i], num_temps - i - 1, FP, s); // restore caller's $si
  }

  emit_load(FP, 3  + num_temps, SP, s);               // restore caller's FP
  emit_load(SELF, 2 + num_temps, SP, s);              // restore caller's SELF
  emit_load(RA, 1 + num_temps, SP, s);                // restore caller's RA
                                                      // deallocate frame
  emit_addiu(SP, SP, (3 + num_temps + num_formals) * WORD_SIZE, s); 
  emit_return(s);
}

///////////////////////////////////////////////////////////////////////////////
//
// VarBinding and MethodBinding methods
//
// From the point of view of code generation, there are five distinct 
// classes of names in a method:
//  
//      locals               are stored in the temporary area of the stack frame
//      formal parameters    are stored in the actuals are of the frame
//      self                 is in the SELF register
//      attribute            is at an offset from the address given by SELF
//      method               address is at an offset from the dispatch table
//
// See the discussion about frame layout under function_prologue/epilogue.
// For the first four categories, there are distinct conventions for generating
// code for allocation, reference, and update.  For methods, each method name
// refers to a method in some ancestor class.
//
// VarBinding is the base class of three derived classes:
//
//      AttributeBinding      Records the offset of an attribute in the object.
//
//      LocalBinding          Records the offset from the frame pointer of
//                            a local variable or formal parameter.  Because
//                            locals and formals are layed out in different
//                            areas relative to the frame pointer, there are 
//                            separate methods for creating local and formal 
//                            bindings.
//
//      SelfBinding           The self object.
//
// A VarBinding has virtual functions for generating code for referencing and
// updating variables of each kind.
//
// MethodBindings are pairs consisting of a method's name and the class in
// which it is defined.  This information is needed for each method of a
// class to define the class' dispatch table.
//
///////////////////////////////////////////////////////////////////////////////

VarBinding::VarBinding(int i) : offset(i) { }

MethodBinding::MethodBinding(Symbol cn, Symbol mn) :
method_name(mn), class_name(cn)
{ }

//
// code_ref generates a dispatch table entry.
//
void MethodBinding::code_ref(ostream& s)
{
  s << WORD;
  emit_method_ref(class_name,method_name,s);
  s << endl;
}

AttributeBinding::AttributeBinding(int i) : VarBinding(i) { }

Register AttributeBinding::code_ref(char *dest, ostream& s) 
{
  if (cgen_debug)  cerr << "  Attribute load from offset " << offset << endl;
  emit_load(dest, offset + DEFAULT_OBJFIELDS, SELF, s);
  return dest;
}

void AttributeBinding::code_update(char *source,ostream& s)
{
  if (cgen_debug)  cerr << "  Attribute store to offset " << offset << endl;
  emit_store(source, offset + DEFAULT_OBJFIELDS, SELF, s);

  if (cgen_Memmgr_Debug == GC_DEBUG)
    emit_gc_check(source, s);

  if (cgen_Memmgr == GC_GENGC) {
    /* Memorize write address */
    emit_addiu(A1, SELF, WORD_SIZE * (offset + DEFAULT_OBJFIELDS), s);
    emit_gc_assign(s);
  }
}

SelfBinding::SelfBinding() : VarBinding(0) { }

Register SelfBinding::code_ref(char *dest, ostream& s) 
{
  emit_move(dest,SELF,s);
  return dest;
}

//
// It is a code generation error if an update to self is every generated.
//
void SelfBinding::code_update(char *, ostream&)
{
  cerr << "Cannot assign to self.";
  exit(1);
}


LocalBinding::LocalBinding(int i, CgenEnvironmentP env_) : VarBinding(i) { 
  env = env_;
}

Register LocalBinding::code_ref(char *dest, ostream& s) 
{
  Register reg = env->get_register(offset);
  if (reg != NULL) 
  {
    if (cgen_debug) cerr << "     Local read from register " << reg << endl;
    return reg;
  } 
  else
  {
    if (cgen_debug) cerr << "     Local load from FP offset " << offset << endl;
    emit_load(dest, offset, FP, s);
    return dest;
  }
}

void LocalBinding::code_ref_force_dest(char *dest, ostream& s) 
{
  Register result = code_ref(dest, s);
  emit_move(dest, result, s); //omitted if result = dest
}

void LocalBinding::code_update(char *source, ostream& s)
{
  Register reg = env->get_register(offset);
  if (reg != NULL) 
    {
      if (cgen_debug) cerr << "     Local store to register" 
			   << reg << endl;
     emit_move(reg, source, s);
    }
  else
    {
      if (cgen_debug) cerr << "     Local store to FP offset " << offset << endl;
      emit_store(source, offset, FP, s);
    }
}

Register LocalBinding::get_register() 
{ 
  return env->get_register(offset); 
}

///////////////////////////////////////////////////////////////////////////////
//
// coding strings, ints, and booleans
//
// Cool has three kinds of constants: strings, ints, and booleans.
// This section defines code generation for each type.
//
// All string constants are listed in the global "stringtable" and have
// type StringEntry.  StringEntry methods are defined both for String
// constant definitions and references.
//
// All integer constants are listed in the global "inttable" and have
// type IntEntry.  IntEntry methods are defined for Int
// constant definitions and references.
//
// Since there are only two Bool values, there is no need for a table.
// The two booleans are represented by instances of the class BoolConst,
// which defines the definition and reference methods for Bools.
//
///////////////////////////////////////////////////////////////////////////////

//
// Strings
//
void StringEntry::code_ref(ostream& s)
{
  s << STRCONST_PREFIX << index;
}

//
// Emit code for a constant String.
// You should fill in the code naming the dispatch table.
//

void StringEntry::code_def(ostream& s, int stringclasstag)
{
  IntEntryP lensym = inttable.add_int(len);

  // Add -1 eye catcher
  s << WORD << "-1" << endl;

  code_ref(s);  s  << LABEL                                             // label
      << WORD << stringclasstag << endl                                 // tag
      << WORD << (DEFAULT_OBJFIELDS + STRING_SLOTS + (len+4)/4) << endl // size
      << WORD; emit_disptable_ref(idtable.lookup_string(STRINGNAME),s); 


 /***** Add dispatch information for class String ******/

      s << endl;                                              // dispatch table
      s << WORD;  lensym->code_ref(s);  s << endl;            // string length
  emit_string_constant(s,str);                                // ascii string
  s << ALIGN;                                                 // align to word
}

//
// StrTable::code_string
// Generate a string object definition for every string constant in the 
// stringtable.
//
void StrTable::code_string_table(ostream& s, int stringclasstag)
{  
  for (List<StringEntry> *l = tbl; l; l = l->tl())
    l->hd()->code_def(s,stringclasstag);
}

//
// Ints
//
void IntEntry::code_ref(ostream &s)
{
  s << INTCONST_PREFIX << index;
}

//
// Emit code for a constant Integer.
// You should fill in the code naming the dispatch table.
//

void IntEntry::code_def(ostream &s, int intclasstag)
{
  // Add -1 eye catcher
  s << WORD << "-1" << endl;

  code_ref(s);  s << LABEL                                // label
      << WORD << intclasstag << endl                      // class tag
      << WORD << (DEFAULT_OBJFIELDS + INT_SLOTS) << endl  // object size
      << WORD; 

 /***** Add dispatch information for class Int ******/
      emit_disptable_ref(idtable.lookup_string(INTNAME),s); 

      s << endl;                                          // dispatch table
      s << WORD << str << endl;                           // integer value
}


//
// IntTable::code_string_table
// Generate an Int object definition for every Int constant in the
// inttable.
//
void IntTable::code_string_table(ostream &s, int intclasstag)
{
  for (List<IntEntry> *l = tbl; l; l = l->tl())
    l->hd()->code_def(s,intclasstag);
}


//
// Bools
//
BoolConst::BoolConst(int i) : val(i) { assert(i == 0 || i == 1); }

void BoolConst::code_ref(ostream& s) const
{
  s << BOOLCONST_PREFIX << val;
}
  
//
// Emit code for a constant Bool.
// You should fill in the code naming the dispatch table.
//

void BoolConst::code_def(ostream& s, int boolclasstag)
{
  // Add -1 eye catcher
  s << WORD << "-1" << endl;

  code_ref(s);  s << LABEL                                  // label
      << WORD << boolclasstag << endl                       // class tag
      << WORD << (DEFAULT_OBJFIELDS + BOOL_SLOTS) << endl   // object size
      << WORD;  
    
 /***** Add dispatch information for class Bool ******/
      emit_disptable_ref(idtable.lookup_string(BOOLNAME),s); 

      s << endl;                                            // dispatch table
      s << WORD << val << endl;                             // value (0 or 1)
}

//////////////////////////////////////////////////////////////////////////////
//
//  CgenClassTable methods
//
//////////////////////////////////////////////////////////////////////////////

//***************************************************
//
//  Emit code to start the .data segment and to
//  declare the global names.
//
//***************************************************

void CgenClassTable::code_global_data()
{
  Symbol main    = idtable.lookup_string(MAINNAME);
  Symbol string  = idtable.lookup_string(STRINGNAME);
  Symbol integer = idtable.lookup_string(INTNAME);
  Symbol boolc   = idtable.lookup_string(BOOLNAME);

  str << "\t.data\n" << ALIGN;
  //
  // The following global names must be defined first.
  //
  str << GLOBAL << CLASSNAMETAB << endl;
  str << GLOBAL; emit_protobj_ref(main,str);    str << endl;
  str << GLOBAL; emit_protobj_ref(integer,str); str << endl;
  str << GLOBAL; emit_protobj_ref(string,str);  str << endl;
  str << GLOBAL; falsebool.code_ref(str);  str << endl;
  str << GLOBAL; truebool.code_ref(str);   str << endl;
  str << GLOBAL << INTTAG << endl;
  str << GLOBAL << BOOLTAG << endl;
  str << GLOBAL << STRINGTAG << endl;

  //
  // We also need to know the tag of the Int, String, and Bool classes
  // during code generation.
  //
  str << INTTAG << LABEL
      << WORD << *class_to_tag_table.lookup(integer) << endl;
      // << WORD << intclasstag << endl;
  str << BOOLTAG << LABEL 
      << WORD << *class_to_tag_table.lookup(boolc) << endl;
      // << WORD << boolclasstag << endl;
  str << STRINGTAG << LABEL 
      << WORD << *class_to_tag_table.lookup(string) << endl;
      //<< WORD << stringclasstag << endl;    
}


//***************************************************
//
//  Emit code to start the .text segment and to
//  declare the global names.
//
//***************************************************

void CgenClassTable::code_global_text()
{
  str << GLOBAL << HEAP_START << endl
      << HEAP_START << LABEL 
      << WORD << 0 << endl
      << "\t.text" << endl
      << GLOBAL;
  emit_init_ref(idtable.add_string("Main"), str);
  str << endl << GLOBAL;
  emit_init_ref(idtable.add_string("Int"),str);
  str << endl << GLOBAL;
  emit_init_ref(idtable.add_string("String"),str);
  str << endl << GLOBAL;
  emit_init_ref(idtable.add_string("Bool"),str);
  str << endl << GLOBAL;
  emit_method_ref(idtable.add_string("Main"), idtable.add_string("main"), str);
  str << endl;
}

void CgenClassTable::code_bools()
{
  // new add
  int boolclasstag = *class_to_tag_table.lookup(idtable.add_string(BOOLNAME));
  
  falsebool.code_def(str,boolclasstag);
  truebool.code_def(str,boolclasstag);
}

void CgenClassTable::code_select_gc()
{
  //
  // Generate GC choice constants (pointers to GC functions)
  //
  str << GLOBAL << "_MemMgr_INITIALIZER" << endl;
  str << "_MemMgr_INITIALIZER:" << endl;
  str << WORD << gc_init_names[cgen_Memmgr] << endl;
  str << GLOBAL << "_MemMgr_COLLECTOR" << endl;
  str << "_MemMgr_COLLECTOR:" << endl;
  str << WORD << gc_collect_names[cgen_Memmgr] << endl;
  str << GLOBAL << "_MemMgr_TEST" << endl;
  str << "_MemMgr_TEST:" << endl;
  str << WORD << (cgen_Memmgr_Test == GC_TEST) << endl;
}

//
//  The class name table is a mapping from
//  class tags -> class names.
//
void CgenClassTable::code_class_table()
{
  str << CLASSNAMETAB << LABEL;
  for(int i = 0; i < num_classes; i++)
  {
    StringEntry *c = tag_to_class_table.lookup(i);
    assert(c != NULL);
    str << WORD;
    c->code_ref(str);
    str << endl;
  }
}

//
// The class object table is a mapping from
// class tag -> address of prototype object for the class
//
void CgenClassTable::code_object_table()
{
  str << CLASSOBJTAB << LABEL;
  for(int i = 0; i < num_classes; i++)
    {
      Symbol c = tag_to_class_table.lookup(i);
      assert(c != NULL);
      str << WORD;
      emit_protobj_ref(c,str);
      str << endl << WORD;
      emit_init_ref(c,str);
      str << endl;
    }
}

//********************************************************
//
// Emit code to reserve space for and initialize all of
// the constants.  Class names should have been added to
// the string table (in the supplied code, is is done
// during the construction of the inheritance graph), and
// code for emitting string constants as a side effect adds
// the string's length to the integer table.  The constants
// are emmitted by running through the stringtable and inttable
// and producing code for each entry.
//
//********************************************************

void CgenClassTable::code_constants()
{
  //
  // Add constants that are required by the code generator.
  //
  
  // 将空字符串以及数字0添加进table中
  stringtable.add_string("");
  inttable.add_string("0");

  // new add
  int stringclasstag = *class_to_tag_table.lookup(idtable.lookup_string(STRINGNAME));
  int intclasstag = *class_to_tag_table.lookup(idtable.lookup_string(INTNAME));

  // 将所有的字符串常量与数字常量全部显示出来。
  stringtable.code_string_table(str,stringclasstag);
  inttable.code_string_table(str,intclasstag);
  code_bools();
}


CgenClassTable::CgenClassTable(Classes classes, ostream& s) : nds(NULL) , str(s)
{
   //stringclasstag = 0 /* Change to your String class tag here */;
   //intclasstag =    0 /* Change to your Int class tag here */;
   //boolclasstag =   0 /* Change to your Bool class tag here */;

   if (cgen_debug) cout << "Building CgenClassTable" << endl;
   num_classes = 0;
   // make sure the various tables have a scope
   class_to_tag_table.enterscope();
   class_to_max_child_tag_table.enterscope();
   tag_to_class_table.enterscope();
   table_of_method_tables.enterscope();


   enterscope();
   install_basic_classes();
   install_classes(classes);
   build_inheritance_tree();

   // 递归，将每个CgenNode的attr/method数据
   // 都填入CgenNode和CgenClassTable中的table中
   root()->init(0
               ,*(new SymbolTable<Symbol,int>)
               ,*(new SymbolTable<int,MethodBinding>)
               ,0
               ,*(new SymbolTable<Symbol,VarBinding>)
               ,*(new SymbolTable<int,Entry>));
   // 生成代码
   code();
   exitscope();
}

void CgenClassTable::install_basic_classes()
{

// The tree package uses these globals to annotate the classes built below.
  //curr_lineno  = 0;
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
	new CgenNode(class_(No_class,No_class,nil_Features(),filename),
			    Basic,this));
  addid(SELF_TYPE,
	new CgenNode(class_(SELF_TYPE,No_class,nil_Features(),filename),
			    Basic,this));
  addid(prim_slot,
	new CgenNode(class_(prim_slot,No_class,nil_Features(),filename),
			    Basic,this));

// 
// The Object class has no parent class. Its methods are
//        cool_abort() : Object    aborts the program
//        type_name() : Str        returns a string representation of class name
//        copy() : SELF_TYPE       returns a copy of the object
//
// There is no need for method bodies in the basic classes---these
// are already built in to the runtime system.
//
  install_class(
   new CgenNode(
    class_(Object, 
	   No_class,
	   append_Features(
           append_Features(
           single_Features(method(cool_abort, nil_Formals(), Object, no_expr())),
           single_Features(method(type_name, nil_Formals(), Str, no_expr()))),
           single_Features(method(copy, nil_Formals(), SELF_TYPE, no_expr()))),
	   filename),
    Basic,this));

// 
// The IO class inherits from Object. Its methods are
//        out_string(Str) : SELF_TYPE          writes a string to the output
//        out_int(Int) : SELF_TYPE               "    an int    "  "     "
//        in_string() : Str                    reads a string from the input
//        in_int() : Int                         "   an int     "  "     "
//
   install_class(
    new CgenNode(
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
	   filename),	    
    Basic,this));

//
// The Int class has no methods and only a single attribute, the
// "val" for the integer. 
//
   install_class(
    new CgenNode(
     class_(Int, 
	    Object,
            single_Features(attr(val, prim_slot, no_expr())),
	    filename),
     Basic,this));

//
// Bool also has only the "val" slot.
//
    install_class(
     new CgenNode(
      class_(Bool, Object, single_Features(attr(val, prim_slot, no_expr())),filename),
      Basic,this));

//
// The class Str has a number of slots and operations:
//       val                                  ???
//       str_field                            the string itself
//       length() : Int                       length of the string
//       concat(arg: Str) : Str               string concatenation
//       substr(arg: Int, arg2: Int): Str     substring
//       
   install_class(
    new CgenNode(
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
	     filename),
        Basic,this));

}

// CgenClassTable::install_class
// CgenClassTable::install_classes
//
// install_classes enters a list of classes in the symbol table.
//
void CgenClassTable::install_class(CgenNodeP nd)
{
  Symbol name = nd->get_name();

  if (probe(name))
    {
      return;
    }

  // The class name is legal, so add it to the list of classes
  // and the symbol table.
  nds = new List<CgenNode>(nd,nds);
  addid(name,nd);
}

void CgenClassTable::install_classes(Classes cs)
{
  for(int i = cs->first(); cs->more(i); i = cs->next(i))
    install_class(new CgenNode(cs->nth(i),NotBasic,this));
}

//
// CgenClassTable::build_inheritance_tree
//
void CgenClassTable::build_inheritance_tree()
{
  for(List<CgenNode> *l = nds; l; l = l->tl())
      set_relations(l->hd());
}

//
// CgenClassTable::set_relations
//
// Takes a CgenNode and locates its, and its parent's, inheritance nodes
// via the class table.  Parent and child pointers are added as appropriate.
//
void CgenClassTable::set_relations(CgenNodeP nd)
{
  CgenNode *parent_node = probe(nd->get_parent());
  nd->set_parentnd(parent_node);
  parent_node->add_child(nd);
}

void CgenNode::add_child(CgenNodeP n)
{
  children = new List<CgenNode>(n,children);
}

void CgenNode::set_parentnd(CgenNodeP p)
{
  assert(parentnd == NULL);
  assert(p != NULL);
  parentnd = p;
}


int CgenClassTable::assign_tag(Symbol name)
{ 
  assert(! class_to_tag_table.lookup(name));
  assert(! tag_to_class_table.lookup(num_classes));

  class_to_tag_table.addid(name,new int(num_classes));
  tag_to_class_table.addid(num_classes,stringtable.add_string(name->get_string()));
  return num_classes++; 
}

void CgenClassTable::set_max_child(Symbol name, int tag)
{
  assert(! class_to_max_child_tag_table.lookup(name));
  
  class_to_max_child_tag_table.addid(name, new int(tag));
}

int CgenClassTable::last_tag()
{ 
  assert(num_classes != 0);
  return num_classes - 1; 
}

void CgenClassTable::add_method_table(Symbol name, 
				      SymbolTable<Symbol,int> *method_table)
{
  assert(! table_of_method_tables.lookup(name));
  table_of_method_tables.addid(name,method_table);
}


void CgenClassTable::code()
{
  
  if (cgen_debug) cout << "coding global data" << endl;
  code_global_data();

  if (cgen_debug) cout << "choosing gc" << endl;
  code_select_gc();

  if (cgen_debug) cout << "coding constants" << endl;
  code_constants();

//                 Add your code to emit
//                   - prototype objects
//                   - class_nameTab
//                   - dispatch tables
//
  if (cgen_debug) cerr << "coding class table" << endl;
  code_class_table();

  if (cgen_debug) cerr << "coding object table" << endl;
  code_object_table();

  if (cgen_debug) cerr << "coding dispatch tables" << endl;
  root()->code_dispatch_table(str);

  // 
  // Check that strings required to code the prototype objects are installed.
  //
  assert(inttable.lookup_string("0"));
  assert(stringtable.lookup_string(""));
  assert(idtable.lookup_string(INTNAME));
  assert(idtable.lookup_string(STRINGNAME));
  assert(idtable.lookup_string(BOOLNAME));

  if (cgen_debug) cerr << "coding prototypes" << endl;
  root()->code_prototype_object(str);

  // /*##*/

  if (cgen_debug) cout << "coding global text" << endl;
  code_global_text();

//                 Add your code to emit
//                   - object initializer
//                   - the class methods
//                   - etc...
  CgenEnvTopLevelP env = new CgenEnvTopLevel(&class_to_tag_table,
					     &class_to_max_child_tag_table,
					     &table_of_method_tables,
					     num_classes);

  if (cgen_debug) cerr << "coding init methods" << endl;
  root()->code_init(str,env);

  if (cgen_debug) cerr << "coding methods" << endl;
  root()->code_methods(str,env);

}


CgenNodeP CgenClassTable::root()
{
   return probe(Object);
}


///////////////////////////////////////////////////////////////////////
//
// CgenNode methods
//
///////////////////////////////////////////////////////////////////////

CgenNode::CgenNode(Class_ nd, Basicness bstatus, CgenClassTableP ct) :
   class__class((const class__class &) *nd),
   parentnd(NULL),
   children(NULL),
   basic_status(bstatus),
   num_methods(0),
   method_name_to_offset_table(),
   method_offset_to_binding_table(),
   first_attribute(0),
   num_attributes(0),
   var_binding_table(),
   attribute_proto_table(),
   class_table(ct)
{ 
   //stringtable.add_string(name->get_string());          // Add class name to string table
}

void CgenNode::init(int nm,
		    SymbolTable<Symbol,int> mntot,
		    SymbolTable<int,MethodBinding> motbt,
		    int fa,
		    SymbolTable<Symbol,VarBinding> vbt,
		    SymbolTable<int,Entry> apt)
{
  // method 的个数
  num_methods = nm;
  method_name_to_offset_table = mntot;
  method_offset_to_binding_table = motbt;
  // 从给定的数字开始计算attribute的tag
  first_attribute = fa;
  var_binding_table = vbt;
  attribute_proto_table = apt;

  if (cgen_debug) cerr << "Building CgenNode for " << name << endl;
  // 在当前class_table中的表中，对传入的name赋以一个tag
  // 这条语句既在CgenClassTable中注册，又赋值于当前CgenNode的class_tag
  class_tag = class_table->assign_tag(name);

  // 
  // Methods and attributes are inherited from parent classes.
  // Separate scopes are used to keep child attributes distinct from 
  // parent attributes, and to allow methods to be redefined.
  //
  method_name_to_offset_table.enterscope();
  method_name_to_numtemps_table.enterscope();
  method_offset_to_binding_table.enterscope();
  var_binding_table.enterscope();
  attribute_proto_table.enterscope();

  // 
  // Each class is concerned with initializing only its own attributes,
  // so the attribute init table needs no information from the parent class.
  //

  // DONE 下面第一条语句似乎可以删去？ 删除后仍然可以通过打分测试
  attribute_init_table = *(new SymbolTable<int,attr_class>);
  attribute_init_table.enterscope();
  
  //
  // generate layout information for each feature of the class
  //
  // 将attribute 和 method 相关的信息都添加进table中
  for(int i = features->first(); features->more(i); i = features->next(i))
    features->nth(i)->layout_feature(this);

  //
  // The next attribute of any child classes will be assigned slot:
  //
  int next_attribute = first_attribute + num_attributes;

  //
  // Recursively lay out each of the child classes.  Information is
  // passed from parent to child about the layout of the parent.
  // The list of child classes is also constructed.
  //

  // 递归执行
  // 添加attr和method时，直接在原来的基础上添加
  List<CgenNode> *child = children;
  //children = NULL;
  for(; child; child = child->tl()) {
     child->hd()->init(num_methods,
                          method_name_to_offset_table,
                          method_offset_to_binding_table,
                          next_attribute,
                          var_binding_table,
                          attribute_proto_table);
     //children = new List<CgenNode>(child->hd(),children);      
     // Reverse tree so traversed in the same order as old code
  }
  //
  // Record the largest descendant class tag.  Because classes are
  // numbered depth first, class A is a subclass of B if A's tag lies
  // in the range
  //      B's tag <= A's tag <= (max descendant of B)'s tag
  //
  max_child = class_table->last_tag();
  class_table->set_max_child(name,max_child);

  if (cgen_debug) cerr << "For class " << name << " tag is " << class_tag <<
    " and max child is " << max_child << endl;
  // add the method table to the table of method tables
  class_table->add_method_table(name, &method_name_to_offset_table);
}

//
// layout_method assigns methods slots in the dispatch table for the
// class.  It also ensures that inverse mapping offset -> binding is
// updated and the number of method temporaries is recorded.  The
// first test to see of the method is already in the table is necessary
// because methods can be redefined by inheriting classes.
//
void CgenNode::layout_method(Symbol mname, int numtemps)
{
  int offset;

  // 如果在之前的类中，发现了相同的method
  if (method_name_to_offset_table.lookup(mname))
    // 则将其找出来，准备覆盖原method
    offset = *(method_name_to_offset_table.lookup(mname));
  else
    // 否则，准备新的offset
    offset = num_methods++;

  if (cgen_debug) cerr << "   Method: " << mname << " Class: " << name <<
    " dispatch table offset: " << offset << endl;
  method_name_to_offset_table.addid(mname,new int(offset));
  // 如果是覆盖函数的话，则覆盖原先offset上的MethodBindin，重新进行方法绑定
  // DONE 覆盖会不会影响其他的类？ 不影响，因为addid只会添加到当前scope里
  // 那一个SymbolTable里不就会有两个offset相同的MethodBinding了么？
  // 问题不大， lookup函数只会返回第一个找到的，所以可以完美避开这个坑
  method_offset_to_binding_table.addid(offset, new MethodBinding(name,mname));
  method_name_to_numtemps_table.addid(name,new int(numtemps));
}

//
// layout attribute assigns an attribute a position in an object.
// There is no need to test if the attribute already has a position assigned
// by an ancestor class, because (unlike methods) attributes cannot be
// redefined.  Information is also record for initialization of the
// attribute and for generating the prototype object of the class.
//
void CgenNode::layout_attribute(Symbol aname, attr_class *a,  int init)
{
  int offset = first_attribute + num_attributes++;
  var_binding_table.addid(aname, new AttributeBinding(offset));
  if (init) attribute_init_table.addid(offset,a);
  attribute_proto_table.addid(offset,a->get_type_decl());
  if (cgen_debug) cerr << "   Attribute: " << aname << " Class: " << name <<
    " offset: " << offset << " initialization : " << (init ?"yes":"no") << endl;
}

//
// The init method for a class is a bit tricky to get right.
// First, temporaries may be needed to evaluate the initialization
// of attributes; the number of temporaries needed is the max
// over the number of temps needed to calculate any attribute of the
// class.  Second, the initialization method of the parent class
// must be invoked prior to initializing any of the attributes
// of the class.
//
void CgenNode::code_init(ostream& str, CgenEnvTopLevelP e)
{
  int i;

  if (cgen_debug) cerr << "Coding init method of class " << name << 
                       " first attribute: " << first_attribute << 
                       " # attributes: " << num_attributes << endl;
  code_init_ref(str);
  str << LABEL;
  
  CalcTempP n = new CalcTemp();
  for(i = first_attribute; i < first_attribute + num_attributes; i++)
    if (attribute_init_table.lookup(i))
      // TODO n不赋以返回值么？不然n要如何更新呢
      // 已测试，无论 "n = " 加不加，都可以通过测试，有点奇怪
      /* n = */attribute_init_table.lookup(i)->calc_temps(n);
  int num_temps = n->get_max();
  if (cgen_debug) cerr << "Number of temporaries = " << num_temps << endl;

  CgenEnvClassLevelP classenv = 
    new CgenEnvClassLevel(e,
			  &method_name_to_offset_table,
			  var_binding_table,
			  name,
			  filename);

  CgenEnvironmentP env = new CgenEnvironment(classenv, nil_Formals(), num_temps);

  function_prologue(env,str);

  if (this != class_table->root())  // root has no parent
    emit_init(parent,str);

  for(i = first_attribute; i < first_attribute + num_attributes; i++)
    if (attribute_init_table.lookup(i))
      attribute_init_table.lookup(i)->code_init(str,env);

  emit_move(ACC,SELF,str);
  function_epilogue(env,0,str);

  for(List<CgenNode> *l = children; l; l = l->tl())
    l->hd()->code_init(str,e);
}    

void CgenNode::code_disptable_ref(ostream& str)
{
  emit_disptable_ref(name,str);
}

void CgenNode::code_dispatch_table(ostream& str)
{
  code_disptable_ref(str);
  str << LABEL;
  for(int i = 0; i < num_methods; i++)
    method_offset_to_binding_table.lookup(i)->code_ref(str);

  for(List<CgenNode> *l = children; l; l = l->tl())
    l->hd()->code_dispatch_table(str);
}

void CgenNode::code_protoobj_ref(ostream& str)
{
  emit_protobj_ref(name,str);
}

void CgenNode::code_prototype_object(ostream& str)
{
  // Put the -1 header for the garbage collector
  str << WORD << "-1" << endl;

  code_protoobj_ref(str);

  // The prototype object must have slots for all attributes of the
  // class, including inherited attributes.  The number of such
  // attributes is the first attribute index of the class plus the
  // number of attributes defined in the class.
  //
  int total_attributes = first_attribute + num_attributes; 
  str << LABEL
      << WORD << class_tag << endl
      << WORD << (total_attributes + DEFAULT_OBJFIELDS) << endl
      << WORD;
  code_disptable_ref(str);
  str << endl;

  for(int i = 0; i < total_attributes; i++)
  {
    str << WORD;
    
    Symbol type_decl = attribute_proto_table.lookup(i);

    if (idtable.lookup_string(INTNAME) == type_decl)
      inttable.lookup_string("0")->code_ref(str);
    else if (idtable.lookup_string(STRINGNAME) == type_decl)
      stringtable.lookup_string("")->code_ref(str);
    else if (idtable.lookup_string(BOOLNAME) == type_decl)
      falsebool.code_ref(str);
    else
      str << EMPTYSLOT;
    str << endl;
  }

  for(List<CgenNode> *l = children; l; l = l->tl())
    l->hd()->code_prototype_object(str);
}


//
// Generate code for each method in a class if the class is not basic.
// Once complete, generate code for each method of each child class
// recursively.
//
void CgenNode::code_methods(ostream& str, CgenEnvTopLevelP e)
{
  if (basic_status == NotBasic)
  {
    CgenEnvClassLevelP env = new CgenEnvClassLevel(e, 
                &method_name_to_offset_table,
                var_binding_table,
                name,
                filename);
    
    for(int i = features->first(); features->more(i); i = features->next(i))
      features->nth(i)->code_method(str,env);
  }

  for(List<CgenNode> *l = children; l; l = l->tl())
    l->hd()->code_methods(str,e);
}

void CgenNode::code_init_ref(ostream& str)
{
  emit_init_ref(name,str);
}

/////////////////////////////////////////////////////////////////////////////
//
// Methods for:
// CgenEnvTopLevel
// CgenEnvClassLevel
// CgenEnvironment
//
// Code generation for expressions is implemented using an environment that 
// gives meaning (i.e., layout information) for free names in an expression.
// In addition to free variable and method names, there is also a need to
// provide the class name and file name (needed for code that produces runtime 
// error messages), as well as a global counter for generating unique labels.
// This environment is implemented by the class CgenEnvironment.
//
// The information required for the CgenEnvironment is maintained in three
// places:  some in the CgenClassTable, some in the individual CgenNodes for
// each class, and some only in individual methods.  Thus, the CgenEnvironment
// for a method is built in stages.  The correspondence is:
//
//      CgenClassTable   defines a   CgenEnvTopLevel
//      CgenNode         defines a   CgenEnvClassLevel
//      method_class     defines a   CgenEnvironment
//
//  The CgenEnvX classes are arranged as an inheritance hierarchy, with
//  the ClassLevel inheriting from TopLevel, and Environment inheriting
//  from ClassLevel.  For the TopLevel and ClassLevel classes only 
//  constructors are provided, allowing their environment information to
//  be passed opaquely to the lower levels.  The CgenEnvironment class
//  provides the interface functions for use in coding expressions.
//
//  CgenEnvTopLevel contributes:
//          a table mapping class names to class tags
//          a table mapping class names to the max tag of any descendant class
//          a table of all method tables (class name -> method name -> offset)
//          
//  CgenEnvClassLevel constributes:
//          a table mapping methods in the current class to offsets
//          the class name
//          the name of the file where the class is defined
//          a table mapping variables to VarBindings
//
//  With the exception of the var_binding_table, tables are merely
//  referenced by the Environment methods.  Because expressions have 
//  local variables, new binding are installed and removed from the 
//  var_binding_table.  For this reason, the var_binding_table structure 
//  is copied by the CgenEnvironment constructor; all other tables are 
//  referenced through pointers.
//  
//  Each method builds its own CgenEnvironment.  The final information 
//  required is:
//          the formal parameters of the method
//          the number of temporary variables needed
//
/////////////////////////////////////////////////////////////////////////////

//
// next_label is a member of all CgenEnvironments.  It is declared static
// (shared by all instances) to guarantee that labels are unique.
//
int CgenEnvironment::next_label = 0;

CgenEnvironment::CgenEnvironment(CgenEnvClassLevelP env,
				 Formals formals,
				 int num_temporaries) :
				 CgenEnvClassLevel(*env),
				 num_temps(num_temporaries)

{
  //Location of the first local var.  See comments i
  // CgenEnvironment::get_register() to see how locals are allocated.
  next_temp_location = -1 * get_register_temps();

  // The first formal is at the highest address of the stack frame,
  // which is beyond all the temporaries (num_temporaries), the
  // callee-save area (3 words), and at the end of formals (formals->len() - 1).
  // 
  next_formal = num_temporaries + 2 + formals->len();
    
  //
  // add bindings for all of the formals to the variable environment
  //
  for(int i = formals->first(); formals->more(i); i = formals->next(i))
    add_formal(formals->nth(i)->get_name());

  // make sure there is a binding for self
  var_binding_table.addid(self,new SelfBinding()); 
}

//
// Given a class name, find the class tag.
//
int CgenEnvironment::lookup_tag(Symbol sym)
{
  int *tag = class_to_tag_table->lookup(sym);
  assert(tag);
  return *tag;
}

//
// Given a class name, find the largest tag of any descendant class.
//
int CgenEnvironment::lookup_child_tag(Symbol sym)
{
  int *tag = class_to_max_child_tag_table->lookup(sym);
  assert(tag);
  return *tag;
}
      
//
// Given a class name and a method name, find the offset of that 
// method in the class' dispatch table.  If the class name is not
// SELF_TYPE, the global table-of-method-tables is used.  Otherwise,
// the method table for the class is used.
//
int CgenEnvironment::lookup_method(Symbol classname, Symbol methodname)
{
  SymbolTable<Symbol,int> *table = 
    (classname == SELF_TYPE) ? method_name_to_offset_table :
                               table_of_method_tables->lookup(classname);
  //if (cgen_debug) cerr << "   classname == " << classname << endl;
  assert(table);
  int *offset = table->lookup(methodname);
  assert(offset);
  return *offset;
}

//
// Get the binding of a variable.  It is assumed that any variable looked
// up should actually have a binding.
//
VarBinding *CgenEnvironment::lookup_var(Symbol sym)
{
  if (cgen_debug) cerr << "    looking up binding for " << sym << endl;
  VarBinding *v = var_binding_table.lookup(sym);
  assert(v);
  return v;
}

//
//  Add a local variable.
//  A negative location is a register, a positive location is a stack slot.
//
//
void CgenEnvironment::add_local(Symbol sym)
{
  if (cgen_debug) cerr << "    Adding local binding " << sym << " " << next_temp_location << endl;
  var_binding_table.enterscope();

  //
  // verify that the coding of expressions is consistent with the
  // calculation of the number of temporaries required.
  //
  assert(next_temp_location < get_stack_temps()); 
  var_binding_table.addid(sym,new LocalBinding(next_temp_location, this));
  next_temp_location++;
}

//
// Add a formal parameter.  Formals are allocated at decreasing memory
// addresses in the formal area of the stack frame.
//
void CgenEnvironment::add_formal(Symbol sym)
{
  if (cgen_debug) cerr << "    Adding formal binding " << sym << " " << next_formal << endl;
  var_binding_table.enterscope();
  var_binding_table.addid(sym, new LocalBinding(next_formal--, this));
}

//
// Remove a local variable, freeing up its slot in the stack frame.
//
void CgenEnvironment::remove_local()
{
  if (cgen_debug) cerr << "   Removing local binding." << endl;
  var_binding_table.exitscope();
  --next_temp_location;
}

//
// Allocate a new label and bump the counter.  Note that next_label
// is static, so labels are guaranteed to be unique.
//
int CgenEnvironment::label()
{
  return next_label++;
}
  
//
// Used by LocalBinding.  If the local var denoted by the specified offset is 
// stored in a register, returns the name of that register.
//
// Returns NULL if
//  1) register allocation is disabled, or
//  2) the offset refers to a formal var, or
//  3) the offset refers a local for which there were not enough registers.
//
// Local variables are stored as follows:
// If there are enough registers for all N locals, store them in
//     sN, sN-1, ... s1.
// Otherwise, store them in
//     s6, s5,  ..., s1, 0($FP), 4($FP), ... 4*(n-6)($FP) 
// A negative value of "offset" means it's in a register.
//
Register CgenEnvironment::get_register(int offset)
{
  assert(offset >= -1 * get_register_temps()); 
  if (!disable_reg_alloc && offset < 0) {
    int reg = -1 - offset;  /// === (-offset -1)
    assert (reg < NUM_REGS);
    return regNames[reg];
  }
  else 
    return NULL;
}

//
// Returns the unallocated register that we expect to allocate to the next
// variable that is bound in the environment.
//
// e.g. we use this in "let x <- init ..." to predict where the the new
// variable x will be allocated so we can put the result of init there.
// This idea of writing to an unallocated register only works because the 
// write to the target register is the very last thing done by
//    init->code(s, env, get_next_register())
// and we will actually allocate the variable immediately afterwords.
//
Register CgenEnvironment::get_next_register()
{
  return get_register(next_temp_location);
}

//
// For function prologues and epilogues: how many registers/stack slots
// do we need?
//

// Total number of temps
int CgenEnvironment::get_num_temps() { 
  return num_temps;
}
// How many of these temps will be put in registers
int CgenEnvironment::get_register_temps() {
  if (disable_reg_alloc)
    return 0;
  else if (NUM_REGS < num_temps)
    return NUM_REGS;
  else 
    return num_temps;
}
// How many of these temps will be put on the stack
int CgenEnvironment::get_stack_temps() { 
  int result = num_temps - get_register_temps();
  assert(result >= 0);
  return result;
}


////////////////////////////////////////////////////////////////////////////
//
// APS class methods
//
////////////////////////////////////////////////////////////////////////////

void method_class::layout_feature(CgenNodeP nd)
{
  // 计算出当前method所需的tmporarie，并将当前method注册
  nd->layout_method(name, this->calc_temps());
}
void method_class::code_method(ostream& s, CgenEnvClassLevelP e)
{
  if (cgen_debug) cerr << "  Coding method: " << name << endl;
  int num_temps = calc_temps();
  
  CgenEnvironmentP env = new CgenEnvironment(e, formals, num_temps);

  emit_method_ref(env->get_classname(), name, s);
  s << LABEL;
  function_prologue(env,s);

  if (cgen_Memmgr_Debug == GC_DEBUG)
    for (int i = formals->len() - 1; i >= 0; i--)
    {
      emit_load(A1, 3 + num_temps + i, FP, s);
      emit_gc_check(A1, s);
    }

  Register result = expr->code(s,env,ACC);
  emit_move(ACC, result, s);
  function_epilogue(env, formals->len(), s);
}
void attr_class::layout_feature(CgenNodeP nd)
{
  nd->layout_attribute(name, this, !(init->no_code()));
}
void attr_class::code_method(ostream&, CgenEnvClassLevelP)
{ }
void attr_class::code_init(ostream& str, CgenEnvironmentP env)
{
 assign(name,init)->code(str,env,ACC);
}

// code_force_dest is a wrapper for the code functions that guarantees the
// result will go in "target".  Since the destination register is always
// the target, there's no need for a return value.
static
void code_force_dest(Expression e, ostream& s, CgenEnvironmentP env, 
		     Register target)
{
  Register r = e->code(s, env, target);
  emit_move(target, r, s);  //omitted if target = r.
}


/////////////////////////////////////////////////////////////////////////
//
//  code methods
//
//  expr.code(ostream s, CgenEnvironmentP env, Register target):
//    s:      where assembly code should be printed
//    env:    the context
//    target: If code() has a choice, it will put the result here.  There
//        are no guarantees; so use code_force_dest insteadif you really care.
//        On the other hand if you don't care, then ACC is a good choice.
//        NB: target may be a local variable.  So don't write to it until
//         after all other variable accesses.
//
//  Return value: the name of the register holding the result.  Possibly
//    the same as target.
//
/////////////////////////////////////////////////////////////////////////

//
// For an assignment, 
//    1. the expression is evaluated
//    2. the value is saved at the variables location
//

//******************************************************************
//
//   Fill in the following methods to produce code for the
//   appropriate expression.  You may add or remove parameters
//   as you wish, but if you do, remember to change the parameters
//   of the declarations in `cool-tree.h'  Sample code for
//   constant integers, strings, and booleans are provided.
//
//*****************************************************************

Register assign_class::code(ostream& s, CgenEnvironmentP env, Register target)
{  
  VarBinding* lhs = env->lookup_var(name);
  Register dest = lhs->get_register();
  if (dest == NULL)
  { //stack variable or attribute on the heap.  
    //So put the result somewhere else.
    dest = target;
  }
  //try to put the result in dest.
  Register rhs_value = expr->code(s,env, dest);
  lhs->code_update(rhs_value,s);
  return rhs_value;
}
//
// For a dispatch,
//    1. The arguments are evaluated and pushed on the stack.
//    2. The dispatch expression is evaluated, and the result put in $a0.
//    3. The dipatch expression is tested for void.
//    4.     If void, computation is aborted.
//    5. The dispatch table of the dispatch value is loaded.
//    6. The dispatch table is indexed with the method offset.
//    7. Jump to the method.
//
Register dispatch_class::code(ostream& s, CgenEnvironmentP env, 
			      Register target)
{
  for(int i = actual->first(); actual->more(i); i = actual->next(i))
  {
    Register r_actual = actual->nth(i)->code(s,env, ACC);
    emit_push(r_actual,s);
  }
  code_force_dest(expr, s, env, ACC);
  if (cgen_debug) cerr << "    Dispatch to " << name << endl;
  int method_offset = env->lookup_method(expr->get_type(),name);
  int lab = env->label();
  emit_bne(ACC,ZERO,lab,s);      // test for void
  emit_load_string(ACC, 
	      stringtable.lookup_string(env->get_filename()->get_string()),s);
  emit_load_imm(T1, line_number, s);
  emit_dispatch_abort(s);
  emit_label_def(lab,s);
  emit_load(T1, DISPTABLE_OFFSET, ACC, s);
  emit_load(T1, method_offset, T1, s);
  emit_jalr(T1, s);
  return ACC;
}

//
// Static dispatch has the same steps as normal dispatch, except
// the dispatch table is looked from the user-specified class.
// 
Register static_dispatch_class::code(ostream& s, CgenEnvironmentP env, 
				     Register target)
{ 
  for(int i = actual->first(); actual->more(i); i = actual->next(i))
  {
    Register r_actual = actual->nth(i)->code(s,env,ACC);
    emit_push(r_actual,s);
  }
  code_force_dest(expr, s, env, ACC);
  if (cgen_debug) cerr << "    Static dispatch to " << name << endl;
  int method_offset = env->lookup_method(type_name,name);

  int lab = env->label();
  emit_bne(ACC,ZERO,lab,s);   // test for void
  emit_load_string(ACC, 
		   stringtable.lookup_string(env->get_filename()->get_string()),s);
  emit_load_imm(T1, line_number, s);
  emit_dispatch_abort(s);
  emit_label_def(lab,s);
  emit_partial_load_address(T1,s);
  emit_disptable_ref(type_name,s);
  s << endl;
  emit_load(T1, method_offset, T1, s);
  emit_jalr(T1, s);
  return ACC;
}

Register cond_class::code(ostream& s, CgenEnvironmentP env, Register target)
{
  if(cgen_debug) cerr << "   cond_class::node " << endl;
  int else_label = env->label();
  int out_label  = env->label();

  Register r_pred = pred->code(s,env,ACC);
  // 取出条件判断后的结果
  emit_fetch_int(T1,r_pred,s);
  // 如果$t1 = 0，则跳转至else
  emit_beqz(T1,else_label,s);
  // 建立then的代码
  code_force_dest(then_exp, s, env, ACC);
  // 在then代码中添加跳出if操作
  emit_branch(out_label,s);
  // 生成else代码
  emit_label_def(else_label,s);
  code_force_dest(else_exp, s, env, ACC);
  // else代码后就是外围代码，直接添加外围代码的label
  emit_label_def(out_label,s);
  return ACC;
}

Register loop_class::code(ostream& s, CgenEnvironmentP env, Register target)  
{
  int loop_label = env->label();
  int out_label  = env->label();

  emit_label_def(loop_label,s);
  Register r_pred = pred->code(s,env,ACC);
  emit_fetch_int(T1,r_pred,s);
  emit_beq(T1,ZERO,out_label,s);
  body->code(s,env, ACC); //ignore the result
  emit_branch(loop_label,s);
  emit_label_def(out_label,s);
  emit_move(ACC,ZERO,s);       // value of while is void
  return ACC;

}

//
// for a case expression, the cases are tested in the order
// of most specific to least specific.  Since tags are assigned
// in depth-first order with the root being assigned 0, higher-numbered
// classes should be coded before lower-numbered classes.
//
Register typcase_class::code(ostream& s, CgenEnvironmentP env, Register target)
{
  int out_label = env->label();

  Register r_expr = expr->code(s,env,ACC);
  int lab = env->label();
  emit_bne(r_expr,ZERO,lab,s);      // test for void
  emit_load_string(ACC, 
		   stringtable.lookup_string(env->get_filename()->get_string()),s);
  emit_load_imm(T1, line_number, s);
  emit_case_abort2(s);
  emit_label_def(lab,s);
  emit_load(T2, TAG_OFFSET, r_expr, s);  // fetch the class tag

  for(int class_num = env->get_num_classes(); class_num >= 0; class_num--)
    for(int i = cases->first(); cases->more(i); i = cases->next(i))
	{
	  int tag = env->lookup_tag(cases->nth(i)->get_type_decl());
	  if (class_num == tag)
	    {
	      if (cgen_debug) cerr << "    Coding case " << 
				cases->nth(i)->get_type_decl() << endl;
	                
	      cases->nth(i)->code(s,env,out_label, r_expr); //result in ACC
	    }
	}
  emit_case_abort(s);
  emit_label_def(out_label,s);
  return ACC;

}

// r_newvar is the value that we did the case on.  It will be bound to the
// new var.
void branch_class::code(ostream& s, CgenEnvironmentP env, int outlabel, 
			Register r_newvar)
{
  int lab = env->label();
  int class_tag = env->lookup_tag(type_decl);
  int last_tag  = env->lookup_child_tag(type_decl);
  emit_blti(T2, class_tag, lab, s);
  emit_bgti(T2, last_tag, lab, s);
  env->add_local(name);
  env->lookup_var(name)->code_update(r_newvar, s);
  code_force_dest(expr,s,env,ACC);
  env->remove_local();
  emit_branch(outlabel,s);
  emit_label_def(lab,s);
}

Register block_class::code(ostream& s,  CgenEnvironmentP env, Register target)
{
  Register r_lastvalue = ACC;
  for(int i = body->first(); body->more(i); i = body->next(i))
     r_lastvalue = body->nth(i)->code(s,env, ACC);
  return r_lastvalue;
}

Register let_class::code(ostream& s,  CgenEnvironmentP env, Register target)  
{
  //r_newvar is the register to which we think the new variable will be
  //  assigned (NULL if register allocation is disabled or no regs availible).
  //r_init is the register that holds the result of the init expr.  We'd like 
  //  r_init to be the same as r_newvar.
  Register r_newvar = env->get_next_register();
  Register r_init = r_newvar;
  if (r_init == NULL){
    r_init = ACC;
  }

  if (init->no_code())
  {
    if (idtable.lookup_string(INTNAME) == type_decl)
    {
      emit_partial_load_address(r_init,s);
      inttable.lookup_string("0")->code_ref(s);
      s << endl;
    }
    else if (idtable.lookup_string(STRINGNAME) == type_decl)
    {
      emit_partial_load_address(r_init,s);
      stringtable.lookup_string("")->code_ref(s);
      s << endl;
    }
    else if (idtable.lookup_string(BOOLNAME) == type_decl)
    {
      emit_partial_load_address(r_init,s);
      falsebool.code_ref(s);
      s << endl;
    }
    else
    {
      r_init = ZERO;
    }
  }
  else
    {
      r_init = init->code(s,env,r_init);
    }

  //Register r_init now holds the location of the value to which newvar should
  //be initialized.  Hopefully, r_init and newvar are one and the same, in
  //which case the code_update is a nop.
  env->add_local(identifier);
  VarBinding* newvar = env->lookup_var(identifier);
  newvar->code_update(r_init,s);

  //test that get_next_register really returned the register to which newvar
  //was assigned.
  assert( regEq(newvar->get_register(), r_newvar) );

  Register r_body = body->code(s,env,target);
  env->remove_local();
  return r_body;
}
// Helper for "e1 op e2"
//
// The contents of the register that holds e1 could change when
// e2 is executed, so we need to save the result of the first computation.
// This function:
//   1) evaluates e1
//   2) allocates a new var
//   3) puts the result of e1 in that new var.
//
// The caller of store_operand function should deallocate the new variable.
static void store_operand(Symbol temp_var, Expression e1, 
			  ostream& s,  CgenEnvironmentP env)  
{
  Register dest = env->get_next_register(); //where will temp_var be allocated?
  if (dest == 0)
  { //whoops, temp_var is going on the stack
    dest = ACC;
  }
  Register r_e1 = e1->code(s,env,dest);//r_e1 <- e1, where hopefully r_e1=dest
  env->add_local(temp_var);
  env->lookup_var(temp_var)->code_update(r_e1, s);
  
  //sanity check for get_next_register:
  assert( regEq(dest, ACC) 
	  || regEq(dest, env->lookup_var(temp_var)->get_register()));
}

//helper for plus_class::code and other integer binary ops
static
Register binop_int_code(char* binop, Expression e1, Expression e2,
			ostream& s,  CgenEnvironmentP env)  
{ 
  Symbol temp1 = idtable.add_string(TEMP1);
  store_operand(temp1, e1, s, env);  //temp1 <- e1

  code_force_dest(e2, s, env, ACC);
  emit_copy(s);
  //now fetch e1 back out of its temporary:
  Register r_temp1 =  env->lookup_var(temp1)->code_ref(T1,s);
  emit_fetch_int(T2,ACC,s);  //ACC holds the copy of r2
  emit_fetch_int(T1,r_temp1,s);
  emit_binop(binop,T1,T1,T2,s); // result.value <- temp1.value (op) e2.value
  emit_store_int(T1,ACC,s);
  env->remove_local();
  return ACC;
}

Register plus_class::code(ostream& s,  CgenEnvironmentP env, Register target)  
{
  return binop_int_code(ADD, e1, e2, s, env);
}

Register sub_class::code(ostream& s,  CgenEnvironmentP env, Register target)  
{
  return binop_int_code(SUB, e1, e2, s, env);
}

Register mul_class::code(ostream& s,  CgenEnvironmentP env, Register target)  
{
//matth: this is inconsistent:  I don't know why multiply is done 
//without checking overflow while the others do perform a check.
  return binop_int_code(MUL, e1, e2, s, env);
}

Register divide_class::code(ostream& s,  CgenEnvironmentP env, Register target)
{
  return binop_int_code(DIV, e1, e2, s, env);
}

Register neg_class::code(ostream& s, CgenEnvironmentP env, Register target)  
{
  code_force_dest(e1,s,env,ACC);
  emit_copy(s);
  emit_fetch_int(T1,ACC,s);
  emit_neg(T1,T1,s);
  emit_store_int(T1,ACC,s);
  return ACC;
}

Register lt_class::code(ostream& s, CgenEnvironmentP env, Register target)  
{
  int lab_done = env->label();
  Symbol temp1 = idtable.add_string(TEMP1);
  store_operand(temp1, e1, s, env);  //temp1 <- e1

  Register r_e2 = e2->code(s,env,ACC);
  Register r_e1 = env->lookup_var(temp1)->code_ref(T1,s);
  emit_fetch_int(T1,r_e1,s);
  emit_fetch_int(T2,r_e2,s);
  emit_load_bool(ACC,truebool,s);
  emit_blt(T1,T2,lab_done,s);
  emit_load_bool(ACC,falsebool,s);
  emit_label_def(lab_done,s);
  env->remove_local();
  return ACC;
}


//The calling convention for equality_test:
//#  INPUT: The two objects are passed in $t1 and $t2
//#  OUTPUT: Initial value of $a0, if the objects are equal
//#          Initial value of $a1, otherwise

Register eq_class::code(ostream& s, CgenEnvironmentP env, Register target)  
{
  if(cgen_debug) cerr << "   eq_class::node " << endl;
  int lab = env->label();
  Symbol temp1 = idtable.add_string(TEMP1);
  store_operand(temp1, e1, s, env);  //temp1 <- e1

  Register r_e2 = e2->code(s,env,T2);
  Register r_e1 = env->lookup_var(temp1)->code_ref(T1,s);
  emit_move(T1, r_e1, s);
  emit_move(T2, r_e2, s);
  emit_load_bool(ACC,truebool,s);
  emit_beq(T1,T2,lab,s); // TODO 未知用途，不可缺少
  emit_load_bool(A1,falsebool,s);
  // 判断两寄存器的值是否等同
  emit_equality_test(s); //result in ACC
  // 建立then的label
  emit_label_def(lab,s);
  env->remove_local();
  return ACC; 
}

Register leq_class::code(ostream& s, CgenEnvironmentP env, Register target)  
{
  int lab_done = env->label();
  Symbol temp1 = idtable.add_string(TEMP1);
  store_operand(temp1, e1, s, env);  //temp1 <- e1

  Register r_e2 = e2->code(s,env,ACC);
  Register r_e1 = env->lookup_var(temp1)->code_ref(T1,s);
  emit_fetch_int(T1,r_e1,s);
  emit_fetch_int(T2,r_e2,s);
  emit_load_bool(ACC,truebool,s);
  emit_bleq(T1,T2,lab_done,s);
  emit_load_bool(ACC,falsebool,s);
  emit_label_def(lab_done,s);
  env->remove_local();
  return ACC;
}

Register comp_class::code(ostream& s, CgenEnvironmentP env, Register target)  
{
  int lab = env->label();
  Register r_e1 = e1->code(s,env,ACC);
  emit_fetch_int(T1,r_e1,s);
  emit_load_bool(ACC,truebool,s);
  emit_beqz(T1,lab,s);
  emit_load_bool(ACC,falsebool,s);
  emit_label_def(lab,s);
  return ACC;
}

Register int_const_class::code(ostream& s, CgenEnvironmentP, Register target)  
{
  //
  // Need to be sure we have an IntEntry *, not an arbitrary Symbol
  //
  emit_load_int(target,inttable.lookup_string(token->get_string()),s);
  return target;
}

Register string_const_class::code(ostream& s, CgenEnvironmentP,
				  Register target)
{
  emit_load_string(target,stringtable.lookup_string(token->get_string()),s);
  return target;
}

Register bool_const_class::code(ostream& s, CgenEnvironmentP, Register target)
{
  emit_load_bool(target, BoolConst(val), s);
  return target;
}

Register new__class::code(ostream& s, CgenEnvironmentP env, Register target)
{
  if (type_name == SELF_TYPE)
  {
    emit_load_address(T1, CLASSOBJTAB,s);
    emit_load(T2, TAG_OFFSET, SELF, s);        // load the class tag
    emit_sll(T2, T2, LOG_WORD_SIZE + 1, s);    // each entry is 2 words
                                                //   or (2^LWS)*2 bytes
    emit_addu(T1,T1,T2,s);                     // index into the table
    Symbol temp1 = idtable.add_string(TEMP1);
    env->add_local(temp1);
    env->lookup_var(temp1)->code_update(T1,s); //save $t1 before calling copy
    emit_load(ACC, 0, T1, s);                  // get self prototype object
    emit_copy(s);                              // copy the prototype
    Register r_temp1 = 
    env->lookup_var(temp1)->code_ref(T1,s);  // reload temp1
    emit_load(T1, 1, r_temp1, s);              // address of init code
    env->remove_local();
    emit_jalr(T1,s);                           // initialize object
  }
  else
  {
    emit_partial_load_address(ACC,s);
    emit_protobj_ref(type_name,s);
    s << endl;
    emit_copy(s);
    emit_init(type_name,s);
  }
  return ACC;
}

Register object_class::code(ostream& s, CgenEnvironmentP env, Register target)
{
  return env->lookup_var(name)->code_ref(target,s);
}

Register no_expr_class::code(ostream&, CgenEnvironmentP, Register)
{
  cerr << "Reached code() of no_expr." << endl;
  exit(1);
}

Register isvoid_class::code(ostream& s, CgenEnvironmentP env, Register target)
{
  int lab = env->label();

  Register r_e1 = e1->code(s,env,ACC);
  if(regEq(r_e1, target))
    {
      emit_move(T1,r_e1,s);
      r_e1 = T1;
    }
  emit_load_bool(target,truebool,s);
  emit_beqz(r_e1,lab,s);
  emit_load_bool(target,falsebool,s);
  emit_label_def(lab,s);
  return target;
}


/////////////////////////////////////////////////////////////////////////
//
//  CalcTemp methods
//
// calculate the number of temporaries needed to evaluate an expression,
// assuming all computation takes place in a distinguished register
// $a0.   The Cool features which reuquire temporaries are:
//
//   1. binary arithmetic & relational operators 
//      (+,*,/,-, <, <=, =)
//
//   2. constructs that introduce new bindings
//      (let, case)
//
//   3. new SELF_TYPE
//
/////////////////////////////////////////////////////////////////////////

CalcTempP attr_class::calc_temps(CalcTemp* n)
{ return init->calc_temps(n); }

int method_class::calc_temps()
{ return expr->calc_temps(new CalcTemp())->get_max(); }

// 常量是不需要临时变量的
CalcTempP int_const_class::calc_temps(CalcTempP n)
{ return n; }

CalcTempP bool_const_class::calc_temps(CalcTempP n)
{ return n; }

CalcTempP string_const_class::calc_temps(CalcTempP n)
{ return n; }

CalcTempP plus_class::calc_temps(CalcTempP n)
// 之所以CalcTemp会先add后sub，是因为这样可以模拟临时变量的使用
// add和sub的对象是当前已经使用的寄存器数量
// 当计算出e1的值后，需要一个临时变量来进行存储
// 但当计算出e2后，就不再需要该临时变量了
{ return e2->calc_temps(e1->calc_temps(n)->add())->sub(); }

CalcTempP sub_class::calc_temps(CalcTempP n)
{ return e2->calc_temps(e1->calc_temps(n)->add())->sub(); }

CalcTempP mul_class::calc_temps(CalcTempP n)
{ return e2->calc_temps(e1->calc_temps(n)->add())->sub(); }

CalcTempP divide_class::calc_temps(CalcTempP n)
{ return e2->calc_temps(e1->calc_temps(n)->add())->sub(); }

CalcTempP neg_class::calc_temps(CalcTempP n)
{ return e1->calc_temps(n); }

// 比较大小，本质上和四则运算是一样的
CalcTempP lt_class::calc_temps(CalcTempP n)
{ return e2->calc_temps(e1->calc_temps(n)->add())->sub(); }

CalcTempP leq_class::calc_temps(CalcTempP n)
{ return e2->calc_temps(e1->calc_temps(n)->add())->sub(); }

CalcTempP comp_class::calc_temps(CalcTempP n)
{ return e1->calc_temps(n); }

CalcTempP object_class::calc_temps(CalcTempP n)
{ return n; }

CalcTempP no_expr_class::calc_temps(CalcTempP n)
{ return n; }

// new uses one temporary in the case of new SELF_TYPE
CalcTempP new__class::calc_temps(CalcTempP n)
// DONE 暂且没搞懂为什么self_type需要一个临时变量
// self prototype object，请结合new__class::code函数理解
{ if (type_name == SELF_TYPE)
    return n->add()->sub();
  else 
    return n;
}

CalcTempP isvoid_class::calc_temps(CalcTempP n)
{ return e1->calc_temps(n); }

CalcTempP eq_class::calc_temps(CalcTempP n)
{ return e2->calc_temps(e1->calc_temps(n)->add())->sub(); }

CalcTempP let_class::calc_temps(CalcTempP n)
{ return body->calc_temps(init->calc_temps(n)->add())->sub(); }
 
CalcTempP block_class::calc_temps(CalcTempP n)
{
  for(int i = body->first(); body->more(i); i = body->next(i))
    n = body->nth(i)->calc_temps(n);
  return n;
}

CalcTempP assign_class::calc_temps(CalcTempP n)
{ return expr->calc_temps(n); }

CalcTempP dispatch_class::calc_temps(CalcTempP n)
{
  n = expr->calc_temps(n);
  for(int i = actual->first(); actual->more(i); i = actual->next(i))
    n = actual->nth(i)->calc_temps(n);
  return n;
}

CalcTempP static_dispatch_class::calc_temps(CalcTempP n)
{
  n = expr->calc_temps(n);
  for(int i = actual->first(); actual->more(i); i = actual->next(i))
    n = actual->nth(i)->calc_temps(n);
  return n;
}

CalcTempP cond_class::calc_temps(CalcTempP n)
{ return else_exp->calc_temps(then_exp->calc_temps(pred->calc_temps(n))); }

CalcTempP loop_class::calc_temps(CalcTempP n)
{ return body->calc_temps(pred->calc_temps(n)); }

CalcTempP typcase_class::calc_temps(CalcTempP n)
{
  n = expr->calc_temps(n);
  for (int i=cases->first(); cases->more(i); i = cases->next(i))
    n = cases->nth(i)->calc_temps(n);
  return n;
}

CalcTempP branch_class::calc_temps(CalcTempP n)
{
  return expr->calc_temps(n->add())->sub();
}


///////////////////////////////////////////////////////////////////////////

bool isAllocatedReg(Register reg) {
  for (int i = 0; i < NUM_REGS; i++) 
    {
      if (reg == regNames[i])
	return true;
    }
  return false;
}
