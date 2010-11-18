class A inherits IO{
    x : Int;
    y : Int;
    init():Int {{ x <- 1; y <- 1; }};
    setxy():Object {{ x <- 1; y <- 1; }};
    getx():Object { out_int(x) };
};

class Main inherits IO {

    f : A;

    main():Object { 
{
        f <- new A;        
        f.setxy();
        f.getx();
}
    };
};
