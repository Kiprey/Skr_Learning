-- newbasic.cl
-- create basic classes with 'new', which may
-- mess up assumptions, particularly w/bool

class Main {
  io:IO <- new IO;

  main():Object {{
    io.out_string(  (not (new Bool)).type_name()  );
    io.out_string("\n");

    io.out_int(  (new Int) + 1  );
    io.out_string("\n");

    io.out_string(  (new String).substr(0,0).type_name()  );
    io.out_string("\n");
  }};
};

