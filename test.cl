
class B inherits IO{
};
class A inherits B{
};
Class BookList inherits IO{

};

class Book inherits IO{

    print(): Int{ {
        out_string("Bookie");
        5;
    } };
};

class Article inherits Book{


};

Class Cons inherits BookList {
    xcar : Book <- new Article;  -- We keep the car and cdr in attributes.
    a : Book; 
    b : Article;
    print_list() : Object {
        {
            out_string("wjhat");
            -- a <- new Book;
            case a of
                dummy : Article => out_string("- dynamic type was Article -\n");
            esac;
        }
    };
};

class Main inherits IO{
    b : Bool <- true;
    i : Int <- 0;
    d : A;
    s : String <- "empty";
    books : BookList;
    c : Cons;
    main() : Int {
        {
            c <- new Cons;
            c.print_list();
            out_int(10 + if true then 10 else 20 fi);
            s <- "Object";
            s <- "nothingnothing";
            case d of
                z : B => s <- "boobooB";
                y : String => s <- "String";
                x : Bool => s <- "Bool";
                w : Int => s <- "Int";
            esac;
            self.out_string(s.concat(" | something | \n"));
            5;
        }
    };
};

