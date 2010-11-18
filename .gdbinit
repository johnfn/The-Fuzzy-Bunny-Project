shell rm stuff
shell ./lexer test.cl | ./parser | ./semant > stuff
file cgen 
#break cgen.cc:1572
r < stuff

