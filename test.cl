
class B {};

class Main inherits IO {
    b:B <- new B;
    main() : Object {
        {
            if b=b then out_string("good") else out_string("Bad") fi;
        }
    };
};

