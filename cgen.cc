
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
#include <sstream>
#include <stack>
#include <map>
#include <stdlib.h>
#include <stdio.h>

using std::map;
using std::stringstream;
using std::pair;
using std::make_pair;
using std::stack;

#define PRINT(x) if (cgen_debug) cout << x << endl

extern void emit_string_constant(ostream& str, char *s);
extern int cgen_debug;
int label_count = 0;
int cur_offset = 0;
bool isNoExpr = false; //This goes off the deep end from bad to horrible style

map<Symbol, vector<pair<string, string> >* > symToNode;
map<Symbol, int> attrsAbove;
map<Symbol, vector<string> > attrLookup;
map<int, int> subClassRange; //This is a mapping from classTag to minimum class Tag that is a subclass of this class
map<Symbol, int> classTagLookup;

                    //isHeap,offset
SymbolTable<string,pair<bool, int> > variableOffsets; 

SymbolTable<string, Symbol> variableTypes;


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

static void emit_load_address(char *dest_reg,const char *address, ostream& s)
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

static void emit_move(char *dest_reg, char *source_reg, ostream& s)
{ s << MOVE << dest_reg << " " << source_reg << endl; }

static void emit_neg(char *dest, char *src1, ostream& s)
{ s << NEG << dest << " " << src1 << endl; }

static void emit_add(char *dest, char *src1, char *src2, ostream& s)
{ s << ADD << dest << " " << src1 << " " << src2 << endl; }

static void emit_addu(char *dest, char *src1, char *src2, ostream& s)
{ s << ADDU << dest << " " << src1 << " " << src2 << endl; }

static void emit_addiu(char *dest, char *src1, int imm, ostream& s)
{ s << ADDIU << dest << " " << src1 << " " << imm << endl; }

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

static void emit_jal(const char *address,ostream &s)
{ s << JAL << address << endl; }

static void emit_return(ostream& s)
{ s << RET << endl; }

static void emit_gc_assign(ostream& s)
{ s << JAL << "_GenGC_Assign" << endl; }

static void emit_disptable_ref(Symbol sym, ostream& s)
{  s << sym << DISPTAB_SUFFIX; }

static void emit_init_ref(Symbol sym, ostream& s)
{ s << sym << CLASSINIT_SUFFIX; }

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

static void emit_blt(const char *src1, const char *src2, int label, ostream &s)
{
  s << BLT << src1 << " " << src2 << " ";
  emit_label_ref(label,s);
  s << endl;
}

