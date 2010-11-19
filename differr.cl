class List {

   isNil() : Bool { true };
   tail()  : List { {  self; } };

   cons(i : Int) : List {
      (new Cons).init(i, self)
   };

};



class Cons inherits List {

   car : Int;	-- The element in this list cell
   cdr : List;	-- The rest of the list
   isNil() : Bool { false };
   tail()  : List { cdr };
   init(i : Int, rest : List) : List {
      {
	 cdr <- rest;
	 self;
      }
   };

};

class Main inherits IO {

   mylist : List;

   print_list(l : List) : Object {
       if l.isNil() then self else print_list(l.tail()) fi
   };

   main() : Object {
      {
	 mylist <- (new List).cons(1);

       print_list(mylist);
        out_string("howboutnow");
      }
   };

};
