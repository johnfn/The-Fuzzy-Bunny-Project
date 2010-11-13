#Turns example.cl into out.s

#add -r as an argument to run it as well.

rm out.s
./lexer example.cl | ./parser | ./semant | ./cgen_soln > out.s
if [ "$1" = "-r" ] 
then
    /usr/class/cs143/bin/spim -file out.s
fi
