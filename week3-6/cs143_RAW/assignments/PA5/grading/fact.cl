class Main inherits IO {
  fact(n : Int) : Int {
    if (n = 0)
    then
      1
    else
      n * fact(n - 1)
    fi
  };
  main() : Object { {
    out_int(fact(3));
    out_string("\n");
    out_int(fact(7));
    out_string("\n");
    out_int(fact(10));
    out_string("\n");
  } };
};
