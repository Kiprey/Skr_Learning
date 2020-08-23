-- A case-bound variable can shadow a same-named formal parameter.


class Main inherits IO
{
  method( var : String ) : Object
  {
    case "good" of
      var : String =>
      out_string( var );
    esac
  };

  main() : Object
  {
    {
      method( "bad" );
      out_string( "\n" );
    }
  };
};
