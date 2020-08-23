-- not.cl
-- show off quick negation

class Main {
  main():Object {
    let
      x:Int <- 0,
      b:Bool
    in {
      while x < 100 loop {
        b = not not not not not  not not not not not  not b;
        b = not not not not not  not not not not not  not b;
        b = not not not not not  not not not not not  not b;
        b = not not not not not  not not not not not  not b;
        b = not not not not not  not not not not not  not b;
        x <- x + 1;
      } pool;
      if b then abort() else 0 fi;
    }
  };
};

