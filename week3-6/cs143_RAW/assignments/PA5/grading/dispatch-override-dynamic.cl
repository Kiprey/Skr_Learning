-- Dynamic dispatch chooses a method based on the dynamic type of the
-- dispatching object.


class Base inherits IO
{
  identify() : Object
  {
    out_string( "base\n" )
  };
};


class Derived inherits Base
{
  identify() : Object
  {
    out_string( "derived\n" )
  };
};


class Main
{
  main() : Object
  {
    {
      let me : Base <- new Base in
	me.identify();

      let me : Base <- new Derived in
	me.identify();

      let me : Derived <- new Derived in
	me.identify();
    }
  };
};
