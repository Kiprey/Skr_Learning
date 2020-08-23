-- Attribute initializers must be evaluated in the order in which they
-- appeared in the class declaration.


class Main inherits IO
{
  recite( value : Int ) : Object
  {
    {
      out_int( value );
      out_string( "\n" );
      self;
    }
  };

  x : Object <- recite( 1 );
  y : Object <- recite( 2 );
  z : Object <- recite( 3 );
  c : Object <- recite( 4 );
  b : Object <- recite( 5 );
  a : Object <- recite( 6 );

  main() : Object
  {
    self
  };
};
