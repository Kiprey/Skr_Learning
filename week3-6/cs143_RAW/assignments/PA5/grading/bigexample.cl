
(*  Example cool program testing as many aspects of the code generator
    as possible.
 *)

 
class P {
  x:Int <- 3;
};
class C1 inherits P {};
class C2 inherits P {
  getX():Int { x };
  setX(i:Int):Int { x <- i };
};

class Blah {
  i:Int;
  b:Bool;
  s:String;

  j:Int <- tiny();
  c:Bool <- true;
  t:String <- { self@Blah.add(); "whassup?!"; };
  
  io : IO <- new IO;
  void : Object;

  tiny():Int {{ "Hi"; false; 0; }};

  add():Int { 1+2 };

  compare():Bool { 1 < 2 };
  compareEq():Bool { 1 = 2 };

  isv():Bool { isvoid 1 };

  divByZero():Int { 1 / 0 };

  cmp():Bool { not true };

  ng():Int { ~5 };

  caller():Int { self@Blah.callee(6,13) + self@Blah.callee(9,2) };
  callee(x:Int, y:Int):Int { x+y };

  get_t():String { t };

  useLet():Int {
    let
      q:Int,
      r:Int <- 99,
      s:Int <- j
    in
      q+r+s
  };

  useIf():Int {
    if cmp() then
      ng()
    else
      tiny()
    fi
  };

  useWhile():Object {
    while cmp() loop
      get_t()
    pool
  };

  useCase():Int {
    case self of
      p:P => 3;
      c:C => 4;
      b:Blah => 5;
    esac
  };

  useNew():Object {{
    new Int;
    new Bool;
    new C;
    new Main;
    new SELF_TYPE;
  }};

  doThemAll():Int {{
    io@IO.out_string("basics:\n");
    tiny();
    add();
    compare();
    compareEq();
    isv();
    -- divByZero();
    cmp();
    ng();
    caller();
    callee(3,4);
    get_t();
    useLet();
    useIf();
    useWhile();
    useCase();
    useNew();

    io@IO.out_string("more advanced:\n");
    bigIf();
    bigMath();
    bigAssign();
    bigWhile();
    bigSelf();
    bigStrCompare();
    bigCase();
    bigAttrAccess();

    0;
  }};

  bigIf():Int {{
    io.out_string("bigIf\n");

    if true then
      if false then
        self@Object.abort()
      else
        0
      fi
    else
      self@Object.abort()
    fi;

    if false then
      self@Object.abort()
    else
      if false then
        self@Object.abort()
      else
        0
      fi
    fi;

    if true then
      if true then
        0
      else
        self@Object.abort()
      fi
    else
      self@Object.abort()
    fi;

    if false then
      self@Object.abort()
    else
      if true then
        0
      else
        self@Object.abort()
      fi
    fi;

    0;
  }};

  bigWhile():Object {{
    io.out_string("bigWhile\n");

    -- simple linear loop
    let
      x : Int <- 34,
      ct : Int <- 0
    in {
      while not x <= 10 loop {
        x <- x - 1;
        ct <- ct + 1;
      } pool;

      if not x = 10 then abort() else
      if not ct = 24 then abort() else
      self
      fi fi;
    };

    -- triply-nested loop
    let
      i : Int,
      j : Int,
      k : Int,
      ctI : Int <- 0,
      ctJ : Int <- 0,
      ctK : Int <- 0
    in {
      i <- 0;
      while i < 10 loop {
        j <- 0;
        while j < 10 loop {
          k <- 0;
          while k < 10 loop {
            k <- k + 1;
            ctK <- ctK + 1;
          } pool;
          j <- j + 1;
          ctJ <- ctJ + 1;
        } pool;
        i <- i + 1;
        ctI <- ctI + 1;
      } pool;

      assert(ctI = 10, 12);
      assert(ctJ = 100, 123);
      assert(ctK = 1000, 1234);
    };

  }};  


