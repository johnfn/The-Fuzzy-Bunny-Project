class Main inherits IO {

    main():Int { 
        6
    };

    g : Int <- 3341;
    f : Int <- 6789;
      stop : Int <- 500;	-- stop is an arbitrary value limiting testee. 	

  m : SELF_TYPE <-		-- m supplants the main method.
        out_int(stop);
};
