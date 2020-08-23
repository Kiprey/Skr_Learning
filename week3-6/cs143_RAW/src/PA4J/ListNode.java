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

import java.io.PrintStream;
import java.util.Vector;
import java.util.Enumeration;

/** Base class for lists of AST elements.

    <p> 

    (See <a href="TreeNode.html">TreeNode</a> for a discussion of AST
    nodes in general)

    <p>

    List phyla have a distinct set of operations for constructing and
    accessing lists.  For each phylum named <em>X</em> there is a phylum
    called <em>X</em>s (except for <code>Classes</code>, which is a list
    of <code>Class_</code> nodes) of type <code>List[X]</code>.

    <p> 

    An empty list is created with <code>new Xs(lineno)</code>.  Elements
    may be appended to the list using either <code>addElement()</code>
    or <code>appendElement()</code>.  <code>appendElement</code> returns
    the list itself, so calls to it may be chained, as in <code>
    list.appendElement(Foo).appendElement(Bar).appendElement(Baz)</code>.

    <p>
    
    You can use <code>java.util.Enumeration</code> to iterate through
    lists.  If you are not familiar with that interface, look it up in
    the Java API documentation.  Here's an example of iterating through
    a list:

<pre>
  for (Enumeration e = list.getElements(); e.hasMoreElements(); ) {
    Object n = e.nextElement();
    ... do something with n ...
  }
</pre>

    Alternatively, it is possible to iterate using an integer index:

<pre>
  for (int i = 0; i < list.getLength(); i++) {
    ... do something with list.getNth(i) ...
  }
</pre>

    Note: <code>nextElement()</code> returns the value of type
    <code>Object</code> and <code>getNth()</code> returns the value of
    type <code>TreeNode</code>.  You will most likely need to cast it to
    the type appropriate for the list element. <em>This is one of the
    very few cases where casting is actually necessary and
    appropriate</em>.

*/
    
abstract class ListNode extends TreeNode {
    private Vector elements;

    protected ListNode(int lineNumber, Vector elements) {
	super(lineNumber);
	this.elements = elements;
    }

    /** Builds a new list node
     *
     * @param lineNumber line in the source file from which this node came. 
     * */
    protected ListNode(int lineNumber) {
	super(lineNumber);
	elements = new Vector();
    }

    /** Creates a deep copy of this list.
     *
     * None of the elements are shared between the lists, e.g. all
     * elements are duplicated (which is what "deep copy" means).
     *
     * @return a copy of this elements vector
     * */
    protected Vector copyElements() {
	Vector cp = new Vector();
	for (int i = 0; i < elements.size(); i++) {
	    cp.addElement(((TreeNode)elements.elementAt(i)).copy());
	}
	return cp;
    }

    /** Returns the class of list elements.
     *
     * @return the element class
     * */
    public abstract Class getElementClass();

    /** Retreives nth element of the list.
     *
     * @param n the index of the element
     * @return the element
     * */
    public TreeNode getNth(int n) {
	return (TreeNode)elements.elementAt(n);
    }

    /** Retreives the length of the list.
     *
     * @return the length of the list
     * */
    public int getLength() {
	return elements.size();
    }

    /** Retreives the elements of the list as Enumeration.
     *
     * @return the elements
     * */
    public Enumeration getElements() {
	return elements.elements();
    }

    /** Appends an element to the list.
     * 
     * <p>Note: each generated subclass of ListNode also has an
     * appendElement() method, which calls addElement() and returns the
     * list of the appropriate type, so that it can be used like this:
     * <code>l.appendElement(i).appendElement(j).appendElement(k);</code>
     *
     * @param node a node to append
     * */
    public void addElement(TreeNode node) {
	elements.addElement(node);
    }

    /** Pretty-prints this list to this output stream.
     *
     * @param out the output stream
     * @param n the number of spaces to indent the output
     * */
    public void dump(PrintStream out, int n) {
	out.print(Utilities.pad(n));
	out.print("list\n");
	for (int i = 0; i < getLength(); i++) {
	    getNth(i).dump(out, n + 2);
	}
	out.print(Utilities.pad(n));
	out.print("(end_of_list)\n");
    }

    /** Returns a string representation of this list.
     *
     * @return a string representation
     * */
    public String toString() {
	return elements.toString();
    }
}
	


    
    

    

    
    
