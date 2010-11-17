
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
    i : Int <- 0;
    s : String <- "empty";
    main():Object { 
        
        case b of
            x : Int => s <- "Int";
            y : String => s <- "String";
        esac
       (* 
        self.out_string(s);

        while i<10 loop {
            i <- i + 1;
            out_int(i);
            out_string("\n");
        } pool;
*)
        (*while b loop {
            if c then c <- false else b <- false fi; 
            out_string("Should see this twice.");
        } pool*)
        (* out_int(~5)*)
    };
};