  bail(i : Int) : Object {{
    io.out_string("failed: ");
    io.out_int(i);
    io.out_string("\n");
    abort();
  }};

  bigMath():Object {{
    if 1 + 2 = 3 then
      0
    else
      bail(55)
    fi;

    io.out_string("bigMath\n");

    if 1 = 1 then
    if 1 + 2 = 3 then
    if 5 - 3 = 2 then
    if 11 * 2 = 22 then
    if 99 / 11 = 9 then
    if 45 + 14 - 75 * 2 + 100 = 9 then
    if 100 / 2 / 2 / 2 / 2 / 2 = 3 then
    if 1 + (2 + 3) = 6 then
    if 1 + (2 + (3 + (4 + 5))) = 15 then
    if 1 + (2 + (3 + (4 + (5 + (6 + (7 + (8 + (9 + 10)))))))) = 55 then
      true
    else bail(1) fi
    else bail(2) fi
    else bail(3) fi
    else bail(4) fi
    else bail(5) fi
    else bail(6) fi
    else bail(7) fi
    else bail(8) fi
    else bail(9) fi
    else bail(10) fi
    ;
    
    assert(not false, 250);
    assert(not not true, 251);

    assert(3 < 4, 253);
    assert(not 3 < 3, 254);
    assert(65 <= 66, 255);
    assert(65 <= 65, 255);
    
    assert(~2 = (0 - 2), 257);
    assert(~2 < 2, 258);
    assert(~0 = 0, 259);
  }};

  assert(b:Bool, line:Int):Object {
    if b then true else {
      io.out_string("assertion failed: ");
      io.out_int(line);  
      io.out_string("\n");
      abort();
    } fi
  };

  bigAssign():Object {
    let
      x : Int,
      y : Int,
      z : Int
    in {
      io.out_string("bigAssign\n");

      assert(x=0, 1);
      assert(y=0, 2);
      assert(z=0, 3);

      x <- 1;
      y <- 2;
      z <- 3;
      assert(x=1, 4);
      assert(y=2, 5);
      assert(z=3, 6);

      z <- 1;
      y <- 2;
      x <- 3;
      assert(z=1, 14);
      assert(y=2, 15);
      assert(x=3, 16);

      x <- y <- 6;
      assert(x=6, 114);
      assert(y=6, 115);
      assert(z=1, 116);

      x <- y + 4;
      y <- x - 3;
      z <- x * y + 14;
      y <- y + 1;
      assert(x=10, 2114);
      assert(y=8,  3114);
      assert(z=84, 2116);

      x <- y;
      y <- z;
      z <- x;
      assert(x=8,  101);
      assert(y=84, 102);
      assert(z=8,  103);

      assert(x+y = 92, 104);
      assert(x=8,  105);
      assert(y=84, 106);
    }
  };
  
  setI(newI : Int):Int { i <- newI };
  getI():Int { i };

  bigSelf():Object {
    let
      v : Blah,             -- void
      s : Blah <- self,     -- self
      t : Blah <- new Blah  -- another Blah
    in {
      io.out_string("bigSelf\n");

      assert(isvoid v, 310);
      assert(not v = s, 311);
      assert(not s = t, 312);
      assert(not v = t, 313);

      s.setI(3);
      t.setI(4);
      assert(not s.getI() = t.getI(), 320);

      t <- self;
      assert(s = t, 314);

      s.setI(5);
      t.setI(6);
      assert(s.getI() = t.getI(), 327);    -- both are 6
    }
  };
  
  bigStrCompare():Object {
    let
      s : String <- "foo",
      t : String <- "bar",
      u : String <- "foobar",
      v : String,
      w : String
    in {
      io.out_string("bigStrCompare\n");

      assert(not s = t, 339);
      assert(s = "foo", 340);
      assert(t = "bar", 341);
      assert(v = w, 342);
      assert(not v = u, 346);

      v <- u.substr(0, 3);
      w <- u.substr(3, 3);
      assert(not v = w, 350);
      assert(v = s, 351);
      assert(w = t, 352);
      assert(not v = t, 353);

      assert(s.substr(1,1) = s.substr(2,1), 355);
    }
  };
  
