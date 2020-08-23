-- Object.copy creates an object with the same dynamic type as self,
-- which affects subsequent dynamic dispatches on the new object.


class Base inherits IO
{
  identify() : Object
  {
    out_string( "base\n" )
  };

  duplicate() : Base
  {
    copy()
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
    (new Derived).duplicate().identify()
  };
};
