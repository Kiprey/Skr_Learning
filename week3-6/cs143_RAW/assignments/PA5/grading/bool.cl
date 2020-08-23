-- bool.cl: tricky cases for unboxed bools

class Main {
  main():Object {
    let
      t:Bool <- true,
      f:Bool <- false,
      t1:Object <- t,
      t2:Object <- true,
      f1:Object <- f,
      f2:Object <- false,
      b1:Bool,
      b2:Object,
      io:IO <- new IO
    in {
      io.out_string("t: ");
      io.out_string(t.type_name());
      io.out_string("\n");

      b1 <- t;
      io.out_string("b1: ");
      io.out_string(b1.type_name());
      io.out_string("\n");

      b2 <- t1;
      io.out_string("b2: ");
      io.out_string(b2.type_name());
      io.out_string("\n");

      b1 <- f.copy();
      io.out_string("b1: ");
      io.out_string(b1.type_name());
      io.out_string("\n");

      b2 <- f2.copy();
      io.out_string("b2: ");
      io.out_string(b2.type_name());
      io.out_string("\n");
    }
  };
};






