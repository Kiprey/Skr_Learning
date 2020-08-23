-- Case branches are tested from most specific to most generic.


class Main inherits IO
{
  main() : Object
  {
    let thing : Object <- self in
      case thing of
	i : Int => out_string( "int\n" );
	b : Bool => out_string( "bool\n" );
      esac
  };
};
