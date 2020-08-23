class Main inherits IO {
	y:Int;
	f(x:Int):Int { y <- x + 6 + 7 + 8 + 9 };
	main():Object { out_int(y <- 1 + 2 + 3 + 4 + f(5)) };
};
