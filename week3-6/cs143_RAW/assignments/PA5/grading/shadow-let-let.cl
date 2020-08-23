-- A let-bound variable can shadow a same-named let-bound variable.


class Main inherits IO
{
  main() : Object
  {
    {
      let var : String <- "bad" in
	let var : String <- "good" in
	  out_string( var );

      out_string( "\n" );
    }
  };
};
