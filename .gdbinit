shell rm stuff
shell ./lexer wellformed/cells.cl | ./parser | ./semant > stuff
file cgen 
#break cgen.cc:1572
r -c < stuff

