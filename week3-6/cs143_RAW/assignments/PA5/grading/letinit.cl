class Main inherits IO {
	main():Object {{
		let x:Bool, y:Int, z:String in 
			if x then abort() else { 
				out_int(y + 1); 
				out_string(z.concat("test\n")); 
			} fi;
	}};
};
