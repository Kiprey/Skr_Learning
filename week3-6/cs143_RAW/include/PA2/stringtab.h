// -*-Mode: C++;-*-
//
// See copyright.h for copyright notice and limitation of liability
// and disclaimer of warranty provisions.
//
#include "copyright.h"


#ifndef _STRINGTAB_H_
#define _STRINGTAB_H_

#include <assert.h>
#include <string.h>
#include "list.h"    // list template
#include "cool-io.h"

class Entry;
typedef Entry* Symbol;

extern ostream& operator<<(ostream& s, const Entry& sym);
extern ostream& operator<<(ostream& s, Symbol sym);

/////////////////////////////////////////////////////////////////////////
//
//  String Table Entries
//
/////////////////////////////////////////////////////////////////////////

class Entry {
protected:
  char *str;     // the string
  int  len;      // the length of the string (without trailing \0)
  int index;     // a unique index for each string
public:
  Entry(char *s, int l, int i);

  // is string argument equal to the str of this Entry?
  int equal_string(char *s, int len) const;  
                         
  // is the integer argument equal to the index of this Entry?
  bool equal_index(int ind) const           { return ind == index; }

  ostream& print(ostream& s) const;

  // Return the str and len components of the Entry.
  char *get_string() const;
  int get_len() const;
};

//
// There are three kinds of string table entries:
//   a true string, an string representation of an identifier, and 
//   a string representation of an integer.
//
// Having separate tables is convenient for code generation.  Different
// data definitions are generated for string constants (StringEntry) and 
// integer  constants (IntEntry).  Identifiers (IdEntry) don't produce
// static data definitions.
//
// code_def and code_ref are used by the code to produce definitions and
// references (respectively) to constants.  
//
class StringEntry : public Entry {
public:
  void code_def(ostream& str, int stringclasstag);
  void code_ref(ostream& str);
  StringEntry(char *s, int l, int i);
};

class IdEntry : public Entry {
public:
  IdEntry(char *s, int l, int i);
};

class IntEntry: public Entry {
public:
  void code_def(ostream& str, int intclasstag);
  void code_ref(ostream& str);
  IntEntry(char *s, int l, int i);
};

typedef StringEntry *StringEntryP;
typedef IdEntry *IdEntryP;
typedef IntEntry *IntEntryP;

//////////////////////////////////////////////////////////////////////////
//
//  String Tables
//
//////////////////////////////////////////////////////////////////////////

template <class Elem> 
class StringTable
{
protected:
   List<Elem> *tbl;   // a string table is a list
   int index;         // the current index
public:
   StringTable(): tbl((List<Elem> *) NULL), index(0) { }   // an empty table
   // The following methods each add a string to the string table.  
   // Only one copy of each string is maintained.  
   // Returns a pointer to the string table entry with the string.

   // add the prefix of s of length maxchars
   Elem *add_string(char *s, int maxchars);

   // add the (null terminated) string s
   Elem *add_string(char *s);

   // add the string representation of an integer
   Elem *add_int(int i);


   // An iterator.
   int first();       // first index
   int more(int i);   // are there more indices?
   int next(int i);   // next index

   Elem *lookup(int index);      // lookup an element using its index
   Elem *lookup_string(char *s); // lookup an element using its string

   void print();  // print the entire table; for debugging

};

class IdTable : public StringTable<IdEntry> { };

class StrTable : public StringTable<StringEntry>
{
public: 
   void code_string_table(ostream&, int classtag);
};

class IntTable : public StringTable<IntEntry>
{
public:
   void code_string_table(ostream&, int classtag);
};

extern IdTable idtable;
extern IntTable inttable;
extern StrTable stringtable;
#endif
