-- Actual arguments to a method call must be evaluated left to right.


class Main inherits IO
{
  recite( value : Int ) : Object
  {
    {
      out_int( value );
      out_string( "\n" );
    }
  };

  disregard( a : Object, b : Object, c : Object ) : Object
  {
    self
  };

  main() : Object
  {
    disregard( recite( 1 ), recite( 2 ), recite( 3 ) )
  };
};
