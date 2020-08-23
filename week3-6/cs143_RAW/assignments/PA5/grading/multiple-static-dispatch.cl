class A inherits IO {
  x : Int <- 1;
  f(y : Int) : SELF_TYPE { {
    x <- x + y;
    out_int(x);
    out_string("\n");
    self;
  } };
  g(y : Int) : SELF_TYPE { {
    x <- x - y;
    out_int(x);
    out_string("\n");
    self;
  } };
};

class B inherits A {
  f(y : Int) : SELF_TYPE { {
    x <- x * y;
    out_int(x);
    out_string("\n");
    self;
  } };
};

class Main inherits IO {
  b : B <- new B;
  main() : Object {
    b.g(~4)@A.f(8).f(5)@A.g(3)
  };
};
