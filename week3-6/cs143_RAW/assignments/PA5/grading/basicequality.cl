class Main {
	main():Object {{
		if (true = false) then abort() else 0 fi;
		if (true = true) then 0 else abort() fi;
		if ("hello" = "hello".copy()) then 0 else abort() fi;
		let a:String in if (a = "") then 0 else abort() fi;
		if 5 = 6 then abort() else 0 fi;
	}};

};
