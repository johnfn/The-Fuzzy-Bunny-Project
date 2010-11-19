
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

Class Main {

    books : BookList;
    c : Cons;
    main() : Object {
        {   
            c <- new Cons;
            c.print_list();
        }
    };
};
