







class CellularAutomaton inherits IO {
    population_map : String ;
   
    evolve() : Int {
        (let temp : String in 
            1
        ) 
    };
};

class Main inherits IO{
    cells : CellularAutomaton;
   
    main() : SELF_TYPE {
        {
            cells <- (new CellularAutomaton);

            cells.evolve();
            out_string("?");

            self;
        }
    };
};
