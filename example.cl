
(*  Example cool program testing as many aspects of the code generator
    as possible.
 *)

class Main inherits IO{ 
    a : Int <- 5;
    main():SELF_TYPE { self.out_string("hello world") };
};

