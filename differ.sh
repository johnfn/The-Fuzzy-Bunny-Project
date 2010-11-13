#Turns example.cl into out.s

#add -r as an argument to run it as well.


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

rm out.s
./lexer $FILE | ./parser | ./semant | ./cgen_soln > out.s
if [ $OPTION = "-r" ] 
then
    /usr/class/cs143/bin/spim -file out.s
fi
