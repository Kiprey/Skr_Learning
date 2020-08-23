-- A let-bound variable can shadow a same-named formal parameter.


class Main inherits IO
{
  method( var : String ) : Object
  {
    let var : String <- "good" in
      out_string( var )
  };

  main() : Object
  {
    {
      method( "bad" );
      out_string( "\n" );
    }
  };
};
