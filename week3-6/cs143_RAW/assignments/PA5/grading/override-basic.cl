-- overriding a built-in method is allowed. 


class Base inherits IO
{
  identify() : Object {{
   out_string(type_name());
   self@Base.out_string("\n");}
  };
};


class Derived inherits Base
{
  out_string(s:String) : SELF_TYPE
  {
    {
      self@Base.out_string("derived ");
      self@Base.out_string(s);
      self@Base.out_string("\n");
    }
  };   
};


class Main
{
  main() : Object
  {
    {
      let me : Base <- new Base in
      me@Base.identify();

      let me : Base <- new Derived in
      me@Base.identify();

      let me : Derived <- new Derived in
      {
	me@Base.identify();
	me@Derived.identify();
      };
    }
  };
};

