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
./lexer $FILE | ./parser | ./semant | ./cgen > us.s
if [ $OPTION = "-us" ] 
then
    /usr/class/cs143/bin/spim -file us.s
fi

if [ $OPTION = "-them" ]
then
    /usr/class/cs143/bin/spim -file them.s
fi
