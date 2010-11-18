class Main {
    (* test of single block *)
    someVar : Int;
    mainVar : SELF_TYPE;

    something : Object;
    someFun(x : String) : Int {
        6
    };
    main() : Int {
        {
            someVar <- (someVar <- 6); (*Test assignment return value *)

            (*someVar <- ( let (xx <- "something") in let (yy <- 6) in yy);  I think that this is correct. *)

            something <- while true loop "derp" pool; (* Return value of loop is Object. This passes *)

            something <- if true then 6 else "six" fi; (* Actually, this is right. *)

            (* mainVar <- (new self); This just seems like a bad idea... but it should work... *)
        }
    }; 
};
