-- Object.type_name returns a string representation of the dynamic
-- type of the dispatching object.


class Base inherits IO
{
  identify( thing : Object ) : Object
  {
    {
      out_string( thing.type_name() );
      out_string( "\n" );
    }
  };

  test() : Object
  {
    {
      identify( new Base );
      identify( new Derived );
      identify( new Main );

      let poly : Base <- new Derived in
	identify( poly );

      identify( self );
    }
  };
};


class Derived inherits Base
{
};


class Main
{
  main() : Object
  {
    (new Derived).test()
  };
};