static void emit_bgt(const char *src1, const char *src2, int label, ostream &s)
{
  s << BGT << src1 << " " << src2 << " ";
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


/*
 * pop into register reg
 *
 * It bugs me that they didn't code this for us...
 */
static void emit_pop(char *reg, ostream& str)
{

    emit_addiu(SP,SP,4,str);
    emit_load(reg,0,SP,str);
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

static void emit_wind(ostream &s){

    //NOTE TO FUTURE SELF(S):
    //
    //If we ever adjust this to store more variables, 
    //we need to change the argument code that puts them
    //into the scope. Search for the tag FIXTHIS if 
    //that happens.

    emit_addiu(SP, SP, -12, s);
    emit_store(FP, 3, SP, s);
    emit_store(SELF, 2, SP, s);
    emit_store(RA, 1, SP, s);
    emit_addiu(FP, SP, 4, s);
    emit_move(SELF, ACC, s);

}

static void emit_unwind(ostream &s, int x){
    ///
    emit_load(FP, 3, SP, s);
    emit_load(SELF, 2, SP, s);
    emit_load(RA, 1, SP, s);
    emit_addiu(SP, SP, 12 + x * 4, s);
    emit_return(s);
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
      << WORD << STRINGNAME << DISPTAB_SUFFIX << endl;              //dispatch table


 /***** Add dispatch information for class String ******/

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
      << WORD << INTNAME << DISPTAB_SUFFIX << endl;              //dispatch table
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
      << WORD << BOOLNAME << DISPTAB_SUFFIX << endl;              //dispatch table

 /***** Add dispatch information for class Bool ******/

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
      << WORD << intclasstag << endl;
  str << BOOLTAG << LABEL 
      << WORD << boolclasstag << endl;
  str << STRINGTAG << LABEL 
      << WORD << stringclasstag << endl;    
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

void CgenClassTable::code_bools(int boolclasstag)
{
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
  stringtable.add_string("");
  inttable.add_string("0");

  stringtable.code_string_table(str,stringclasstag);
  inttable.code_string_table(str,intclasstag);
  code_bools(boolclasstag);
}

int CgenClassTable::generate_class_tags(CgenNodeP obj, int curTag){

    List<CgenNode> *children = obj->get_children();
    
    if(children){ 
        for(; children; children = children->tl()){
            curTag = generate_class_tags(children->hd(), curTag);
        }
    }

    curTag++;
    PRINT(" class " << obj->name << " has " << curTag);
    
    classTagLookup[obj->name] = curTag;
    
    children = obj->get_children();
    if(children){
        subClassRange[curTag] = subClassRange[classTagLookup[children->hd()->name]];
    }else{
        subClassRange[curTag] = curTag;
    }
    
    PRINT(" subclass range for " << obj->name << "  " << curTag << " - " << subClassRange[subClassRange[curTag]]);
    
    if(obj->name == Str){
        stringclasstag = curTag;
    }else if(obj->name == Int){
        intclasstag = curTag;
    }else if(obj->name == Bool){
        boolclasstag = curTag;
    }

    return curTag;
}

CgenClassTable::CgenClassTable(Classes classes, ostream& s) : nds(NULL) , str(s)
{
   // TODO: Change this
   stringclasstag = 99 /* Change to your String class tag here */;
   intclasstag =    99 /* Change to your Int class tag here */;
   boolclasstag =   99 /* Change to your Bool class tag here */;

   this->curNumber = 6;

   enterscope();
   if (cgen_debug) cout << "Building CgenClassTable" << endl;
   install_basic_classes();
   install_classes(classes);
   build_inheritance_tree();
   generate_class_tags(this->root(), 0);

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
  PRINT(n->name);
  children = new List<CgenNode>(n,children);
}


void CgenNode::set_parentnd(CgenNodeP p)
{
  assert(parentnd == NULL);
  assert(p != NULL);
  parentnd = p;
}

Symbol curClass = Object;

void CgenClassTable::code_nameTab(CgenNodeP obj){

    //classTagLookup[obj->name]; 

    //Need to reverse this, starting at 0 and going up until we've found them all.

    map<int, Symbol> reverseLookup;

    map<Symbol, int>::iterator it;

    int numSymbols = 0;
    for (it = classTagLookup.begin(); it != classTagLookup.end(); it++){
        ++numSymbols;
        reverseLookup[it->second] = it->first;
    }

    int j=0;
    for (int i=0;j<numSymbols;i++){
        if (reverseLookup.count(i) > 0){
            j++;
            str << WORD;
            stringtable.lookup_string(reverseLookup[i]->get_string())->code_ref(str);
            str << endl;
        } else {
            str << WORD;
            stringtable.lookup_string("")->code_ref(str);
            str << endl;
        }
    }

    /* 
    List<CgenNode> *children = obj->get_children();

    if (!children) return; //TODO

    //stack<CgenNodeP> s;

    for(; children; children = children->tl()){
        //s.push(children->hd());
        code_nameTab(children->hd());
    }
  */
}
                                                        //pair< class, method > 
void CgenClassTable::code_objTab(CgenNodeP obj){

    str << WORD << obj->name << "_protObj" << endl;
    str << WORD << obj->name << "_init" << endl;
    
    List<CgenNode> *children = obj->get_children();

    if (!children) return; //TODO

    //stack<CgenNodeP> s;

    for(; children; children = children->tl()){
        //s.push(children->hd());
        code_objTab(children->hd());
    }
    /*
    while(s.size()){ 
        code_objTab(s.top());
        s.pop();
    }
    */
}

void CgenClassTable::code_proto(CgenNodeP obj, vector<string> attrTbl, vector<string> attrNameTbl){
    attrsAbove[obj->name] = attrTbl.size();

    str << WORD << "-1" << endl;
    str << obj->name << "_protObj:" <<  endl;
    str << WORD << classTagLookup[obj->name] << endl; //TODO
    int objSize = 3;
    
    PRINT("Generating for class : " << obj->name);

    Features features = obj->features; 
    for(int i=0; i < features->len(); i++){
        if(!features->nth(i)->method){
            stringstream sstype, ssname;
            sstype << features->nth(i)->type_decl; 
            ssname << features->nth(i)->name;
            attrTbl.push_back(sstype.str());
            attrNameTbl.push_back(ssname.str());
        }
    }
    
    attrLookup[obj->name] = attrNameTbl; //copy

    objSize += (int)attrTbl.size();
    str << WORD << objSize << endl;
    str << WORD << obj->name << "_dispTab" << endl;
    
    for(int i=0;i<(int)attrTbl.size();i++){
        if(attrTbl[i] == "Int"){
            str << WORD;
            inttable.lookup_string("0")->code_ref(str);
            str << endl;
        }else if(attrTbl[i] == "String"){
            str << WORD;
            stringtable.lookup_string("")->code_ref(str);
            str << endl;
        }else if(attrTbl[i] == "Bool"){
            str << WORD;
            BoolConst(0).code_ref(str); // TODO :: test this
            str << endl;
        }else{
            str << WORD << "0" << endl;
        }
    }

    //map<Symbol, vector<string>* > symToAttrTbl;


    List<CgenNode> *children = obj->get_children();

    if (!children) return; //TODO

    stack<CgenNodeP> s;

    for(; children; children = children->tl()){
        s.push(children->hd());
    }
    while(s.size()){ 
        code_proto(s.top(), attrTbl, attrNameTbl);
        s.pop();
    }

}

void initialize_default_value(Symbol type_decl, ostream &s){

    if (type_decl != Int &&
        type_decl != Str &&
        type_decl != Bool){

        emit_move(ACC, ZERO, s);
        return;
    }
    s << "\tla\t" << ACC << "\t";
    if (type_decl == Int){
        inttable.lookup_string("0")->code_ref(s);
    } else if (type_decl == Str){
        stringtable.lookup_string("")->code_ref(s);
    } else if (type_decl == Bool){
        BoolConst(0).code_ref(s); // TODO :: test this
    } 
    s << endl;
    //emit_jal("Object.copy", s);
}

void CgenClassTable::code_init(CgenNodeP obj){
    str << obj->name <<  CLASSINIT_SUFFIX << ":" << endl;
    emit_wind(str);
    curClass = obj->name;
    
    Features features = obj->features;
    
    variableOffsets.enterscope();
    variableTypes.enterscope();
    PRINT("Creating a class..." << endl); 
    vector<string> attrs = attrLookup[obj->name]; 
    for (int i=0;i<(int)attrs.size();i++){ //Add variable into scope at correct offset
        pair<bool, int>* p = new pair<bool, int>(true, i+3);
        PRINT(" attr : " << attrs[i] << endl);
        variableOffsets.addid(attrs[i], p); //TODO offset+3?
    }
    
    for (int i=0; i<(int)attrs.size();i++){
        PRINT(i << endl);
        if (i < features->len() && !features->nth(i)->method){
            PRINT(features->nth(i)->type_decl);
            variableTypes.addid(features->nth(i)->name->get_string(), &features->nth(i)->type_decl);
        }
    }
    
    PRINT("Done adding types");
        
    if(obj->parent != No_class){
        PRINT("SOMETHING MYSTERIOUS");
        stringstream s;
        s << obj->parent << CLASSINIT_SUFFIX;
        emit_jal(s.str().c_str(), str);
    }

    //cout << "attrs above " << obj->name << " : " << attrsAbove[obj->name] << endl;

    int featuresSeen = 0;

    for (int i=0; i< features->len(); i++){
        if (!features->nth(i)->method && !obj->basic() ){
            attr_class *a = (attr_class *) features->nth(i);

            initialize_default_value(a->type_decl, str); //TODO, hmm.
            a->init->code(str);
            int offset = attrsAbove[obj->name] + featuresSeen + 3;
            ++featuresSeen;
            
            emit_store(ACC, offset, SELF, str);
        }
    }

    emit_move(ACC, SELF, str);
    emit_unwind(str, 0);

    variableOffsets.exitscope();
    variableTypes.exitscope();

    List<CgenNode> *children = obj->get_children();

    if (!children) return; //TODO
    //this->dispatch_table = tbl;

    stack<CgenNodeP> s;

    for(; children; children = children->tl()){
        s.push(children->hd());
    }
    while(s.size()){ 
        code_init(s.top());
        s.pop();
    }
}
                                                        //pair< class, method > 
void CgenClassTable::code_dispatch(CgenNodeP obj, vector<pair<string, string> > tbl){

    Features features = obj->features;
    str << obj->name << "_dispTab:" << endl;
    ///Loop through the features of a class         
    for (int i=0;i < features->len(); i++){
        if (features->nth(i)->method){ 
            stringstream ssobj, ssnm;
            ssobj << obj->name;
            ssnm << features->nth(i)->name; //TODO change if inherited?
            int j;
            for (j=0;j < (int)tbl.size(); j++) if ( tbl[j].second == ssnm.str()) break;
            //Method override
            if (j != (int)tbl.size()){ 
                tbl[j].first = ssobj.str();
            }
            else {
                tbl.push_back(make_pair(ssobj.str(), ssnm.str()));
            }
        }
        //Define a method on features that returns name of method 
    }

    for (int i=0;i < (int)tbl.size() ;i++){
        str << WORD << tbl[i].first << "." << tbl[i].second << endl;
    }

    obj->functions = tbl;

    List<CgenNode> *children = obj->get_children();

    if (!children) return; //TODO
    //this->dispatch_table = tbl;

    stack<CgenNodeP> s;

    for(; children; children = children->tl()){
        s.push(children->hd());
    }
    while(s.size()){ 
        code_dispatch(s.top(), tbl);
        s.pop();
    }

}


void CgenClassTable::code_method(CgenNodeP obj){

    variableOffsets.enterscope();
    variableTypes.enterscope();

    if (!obj->basic()) { 
        
        curClass = obj->name;
        Features features = obj->features;
        //add all attrs to symtab
        
        //FIXME loop through the attr table that we create

        vector<string> attrs = attrLookup[obj->name]; 
        for (int i=0;i<attrs.size();i++){
            //cout << attrs[i] << endl;
        }
        for (int i=0;i<(int)attrs.size();i++){ //Add variable into scope at correct offset
            pair<bool, int>* p = new pair<bool, int>(true, i+3);
            variableOffsets.addid(attrs[i], p); //TODO offset+3?
        }
        for (int i=0; i<(int)attrs.size();i++){
            if (i < features->len() && !features->nth(i)->method)
                variableTypes.addid(features->nth(i)->name->get_string(), &features->nth(i)->type_decl);
        }
        
        for (int i=0;i < features->len(); i++){
            if (features->nth(i)->method){ 
                str << obj->name << "." << features->nth(i)->name << ":" << endl;
                emit_wind(str);
                //MARK: "FIXTHIS"
                method_class *method = (method_class*) features->nth(i); 
                //Add all arguments into the scope.
                for (int j=method->formals->len()-1; j>=0;j--){ //FIXED???
                    formal_class *formal = (formal_class *)method->formals->nth(j);
                    pair<bool, int>* p = new pair<bool, int>(false, 3 + method->formals->len() - j - 1); 
                    variableOffsets.addid(formal->name->get_string(), p); 
                    variableTypes.addid(formal->name->get_string(), 
                                       &formal->type_decl);
                }
                method->expr->code(str);
                emit_unwind(str, method->formals->len() );
            } 
            //Define a method on features that returns name of method 
        }
    }

    variableOffsets.exitscope();
    variableTypes.exitscope();

    List<CgenNode> *children = obj->get_children();

    if (!children) return; //TODO

    stack<CgenNodeP> s;

    for(; children; children = children->tl()){
        s.push(children->hd());
    }
    while(s.size()){ 
        code_method(s.top());
        s.pop();
    }
}

void CgenClassTable::code()
{
  if (cgen_debug) cout << "coding global data" << endl;
  code_global_data();

  if (cgen_debug) cout << "choosing gc" << endl;
  code_select_gc();

  if (cgen_debug) cout << "coding constants" << endl;
  code_constants();
    
  vector<pair<string, string> > emptyDispTbl;
  vector<string> emptyAttrTbl;
  vector<string> emptyAttrNameTbl; 
  str << CLASSNAMETAB << ":" << endl; 
  code_nameTab(this->root());

  str << CLASSOBJTAB << ":" << endl; 
  code_objTab(this->root());


  code_dispatch(this->root(), emptyDispTbl);
  code_proto(this->root(), emptyAttrTbl, emptyAttrNameTbl);


//                 Add your code to emit
//                   - prototype objects
//                   - class_nameTab
//                   - dispatch tables
//

  if (cgen_debug) cout << "coding global text" << endl;
  code_global_text();
    
  code_init(this->root());
//                 Add your code to emit
//                   - object initializer
//                   - the class methods
//                   - etc...

    code_method(this->root());

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
   basic_status(bstatus)
{ 
    PRINT(" adding " << name->get_string());
    string idname = name->get_string();
    if (idname == "SELF_TYPE" || idname == "_no_class" || idname == "_prim_slot") return; //TODO: special cases?

   stringtable.add_string(name->get_string());          // Add class name to string table


    //map<Symbol, CgenNode> symToNode;
    symToNode[name] = &functions;
}


//******************************************************************
//
//   Fill in the following methods to produce code for the
//   appropriate expression.  You may add or remove parameters
//   as you wish, but if you do, remember to change the parameters
//   of the declarations in `cool-tree.h'  Sample code for
//   constant integers, strings, and booleans are provided.
//
//*****************************************************************

#define LOADTYPE 0
#define STORETYPE 1

void emit_loadstore_var(char *id, ostream &s, int type){
    pair<bool, int> res = *(variableOffsets.lookup(id));

    if (res.first == true){ //variable is on the heap
        if (type==LOADTYPE){
            emit_load(ACC, res.second, SELF, s);
        } else { //type == STORETYPE
            emit_store(ACC, res.second, SELF, s);
            //GC
            if(cgen_Memmgr != 0){
                emit_addiu(A1, SELF, res.second * 4, s);
                emit_jal("_GenGC_Assign", s);
            }
        }

    } else {
        if (type==LOADTYPE){
            emit_load(ACC, res.second, FP, s); // store the local variable on the stack
        } else {
            emit_store(ACC, res.second, FP, s);
        }
        //stack handling here
    }
}


/*
 * emits a load of this variable into ACC.
 *
 * Has at least 2 special cases (stack vs heap), maybe more
 */
void emit_load_variable(char *id, ostream &s){
    emit_loadstore_var(id, s, LOADTYPE);
}
/*
 * The same thing except it stores ACC into where the var is.
 */
void emit_store_variable(char *id, ostream &s){
    emit_loadstore_var(id, s, STORETYPE);
}

void emit_check_acc_null(int i, ostream &s){
    int jump_label = label_count++;
	emit_bne(ACC, ZERO, jump_label, s);
    emit_load_address(ACC, "str_const0", s); //TODO: This is wrong. (Should be the str that contains the current file name.)
    emit_load_imm(T1, i, s);
    emit_jal("_dispatch_abort", s);
    emit_label_def(jump_label, s);
}

void assign_class::code(ostream &s) {
    expr->code(s); //result stored in ACC
    emit_jal("Object.copy", s); //copy 
    emit_store_variable(name->get_string(), s);
}


int lookupMethod(Symbol classname, Symbol name){
    vector<pair<string, string> > vect = *symToNode[classname];
    for (int i=0;i< (int)vect.size(); i++){
        PRINT(vect[i].second);
        if (vect[i].second == name->get_string()){
            return i;
        }
    }
   return 99999999;// this will never happen
}

//TODO: Check this
//Do we have to use type_name for something? 
void static_dispatch_class::code(ostream &s) {
    for(int i=0; i<actual->len(); i++){ //FIXED
       actual->nth(i)->code(s);
       emit_push(ACC, s);
    }

    expr->code(s);
    
    emit_check_acc_null(this->get_line_number(), s);
    
    stringstream ss;
    ss << type_name << "_dispTab";

    emit_load_address(T1, ss.str().c_str(), s);
    
    PRINT("STATIC");

    PRINT(type_name);
    int offset = lookupMethod(type_name, name);

    emit_load(T1, offset, T1, s);
    emit_jalr(T1, s);
}

void dispatch_class::code(ostream &s) {
    
    //TODO Aiken said that this was tricky because I think you evaluate the object last.
    //Ensure that we do this.

    // Loop through the arguments and evaluate them
    // We need to push the value returned by these arguments onto the stack

    for(int i=0; i<actual->len(); i++){ //FIXED
       actual->nth(i)->code(s);
       emit_push(ACC, s);
    }
    
    bool self = expr->type == SELF_TYPE;

    expr->code(s);
    
    emit_check_acc_null(this->get_line_number(), s);

    emit_load(T1, 2, ACC, s);
    int offset;
    if(self){
        offset = lookupMethod(curClass, name);
    }else{
        offset = lookupMethod(expr->type, name);
    }

    emit_load(T1, offset, T1, s);
    emit_jalr(T1, s);
}

void emit_bool_get_val(ostream &s){ //Turns ACC into bool val of ACC.
    emit_load(ACC, BOOLVAL_OFFSET, ACC, s);
}

void emit_int_get_val(ostream &s){ //Ironically this is exactly the same as the above fn but whatever.
    emit_load(ACC, INTVAL_OFFSET, ACC, s);
}

void cond_class::code(ostream &s) {
    pred->code(s); //result of pred into acc
    
    //load 1/0 into acc
    emit_bool_get_val(s); 

    int firstlabel = label_count++;
    int secondlabel = label_count++;

    emit_beqz(ACC, firstlabel, s); //branch if false
    then_exp->code(s);
    emit_branch(secondlabel, s); //go to end now.
    emit_label_def(firstlabel, s);
    else_exp->code(s);
    emit_label_def(secondlabel, s);
}

void loop_class::code(ostream &s) {
    int top_loop = label_count++;
    int end_loop = label_count++;

    emit_label_def(top_loop, s);
    pred->code(s); //evalulate predicate and jump out if 
    emit_bool_get_val(s);
    emit_beqz(ACC, end_loop, s); //it is false
    body->code(s); //otherwise do the loop
    emit_branch(top_loop, s);
    emit_label_def(end_loop, s);

}

bool compRange(int i, int j){
    int diff1 = i - subClassRange[i];
    int diff2 = j - subClassRange[j];
    return diff1 < diff2;
}


void new_stack_variable(char *identifier, Symbol *type_decl, ostream &s){ //Adds accumulator with id identifier and type type_decl as a new stack var
    int offset = --cur_offset;

    variableOffsets.enterscope();
    variableTypes.enterscope();

    pair<bool, int>* p = new pair<bool, int>(false, cur_offset);
    variableOffsets.addid(identifier, p);
    variableTypes.addid(identifier, type_decl);

    emit_addiu(SP, SP, -4, s); //make room on the stack for the next variable 

    //Initialize a default value

    if (isNoExpr) { 
        //Init default value
        initialize_default_value(*type_decl, s);
    } 
    emit_store(ACC, offset, FP, s); // store the local variable on the stack
    
    isNoExpr = false;
}


void remove_top_stack_variable(ostream &s){
    emit_addiu(SP, SP, 4, s); //stack goes back up

    variableOffsets.exitscope();
    variableTypes.exitscope();

    ++cur_offset;
}

void typcase_class::code(ostream &s) {
    
    vector<int> order; 
    //This vector contain the order of branches for the case to eval

    for(int i=0; i< cases->len(); i++){
        branch_class *b = (branch_class*) cases->nth(i);
        order.push_back(classTagLookup[b->type_decl]);
    }
    
    //Sort the cases based on subClassRange 
    sort(order.begin(), order.end(), compRange);

    //DEBUG

    for(vector<int>::iterator it = order.begin(); it != order.end(); ++it)
        PRINT(*it << endl);
   
    //Now, we generate the code

    int end_case = label_count++; //This is for when we return to outside case
    int this_case = label_count++;
    bool first = true;
    
    expr->code(s); //Now, the result of the expr is ACC

    //Case on void run-time error
    emit_bne(ACC, ZERO, this_case, s);
    emit_load_address(ACC, "str_const0", s); //TODO: Don't hardcode this
    emit_load_imm(T1, this->get_line_number() , s); //TODO: Get the real line number
    emit_jal("_case_abort2", s);
    
    for(int i=0; i < order.size(); i++){
        
        int classTag = order[i];
        branch_class *b;

        //Lame way to find the right branch
        for(int j=0; j<cases->len(); j++){
            PRINT(((branch_class*)(cases->nth(j)))->type_decl);
            if(classTagLookup[((branch_class*)(cases->nth(j)))->type_decl] == classTag){
                b = (branch_class*) cases->nth(j);
            }
        }
        
        emit_label_def(this_case, s);
        
        //In the first case branch, we read the class tag of the object in the ACC 
        if(first){
            emit_load(T2, 0 , ACC, s); //load classTag value from result
            first = false;
        }

        this_case = label_count++; //incrementing to find the next case
        
        string temp = "";

        //Check if it's within the class tag range

        stringstream ss;
        ss << subClassRange[classTag];
        temp = ss.str();
        emit_blt(T2, temp.c_str(), this_case, s);
        
        stringstream si;
        si << classTag;
        temp = si.str();
        emit_bgt(T2, temp.c_str(), this_case, s);

        new_stack_variable(b->name->get_string(), &b->type_decl, s); 
        //Adds accumulator with id identifier and type type_decl as a new stack var
        
        //Run the expr
        b->expr->code(s);
        remove_top_stack_variable(s);
        emit_branch(end_case, s);
    }
    
    //call _case_abort here, this branch is to throw a run-time error if nothing matches
    //TODO: We are getting the wrong class name here.
    emit_label_def(this_case, s);
    emit_jal("_case_abort", s);
    
    //Up it for the next label wherever
    label_count++;
    emit_label_def(end_case, s);
}

void block_class::code(ostream &s) {
    for (int i=0;i<body->len();i++){
        body->nth(i)->code(s);
    }
}

void let_class::code(ostream &s) {
    isNoExpr = false;
    init->code(s);
    new_stack_variable(identifier->get_string(), &type_decl, s);
    body->code(s);
    remove_top_stack_variable(s);
}

#define EMIT_PLUS 0
#define EMIT_SUB 1
#define EMIT_MUL 2
#define EMIT_DIVIDE 3

void emit_arith(ostream &s, int type, Expression e1, Expression e2){
    //Must return an int object.
    e1->code(s);
    //emit_move(T1, ACC, s);
    emit_load(S1, INTVAL_OFFSET, ACC, s); //load int value from result
    emit_push(S1, s); //must push result to stack (no way to guarantee that we wont overwrite values)

    e2->code(s); //result obj in ACC

    emit_jal("Object.copy", s); //copy 2nd object to return
    emit_load(T2, INTVAL_OFFSET, ACC, s); //load int value from 2nd object

    emit_pop(S1, s); //now pop result from stack

    if (type == EMIT_PLUS)
        emit_add(S1, S1, T2, s); //add int values and put them into the copied object
    if (type == EMIT_SUB)
        emit_sub(S1, S1, T2, s);
    if (type == EMIT_MUL)
        emit_mul(S1, S1, T2, s);
    if (type == EMIT_DIVIDE)
        emit_div(S1, S1, T2, s);
        
    emit_store(S1, INTVAL_OFFSET, ACC, s);
}
void plus_class::code(ostream &s) {
    emit_arith(s, EMIT_PLUS, e1, e2);
}

void sub_class::code(ostream &s) {
    emit_arith(s, EMIT_SUB, e1, e2);
}

void mul_class::code(ostream &s) {
    emit_arith(s, EMIT_MUL, e1, e2);
 }

void divide_class::code(ostream &s) {
    emit_arith(s, EMIT_DIVIDE, e1, e2);
}

void neg_class::code(ostream &s) {
    e1->code(s);
    emit_load(S1, INTVAL_OFFSET, ACC, s);
    emit_jal("Object.copy", s); //copy 2nd object to return
    emit_sub(S1, ZERO, S1, s);
    emit_store(S1, INTVAL_OFFSET, ACC, s);
}

//TODO: I don't think these should be hardcoded in. 
void emit_load_false(ostream &s){
    emit_load_address(ACC, "bool_const0", s); 
}

void emit_load_true(ostream &s){
    emit_load_address(ACC, "bool_const1", s); 
}

void comparison_general(Expression e1, Expression e2, ostream &s, bool add1){
    e1->code(s);
    emit_int_get_val(s);
    emit_push(ACC, s);
    e2->code(s);
    emit_int_get_val(s);
    if (add1) emit_addiu(ACC, ACC, 1, s);
    emit_pop(S1, s);

    int label_true = label_count++;
    int label_exit = label_count++;

    emit_blt(S1, ACC, label_true, s); //branch if e1 < e2
    //load false into the accumulator
    emit_load_false(s);
    emit_branch(label_exit, s);
    emit_label_def(label_true, s);
    emit_load_true(s);
    //load true into the acc
    emit_label_def(label_exit, s);
}

//Guaranteed to have int arguments, but it returns a bool.
void lt_class::code(ostream &s) {
    comparison_general(e1, e2, s, false);
}

void leq_class::code(ostream &s) {
    comparison_general(e1, e2, s, true); //add 1 and then eval lt
}

//TODO: we should compare pointers here too (it says so in operational semantics)
void eq_class::code(ostream &s) {
    int label_eq = label_count++;

    e1->code(s);
    emit_push(ACC, s);

    e2->code(s);
    emit_move(T1, ACC, s);
    emit_pop(ACC, s);
    emit_move(T2, ACC, s);

    emit_load_false(s);
    emit_move(A1, ACC, s);
    emit_load_true(s); //Load t/f in a semi stupid way to keep abstraction (yay)

    emit_beq(T1, T2, label_eq, s);

    emit_jal("equality_test", s);

    emit_label_def(label_eq, s);
}

void comp_class::code(ostream &s) {
    int label_false = label_count++;
    int label_done = label_count++;

    e1->code(s);
    emit_bool_get_val(s);
    emit_beqz(ACC, label_false, s);
    emit_load_false(s);
    emit_branch(label_done, s);
    emit_label_def(label_false, s);
    emit_load_true(s);
    emit_label_def(label_done, s);

}

void int_const_class::code(ostream& s)  
{
  //
  // Need to be sure we have an IntEntry *, not an arbitrary Symbol
  //
  emit_load_int(ACC,inttable.lookup_string(token->get_string()),s);
    emit_jal("Object.copy", s);
}

void string_const_class::code(ostream& s)
{
  emit_load_string(ACC,stringtable.lookup_string(token->get_string()),s);
}

void bool_const_class::code(ostream& s)
{
  emit_load_bool(ACC, BoolConst(val), s);
    emit_jal("Object.copy", s);
}

void new__class::code(ostream &s) {
    string prototyp; //I see your protObj "cleverness" and raise you a pun
    string classtyp; 

    if (type_name == SELF_TYPE) { 
        prototyp = curClass->get_string();
        prototyp += PROTOBJ_SUFFIX;


        classtyp = curClass->get_string();
        classtyp += CLASSINIT_SUFFIX;
    } else { 
        prototyp = type_name->get_string(); 
        

        prototyp += PROTOBJ_SUFFIX;

        classtyp = type_name->get_string();
        classtyp += CLASSINIT_SUFFIX;
    }

    emit_load_address(ACC, prototyp.c_str(), s);
    emit_jal("Object.copy", s);
    emit_jal(classtyp.c_str(), s);
}

void isvoid_class::code(ostream &s) {
    int truelabel = label_count++;
    int donelabel = label_count++;

    e1->code(s);


    emit_beqz(ACC, truelabel, s); //branch if false
    //load false into acc
    emit_load_false(s);
    emit_branch(donelabel, s); //go to end now.
    emit_label_def(truelabel, s);
    //load true into acc
    emit_load_true(s);
    emit_label_def(donelabel, s);

}

void no_expr_class::code(ostream &s) {
    isNoExpr = true;
    PRINT("IS NO EXPR");
    //Should return an empty object with 0's everywhere that we care about
}

void object_class::code(ostream &s) {
    if (name == self){
        emit_move(ACC, SELF, s);
    } else { 
        emit_load_variable(name->get_string(), s); 
    } 
}







