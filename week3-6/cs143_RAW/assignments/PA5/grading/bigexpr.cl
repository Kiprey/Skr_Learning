class Main inherits IO {
  main():IO {
	let x:Int <- 5 in
	    out_int((x <- 1) + ((x <- x+1) 
	               + (3 + (4 + (5 + (6 + (7+ (x+6))))))))  -- 36
  };
};
