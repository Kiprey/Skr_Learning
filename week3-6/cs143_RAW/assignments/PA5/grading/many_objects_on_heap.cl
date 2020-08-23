class Main inherits IO {
  a:Int;
  i:Int<-100;
  main():Object{{
    while ~i<0 loop {
      a<-new Int;
      let b:IO<-new IO in b;
      i<-i-1;
    } pool;
    out_int(i);
  }};
};

    