
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
    i : Int <- 4;
    s : String <- "Hello world.";

    main():Object { 

            {
                s <- s.substr(3, 3);
                if s = "lo " then out_string("Something tricky is going on") else out_string("This is bad >_>") fi;
            }

        (* if s = "Hello world." then out_string("Good\n") else out_string("bad\n") fi *)


        (* out_string( s.substr(3, 3)) *)
        (* out_string( s.substr(3,3) ) *)
(*
  test
        out_int( let x : Int <- 5, y : Int <- 7 in { 
            x <- x + 1;
        }      )

*)
        (* if not b then out_string("It works!") else out_string("It fails. :(") fi *)

         (*while i <= 10 loop {
            i <- i + 1;
            out_int(i);
            out_string("\n");
        } pool*)
        (*while b loop {
            if c then c <- false else b <- false fi; 
            out_string("Should see this twice.");
        } pool*)
        (* out_int(~5)*)
    };
};


