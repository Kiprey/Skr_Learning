class Main inherits IO {
  a:Int<-0;
  b:Int<-1;
  main():Object {{
    a<-b+1;
    b<-a+1;
    a<-b+1;
    b<-a+1;
    a<-b+1;
    b<-a+1;
    a<-b+1;
    b<-a+1;
    a<-b+1;
    b<-a+1;
    out_int(a+b);
  }};
};