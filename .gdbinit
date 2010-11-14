shell rm stuff
shell ./lexer example.cl | ./parser | ./semant > stuff
file cgen 
r -c < stuff
