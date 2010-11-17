
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
    c : Bool <- true;

    main():Object { 
        while b loop {
            if c then c <- false else b <- false fi; 
            out_string("Should see this twice.");
        } pool
    };
};

