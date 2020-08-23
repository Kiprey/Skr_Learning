-- A let-bound variable can shadow a same-named case-bound variable.


class Main inherits IO
{
  main() : Object
  {
    {
      case "bad" of
	var : String =>
	  let var : String <- "good" in
	    out_string( var );
      esac;

      out_string( "\n" );
    }
  };
};
