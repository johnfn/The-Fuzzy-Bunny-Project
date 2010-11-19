
(*  Example cool program testing as many aspects of the code generator
    as possible.
 *)
class B inherits IO{
};
class A inherits B{
};
(*
class B{
     b():Int{5};
};
*)

class Main inherits IO{ 
    b : Bool <- true;
    c : Bool <- true;
    i : Int <- 0;
    d : A;
    s : String <- "empty";
    main():Int { 
       {
        s <- "Object";
        s <- "nothingnothing";
        -- case d fails
        d <- new A;
        case d of
            z : B => s <- "boobooB";
            y : String => s <- "String";
            x : Bool => s <- "Bool";
            w : Int => s <- "Int";
        esac;
        self.out_string(s.concat(" | something | \n"));
        5;
        -- self.out_string(s);
        -- self.out_string("WHAT?");
      }
    };
};
