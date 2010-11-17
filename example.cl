
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
    b : Bool <- true;
    c : Bool <- false;

    (* TODO try dispatch w/o self *)
    main():SELF_TYPE { 
        if b then (if c then self.out_string("bah!") else self.out_string("Things are looking good!") fi) else self.out_string("Boo.") fi
    };
};

