-- A case-bound variable can shadow a same-named let-bound variable.


class Main inherits IO
{
  main() : Object
  {
    {
      let var : String <- "bad" in
	case "good" of
	  var : String =>
	    out_string( var );
	esac;

      out_string( "\n" );
    }
  };
};
