-- new-st.cl
-- catch a tricky mistake with new SELF_TYPE
  
class A {
  x : Int <- x + 1;
  printX():Object {{
    (new IO).out_string("x is ");
    (new IO).out_int(x);
    (new IO).out_string("\n");
  }}; 
  new_st():A {
    new SELF_TYPE
  };
  bump():Object {
    x <- x + 1
  };
};

class Main {
  main():Object {
    let
      a1 : A <- new A,     -- 'x' is 1
      a2 : A
    in {
      a1.printX();
      a1.bump();
      a1.bump();
      a1.bump();
      a1.printX();         -- 'x' is 4
      a2 <- a1.new_st();   -- 'x' is 5 if copy of 'a1' made!
      a2.printX();         -- a2.x should be 1
      a1.printX();         -- make sure it's still 4
    }
  };
};



