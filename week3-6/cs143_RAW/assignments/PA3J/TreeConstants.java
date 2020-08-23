/*
Copyright (c) 2000 The Regents of the University of California.
All rights reserved.

Permission to use, copy, modify, and distribute this software for any
purpose, without fee, and without written agreement is hereby granted,
provided that the above copyright notice and the following two
paragraphs appear in all copies of this software.

IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY PARTY FOR
DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT
OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE UNIVERSITY OF
CALIFORNIA HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

THE UNIVERSITY OF CALIFORNIA SPECIFICALLY DISCLAIMS ANY WARRANTIES,
INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
ON AN "AS IS" BASIS, AND THE UNIVERSITY OF CALIFORNIA HAS NO OBLIGATION TO
PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
*/

// This is a project skeleton file

/** For convenience, this clas defines a large number of symbols.  These
 * symbols include the primitive type and method names, as well as fixed
 * names used by the runtime system.  */
class TreeConstants {
    public static final AbstractSymbol arg 
	= AbstractTable.idtable.addString("arg");
    
    public static final AbstractSymbol arg2 
	= AbstractTable.idtable.addString("arg2");
    
    public static final AbstractSymbol Bool 
	= AbstractTable.idtable.addString("Bool");
    
    public static final AbstractSymbol concat 
	= AbstractTable.idtable.addString("concat");

    public static final AbstractSymbol cool_abort 
	= AbstractTable.idtable.addString("abort");

    public static final AbstractSymbol copy 
	= AbstractTable.idtable.addString("copy");

    public static final AbstractSymbol Int 
	= AbstractTable.idtable.addString("Int");

    public static final AbstractSymbol in_int 
	= AbstractTable.idtable.addString("in_int");
    
    public static final AbstractSymbol in_string 
	= AbstractTable.idtable.addString("in_string");

    public static final AbstractSymbol IO 
	= AbstractTable.idtable.addString("IO");

    public static final AbstractSymbol length 
	= AbstractTable.idtable.addString("length");

    public static final AbstractSymbol Main 
	= AbstractTable.idtable.addString("Main");

    public static final AbstractSymbol main_meth 
	= AbstractTable.idtable.addString("main");
    
    public static final AbstractSymbol No_class 
	= AbstractTable.idtable.addString("_no_class");

    public static final AbstractSymbol No_type 
	= AbstractTable.idtable.addString("_no_type");

    public static final AbstractSymbol Object_ = 
	AbstractTable.idtable.addString("Object");
    
    public static final AbstractSymbol out_int 
	= AbstractTable.idtable.addString("out_int");

    public static final AbstractSymbol out_string 
	= AbstractTable.idtable.addString("out_string");

    public static final AbstractSymbol prim_slot 
	= AbstractTable.idtable.addString("_prim_slot");

    public static final AbstractSymbol self 
	= AbstractTable.idtable.addString("self");

    public static final AbstractSymbol SELF_TYPE 
	= AbstractTable.idtable.addString("SELF_TYPE");

    public static final AbstractSymbol Str 
	= AbstractTable.idtable.addString("String");

    public static final AbstractSymbol str_field 
	= AbstractTable.idtable.addString("_str_field");

    public static final AbstractSymbol substr = 
	AbstractTable.idtable.addString("substr");

    public static final AbstractSymbol type_name = 
	AbstractTable.idtable.addString("type_name");

    public static final AbstractSymbol val = 
	AbstractTable.idtable.addString("_val");
}