  bigCase():Object {
    let
      a:A <- new A,
      b:B <- new B,
      c:C <- new C,
      p:P <- new P,
      c1:C1 <- new C1,
      c2:C2 <- new C2,
      i:Int,
      o:Object
    in {
      io.out_string("bigCase\n");

      o <- c1;
      i <- case o of
        x:A => 1;
        y:C1 => 2;
        z:C2 => 3;
        k:Object => 4;
      esac;
      assert(i=2, 381);

      o <- c1;
      i <- case o of
        x:A => 1;
        y:P => 2;
        z:C2 => 3;
        k:Object => 4;
      esac;
      assert(i=2, 390);

      o <- c2;
      i <- case o of
        x:A => 1;
        y:P => 2;
        z:C2 => 3;
        k:Object => 4;
      esac;
      assert(i=3, 399);

      o <- c1;
      i <- case b of
        x:A => 1;     --+
        y:C1 => 2;
        z:C2 => 3;
        k:Object => 4;
      esac;
      assert(i=1, 408);

      o <- b;
      i <- case o of
        x:A => 1;
        x:B => 2;
        x:C => 3;
      esac;
      assert(i=2, 417);

      o <- c;
      i <- case o of
        x:A => 1;
        x:B => 2;
        x:C => 3;
        k:Object => 4;
      esac;
      assert(i=3, 426);

      o <- c1;
      i <- case o of
        x:A => 1;
        x:B => 2;
        x:C => 3;
        k:Object => 4;   --+
      esac;
      assert(i=4, 438);

      o <- b;
      i <- case o of
        k:Object => 4;
      esac;
      assert(i=4, 444);

      o <- true;
      i <- case o of
        x:Bool => 1;
        x:B => 2;
        x:C => 3;
      esac;
      assert(i=1, 453);

      o <- 2;
      i <- case 3 of
        k:Object => 4;
      esac;
      assert(i=4, 459);

      o <- "foobar";
      i <- case "foobar".substr(2,2) of
        x:Bool => 1;
        y:String => 2;
        x:C => 3;
        k:Object => 4;
      esac;
      assert(i=2, 468);
    }
  };
  
  -- not really big as of yet..
  bigAttrAccess():Object {
    let 
      c:C2 <- new C2
    in {
      io.out_string("bigAttrAccess\n");
      assert(c.getX() = 3, 496);
      c.setX(5);
      assert(c.getX() = 5, 498);
    }
  };
};

class A {
  e():Int { 0 };
  f():Int { 0 };
  g():Int { 0 };
  i():Int { 0 };
  j():Int { 0 };
};

class B inherits A {
  e():Int { 0 };
  g():Int { 0 };
  h():Int { 0 };
  k():Int { 0 };
};

class C inherits B {
  e():Int { 0 };
  h():Int { 0 };
  i():Int { 0 };
  ell():Int { 0 };
};


class Base {
  a : Int;
  b : String;
  c : Bool;
};

class Grandparent inherits Base {
  d : Base;
  e : Int;
  f : Grandparent;
};

class Parent inherits Grandparent {
  g : SELF_TYPE;
  h : Int;
  i : Bool;
};

class Child inherits Parent {
  j : Base;
  k : Grandparent;
  l : Main;
  m : String;
};


class Main {
  main():IO {
    let
      io : IO <- new IO,
      b : Blah
    in {
      io@IO.out_string("hello, world\n");

      io.out_string("printed via dynamic dispatch\n");

      io@IO.out_string("tested static dispatch and 'new IO'\n");

      let
        i : Int <- 3
      in {
        io@IO.out_string("an int: ");
        io@IO.out_int(i);
        io@IO.out_string("\n");
      };

      io.out_string("gonna make a Blah\n");
      b <- new Blah;

      io.out_string("gonna call doThemAll\n");
      b.doThemAll();

      io.out_string("looks ok!\n");
    }
  };

};
