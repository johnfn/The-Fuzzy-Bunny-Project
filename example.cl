
(*  Example cool program testing as many aspects of the code generator
    as possible.
 *)
(*
class B{
    b():Int{5};
};

class A inherits B{
    x : B <- new B;
};
*)

class Main inherits IO{ 
    x : Int <- 4;
    (*a : Int <- 5;
    x : B <- new B;
    y : String <- "blahblah";
    z : Int <- (3 * 5 * 9);*)
    main():SELF_TYPE { self.out_int(x + x) };
};

