class Main inherits IO {
    f(x : Int, y : Int, z : Int) : SELF_TYPE { 
{
        out_int(x);
        out_int(y);
        out_int(z);
}
    };
   main() : Object {
        {
            f(1,2,3);

        }
    };
};
