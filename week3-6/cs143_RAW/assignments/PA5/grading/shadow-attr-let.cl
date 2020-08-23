-- A let-bound variable can shadow a same-named attribute.


class Main inherits IO
{
  var : String <- "bad";

  main() : Object
  {
    {
      let var : String <- "good" in
	out_string( var );

      out_string( "\n" );
    }
  };
};
