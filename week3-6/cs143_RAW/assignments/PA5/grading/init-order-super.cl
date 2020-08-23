-- Superclass attribute initializers must be evaluated before subclass
-- attribute initializers.


class Base inherits IO
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
  b : Object <- recite( 2 );
};


class Main inherits Base
{
  y : Object <- recite( 3 );
  z : Object <- recite( 4 );
  c : Object <- recite( 5 );

  main() : Object
  {
    self
  };
};
