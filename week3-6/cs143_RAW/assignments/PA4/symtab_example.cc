#include <stdlib.h>
#include <stdio.h>
#include <symtab.h>

int main(int argc, char *argv[]) {
  // 
  // Create a mapping from strings to ints
  //

  SymbolTable<char *,int> *map = new SymbolTable<char *, int>();
  char *Fred = "Fred";
  char *Mary = "Mary";
  char *Miguel = "Miguel";

  // enter a scope; required before any symbols can be added
  map->enterscope();
  
  // add a couple of entries mapping name to age.
  // note the second argument must be a pointer to an integer
  map->addid(Fred, new int(22));
  map->addid(Mary, new int(25));

  // add a scope, add more names:
  map->enterscope();
  map->addid(Miguel, new int(35));
  map->addid(Mary, new int(23));

  // check whether Fred is in the current scope; predicate is false
  cout << ((map->probe(Fred) != NULL) ? "Yes\n" : "No\n");

  // check whether Mary is in any scope; predicate is true
  cout << ((map->lookup(Mary) != NULL) ? "Yes\n" : "No\n");

  // print age of most-closely-nested Mary; note the
  // lookup returns a pointer to an integer.
  cout << *(map->lookup(Mary)) << "\n";

  // check whether Miguel is in the current scope; predicate is true
  cout << ((map->probe(Miguel) != NULL) ? "Yes\n" : "No\n");

  // leave a scope
  map->exitscope();

  // print age of most-closely-nested Mary
  cout << *(map->lookup(Mary)) << "\n";

  // check whether Fred is in the current scope; predicate is now true
  cout << ((map->probe(Fred) != NULL) ? "Yes\n" : "No\n");

  // check whether Miguel is in any scope; predicate is now false
  cout << ((map->lookup(Miguel) != NULL) ? "Yes\n" : "No\n");

  return 0;
 
}
