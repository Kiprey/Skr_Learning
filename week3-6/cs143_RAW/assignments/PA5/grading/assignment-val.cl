class Main inherits IO {
  f(x : Int, y : Int) : Object { {
    out_string("x: ");
    out_int(x);
    out_string("\ny: ");
    out_int(y);
    out_string("\n");
  } };
  main() : Object {
    let x : Int <- 2 in {
      f(x <- 3, x <- 4);
      out_int(x);
    }
  };
};
