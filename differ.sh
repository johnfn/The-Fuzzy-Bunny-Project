#Turns example.cl into out.s

#add -r as an argument to run it as well.

make cgen

if [ $# = 2 ]
then
OPTION=$1
FILE=$2
fi

if [ $# = 0 ]
then
OPTION="random"
FILE="example.cl"
fi

if [ $# = 1 ]
then
OPTION="random"
FILE=$1
fi

rm them.s
./lexer $FILE | ./parser | ./semant | ./cgen_soln > them.s
rm us.s
./lexer example.cl | ./parser | ./semant | ./cgen -c > us.s
if [ $OPTION = "-r" ] 
then
    /usr/class/cs143/bin/spim -file out.s
fi
