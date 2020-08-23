-- recclass.cl
-- try to break receiver class opt

class P {
  x:P;
  foo():Object {{
    x <- new P;      -- might think we have a P
    abort();
  }};
  badGuy():Object {
    x.foo()          -- and this would become a static call
  };
};

class C inherits P {
  foo():Object {{
    (new IO).out_string("ok\n");
    x <- new C;      -- but we really have a C
  }};
};

class Main {
  f1(p:P):Object {{
    if false then
      p <- new P     -- might think we have a P statically
    else
      0
    fi;
    p.foo();         -- then call it
  }};

  main():Object {
    let
      c:C <- new C
    in {
      f1(c);        -- when it's really a C
      c.badGuy();
    }
  };
};
