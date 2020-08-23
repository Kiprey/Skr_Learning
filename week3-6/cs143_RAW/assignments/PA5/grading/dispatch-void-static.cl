-- Statically dispatching on a void object is a runtime error.


class Main inherits IO
{
  complain() : Object
  {
    out_string( "bad\n" )
  };

  main() : Object
  {
    let nothing : Main in
      nothing@Main.complain()
  };
};
