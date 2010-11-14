#!/bin/csh -f


for f in working/*
do
  amt=$(($amt+1))

  echo "Testing $f"
  sh differ.sh $f
      
    rm -f disp_us.s disp_them.s res
    python disp_differ.py us.s > disp_us.s
    python disp_differ.py them.s > disp_them.s
    diff disp_us.s disp_them.s > res
    cat res

  if [ -s res ]
  then
      #cat res
      echo "FAIL."
  else
      echo "PASS."

      work=$(($work+1))
  fi ;

done
