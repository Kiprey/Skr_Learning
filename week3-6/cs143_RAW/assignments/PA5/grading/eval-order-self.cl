-- The dispatched-upon object is evaluated after all actual arguments.


class Main inherits IO
{
  recite( value : Int ) : Main
  {
    {
      out_int( value );
      out_string( "\n" );
      self;
    }
  };

  disregard( a : Object ) : Object
  {
    self
  };

  main() : Object
  {
    recite( 2 ).disregard( recite( 1 ) )
  };
};
