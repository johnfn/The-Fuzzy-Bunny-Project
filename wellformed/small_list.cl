class List {
   -- Define operations on empty lists.
   cons(i : Int) : List {
      (new Cons).init(i, self)
   };

};

class Cons inherits List {

   wr : Int;	-- The element in this list cell

   init(b : Int, rest : List) : List {
      {
	 wr <- b;
	 self;
      }
   };

};

class Main inherits IO{
    main() : Int {
        {
        (new List).cons(3).cons(4);
        6;
        }
    };
};

