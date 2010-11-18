shell rm stuff
shell ./lexer wellformed/primes.cl | ./parser | ./semant > stuff
file cgen 
#break cgen.cc:1572
r < stuff

