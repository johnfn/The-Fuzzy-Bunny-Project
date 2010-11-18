
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
(*
       { 
        case i of
            x : Bool => s <- "Object";
            z : B => s <- "B";
            y : String => s <- "String";
        esac;
        
        self.out_string(s);
      } *) 

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
    i : Int <- 4;
    *)
--        s : String;

    --f():Int{5};


    i : Int <- 9;
    main():Object { 

        out_int( if i=4 then 4 else if i=5 then 5 else if i=1 then 1 else 0 fi fi fi )
        (* {
            out_string( in_string() );
        } *)

        (* out_int( (new B).b()) *)
(*
            {
                let x : Int <- 5 in out_int(x);
                let y : Int <- 5 in out_int(y);
                let z : Int <- 5 in out_int(z);
                let x : Int <- 5 in out_int(x);
                let x : Int <- 5 in out_int(x);
                let x : Int <- 5 in out_int(x);
                let x : Int <- 5 in out_int(x);
                let x : Int <- 5 in out_int(x);
                let x : Int <- 5 in out_int(x);
                let x : Int <- 5 in out_int(x);
                let x : Int <- 2 in out_int(x);
                let x : Int <- 2 in out_int(x);
                let x : Int <- 2 in out_int(x);
                let x : Int <- 2 in out_int(x);
            }
*)

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
        (* out_int(~5)*) *)
    };
};


