
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
    main():SELF_TYPE { 
       {
        s <- "Object";
        s <- "B";

        case s of
            x : Bool => s <- "Object";
            z : B => s <- "B";
            y : String => s <- "String";
        esac;
        
        self.out_string(s);
      }
    };
};
