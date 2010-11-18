
(*  Example cool program testing as many aspects of the code generator
    as possible.
 *)
class B inherits IO{
};

class A inherits B{
};

class Main inherits IO{ 
    b : Bool <- true;
    c : Bool <- true;
    i : Int <- 0;
    d : A;
    s : String <- "empty";
    main():SELF_TYPE { 
       { 
        case i of
            x : Bool => s <- "Object";
            z : B => s <- "B";
            y : String => s <- "String";
        esac;
        
        self.out_string(s);
      }

       (* 
        (*
        self.out_string(s);

        while i<10 loop {
            i <- i + 1;
            out_int(i);
            out_string("\n");
        } pool;
*)
        if s.substr(3,3) = "lo " then out_string("Something tricky is going on") else out_string("I guess it's just pointers") fi
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
        (* out_int(~5)*) *)
    };
};


