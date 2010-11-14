
(*  Example cool program testing as many aspects of the code generator
    as possible.
 *)

class B{
    b():Int{5};
};

class A inherits B{};

class Main inherits IO{ 
    a : Int <- 5;
    x : B <- new B;
    y : String <- "blahblah";
    z : Int <- (3 * 5 * 9);
    main():SELF_TYPE { self.out_string("hello world") };
};

