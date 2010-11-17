
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
    s : String <- "Hello.";

    x : Int <- 6;

    main():SELF_TYPE { 
    {
        x <- 1;
        self.out_int(x);

        s <- "Hi!";
        self.out_string(s);
    }
};
};

