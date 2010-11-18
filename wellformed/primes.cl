
(*
 * methodless-primes.cl
 *
 * Designed by Jesse H. Willett, jhw@cory, 11103234, with 
 *             Istvan Siposs, isiposs@cory, 12342921.
 *
 * This program generates primes in order without using any methods.
 * Actually, it does use three methods: those of IO to print out each prime, and
 * abort() to halt the program.  These methods are incidental, however,
 * to the information-processing functionality of the program.  We
 * could regard the attribute 'out's sequential values as our output,
 * and the string "halt" as our terminate signal.
 *
 * Naturally, using Cool this way is a real waste, basically reducing it 
 * to assembly without the benefit of compilation.  
 *
 * There could even be a subroutine-like construction, in that different
 * code could be in the assign fields of attributes of other classes,
 * and it could be executed by calling 'new Sub', but no parameters
 * could be passed to the subroutine, and it could only return itself.
 * but returning itself would be useless since we couldn't call methods
 * and the only operators we have are for Int and Bool, which do nothing
 * interesting when we initialize them!
 *)

class Main inherits IO {

  main() : Int {	-- main() is an atrophied method so we can parse. 
    0 
  };

  out : Int <-		-- out is our 'output'.  It's values are the primes.
    {
      out_string("2 is trivially prime.\n");
      2;
    };

  testee : Int <- out;	-- testee is a number to be tested for primeness.   

  divisor : Int;	-- divisor is a number which may factor testee.

  stop : Int <- 500;	-- stop is an arbitrary value limiting testee. 	
}; (* end of Main *)

