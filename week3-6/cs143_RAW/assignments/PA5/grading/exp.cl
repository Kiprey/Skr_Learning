class Main inherits IO {
  exp(b : Int, x : Int) : Int {
    if (x = 0)
    then
      1
    else
      if (x = (2 * (x / 2)))
      then
        let y : Int <- exp(b, (x / 2)) in
          y * y
      else
        b * exp(b, x - 1)
      fi
    fi
  };
  main() : Object { {
    out_int(exp(2, 7));
    out_string("\n");
    out_int(exp(3, 6));
    out_string("\n");
    out_int(exp(8, 3));
    out_string("\n");
  } };
};
