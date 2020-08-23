class Bob inherits IO {
	x : Int;
	y : Int <- 4;
};

class Main inherits Bob {
	z : Int <- 23;
	jack(q : Int, z : Int) : Int 
	{{ 
		let z:Int <- z + y in print_z(z); -- 25+4 = 29
	        y <- y + 4;
	        let y:Int <- z + y  in y; -- 25+8 = 33
	}};
	print_z(z:Int) : SELF_TYPE 
	{{
		out_string("z = "); 
		out_int(z); 
		out_string("\n"); 
	}};
	main() : Bob
	{{
		print_z(z);	-- 23
		let z:Int <- jack(5,z+2) in
		print_z(z);	--33
	}};
};