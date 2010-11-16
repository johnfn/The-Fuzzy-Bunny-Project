
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
    x : Int <- 5;
    y : Int <- 6;
    z : Int <- 8;

    sthg : Int <- 6;
    main():SELF_TYPE { 
    {
        sthg <- 1+2+3+4/2;
        self.out_int(sthg);
    }
};
};

