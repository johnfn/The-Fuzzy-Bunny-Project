
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
        s <- "B";
        -- case d fails
        d <- new A;
        case b of
            x : Bool => s <- "Bool";
            y : B => s <- "AAA";
        esac;
        self.out_string(s);
        5;
        -- self.out_string(s);
        -- self.out_string("WHAT?");
      }
    };
};
