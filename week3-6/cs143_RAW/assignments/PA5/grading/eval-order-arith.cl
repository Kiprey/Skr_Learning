-- Arithmetic operators evaluate their arguments left to right.


class Main inherits IO
{
  recite( value : Int ) : Int
  {
    {
      out_int( value );
      out_string( "\n" );
      value;
    }
  };

  main() : Object
  {
    {
      recite( 1 ) + recite( 2 );
      recite( 3 ) - recite( 4 );
      recite( 5 ) * recite( 6 );
      recite( 7 ) / recite( 8 );
      recite( 9 ) < recite( 10 );
      recite( 11 ) = recite( 12 );
      recite( 13 ) <= recite( 14 );
    }
  };
};
