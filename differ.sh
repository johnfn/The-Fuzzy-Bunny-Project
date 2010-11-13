#Turns example.cl into out.s

#add -r as an argument to run it as well.

#add -o to "o"nly run the .s file (not make it from scratch)
if [ "$1" != '-o' ]
then
    rm out.s
    ./lexer example.cl | ./parser | ./semant | ./cgen_soln > out.s
    rm us.s
    ./lexer example.cl | ./parser | ./semant | ./cgen  > us.s
fi

if [ "$1" = "-r" ] 
then
    /usr/class/cs143/bin/spim -file out.s
fi
