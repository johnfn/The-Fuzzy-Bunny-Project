
class B inherits IO{
    x: String <- "Ffffff";
    y: Int <- 5;
    z: Int <- 6;
};

class C inherits B{
    a: Int <- 1;
    b: Int <- 2;
    c: Int <- y;
    
    boom() :Object {
        out_int(a).
        out_int(b).
        out_int(c).
        out_string(x).
        out_int(y).
        out_int(z)
    };
};

class Main inherits IO {
    c:C <- new C;
    main() : Object {
        {
            c.boom();
        }
    };
};

