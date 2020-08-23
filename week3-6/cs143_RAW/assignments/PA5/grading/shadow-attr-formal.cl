-- A formal parameter can shadow a same-named attribute.


class Main inherits IO
{
  var : String <- "bad";

  method( var : String ) : Object
  {
    out_string( var )
  };

  main() : Object
  {
    {
      method( "good" );
      out_string( "\n" );
    }
  };
};
