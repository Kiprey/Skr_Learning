class Main inherits IO {
  i:Int;
  s:String;
  b:Bool;
  io:IO;

  main():Object {{
    out_int(i);
    out_string(s);
    if b then
      out_string("true")
    else
      out_string("false")
    fi;
    if isvoid io  then
      out_string("void")
    else
      out_string("not void")
    fi;
  }};
};