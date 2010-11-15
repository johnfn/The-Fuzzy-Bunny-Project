
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
    (*a : Int <- 5;
    x : B <- new B;
    y : String <- "blahblah";
    z : Int <- (3 * 5 * 9);*)
    main():Int { 5 (* self.out_string("hello world") *) };
};

