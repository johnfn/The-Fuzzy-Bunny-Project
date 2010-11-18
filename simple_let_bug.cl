class B inherits IO{
};
class A inherits B{
};

class Main inherits IO{ 
    c : Bool <- true;
    i : Int <- 0;
    s : String <- "empty";
    main():SELF_TYPE { 
       {
        s <- "Object";
        s <- "B";
          
        let temp : String in
        {
                self.out_string(temp);
        };

        self.out_string(s);
      }
    };
};

