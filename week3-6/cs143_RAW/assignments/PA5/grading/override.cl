class A {
  f():Int {1};
  g():Int {2};
};

class B inherits A {
  g():Int {3};
};

class C inherits B {
  f():Int {4};
};

class D inherits C {
  f():Int {5};
  g():Int {6};
};

class Main inherits IO {
  a:A<-new A; 
  b:B<-new B;
  c:C<-new C;
  d:D<-new D;
  main():Object {{
    out_int(a.f());
    out_int(a.g());
    out_int(b.f());
    out_int(b.g());
    out_int(c.f());
    out_int(c.g());
    out_int(d.f());
    out_int(d.g());
    out_int(a@A.f());
    out_int(a@A.g());
    out_int(b@A.f());
    out_int(b@A.g());
    out_int(c@B.f());
    out_int(c@B.g());
    out_int(d@C.f());
    out_int(d@C.g());
    a<-new B;
    b<-new C;
    c<-new D;
    out_int(a.f());
    out_int(a.g());
    out_int(b.f());
    out_int(b.g());
    out_int(c.f());
    out_int(c.g());
    out_string("\n");
  }}; 
};