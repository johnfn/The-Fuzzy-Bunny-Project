(*class C {
	a : Int;
	b : Bool;
	init(x : Int, y : Bool) : C {
           {
		a <- x;
		b <- y;
		self;
           }
	};
};*)

(* TODO: test Assignment to undefined variable a *)

class A{

};

class B inherits A{

};

class C inherits B{

};

class Random{

};
class Main {
    a : String;
    b : Int;
    c : A;
    mm : Main;


    (*
    a : String <- "Okay.";
    a : String <- "Okay.";
    a : String <- "Okay.";
    a : String <- "Okay.";
    a : String <- "Okay.";
    a : String <- "Okay.";
    a : String <- "Okay.";
    a : String <- "Okay.";
    a : String <- "Okay.";
    a : String <- "Okay.";
    a : String <- "Okay.";
    a : String <- "Okay.";
    a : String <- "Okay.";
    a : String <- "Okay.";
    a : String <- "Okay.";
    *)

    argtest(x : Int, y : Int) : Int {
        {
            x + y;
        }
    };

    failtest() : Int { 
        {
            5;
            -- y + x;
        }
    };

    copy() : SELF_TYPE { self } ;

    main() : Int {
        {
            (new Object);
            --mm <- copy();

        --derfer <- 6;
            (*
                a <- not a;
                c <- not c; 
            *)


            (* All fine *)
            b <- b + b;
            b <- b - b;
            b <- b * b;
            b <- b / b;

            case b of 
                x : Int => 5;
                y : String => "okay";
            esac;
            (* Definitely not fine *)

        (*
            new Nonexistant;
            ~"lol";
            not 6;

            b <- a + b;
            b <- a - b;
            b <- a * b;
            b <- a / b;

            while "YES" loop "Derp" pool; (* Wrong, no boolean conditional *)

            b <- isvoid b;

            a = 6;

            c <- (new Random);

            b <- if (a = 6)
                then 6
                else 7
                fi ;

        *)
            (* The rest are fine *) 

            while true loop true pool; 

            let a : Int in (a + 1); 

            if true then "lol" else 6 fi; (* This should be wrong but the soln doesn't do anything about it *)
            if (isvoid b) then "boo" else "yay" fi; 
            b < b;
            b <= b; 
            6;
        }
    }; 
	(* main():C {
	  (new C).init(1,true)
	}; *)
};
