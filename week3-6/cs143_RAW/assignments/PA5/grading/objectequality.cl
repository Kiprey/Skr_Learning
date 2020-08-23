class A {
	x:Int <- 5;
	foo(y:Int):SELF_TYPE { { x <- y; self; } };
};
class B inherits A {
};

class Main {
	main():Object {{
		let x:B <- new B in {
			if x = x then 0 else abort() fi;
			if x = new B then abort() else 0 fi;
			if new A = new A then abort() else 0 fi;
			let y:A <- x in {
				if y = x then 0 else abort() fi;
				if y.foo(3) = x then 0 else abort() fi;
			};
		};
		let x:A, y:B in if x = y then 0 else abort() fi; 
	}};
};
