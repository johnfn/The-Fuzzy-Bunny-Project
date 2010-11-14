#!/bin/csh -f

amt = 0
work = 0


if [ $# = 1 ]
then

    echo "Testing $1"

    rm output_answer output_me res res2 res_final

    ./lexer $1 | ./parser | ./semantsoln &> output_answer
    ./mysemant $1 &> output_me
    
    diff output_answer output_me > res     

    if [ -s res_final ]
    then
        #cat res_final
        echo "FAIL."
    else
        echo "PASSED."
    fi ;

else

    for f in illformed/*
    do
        amt=$(($amt+1))

        echo "Testing $f"

        rm output_answer output_me res 
            

        ./lexer $f | ./parser | ./semantsoln > output_answer 2>> output_answer
        ./mysemant $f > output_me 2>> output_me

        cat output_me | sort > output_me_done
        cat output_answer | sort > output_answer_done

        diff output_answer_done output_me_done > res     
        
        if [ -s res ]
        then
            echo "---------------------------------------------------"
            echo "expected"
            cat output_answer
            echo "received"
            cat output_me
            echo "---------------------------------------------------"
            echo "FAIL."
        else
            echo "PASS."

            work=$(($work+1))
        fi ;

    done

  for f in wellformed/*
  do
      amt=$(($amt+1))

      echo "Testing $f"

      rm output_answer output_me res 
          

      ./lexer $f | ./parser | ./semantsoln > output_answer 2>> output_answer
      ./mysemant $f > output_me 2>> output_me

        cat output_me | sort > output_me_done
        cat output_answer | sort > output_answer_done

      diff output_answer_done output_me_done > res     
      
      if [ -s res ]
      then
          #cat res
          echo "FAIL."
      else
          echo "PASS."

          work=$(($work+1))
      fi ;

  done

   for f in individual/*
   do
       amt=$(($amt+1))

       echo "Testing $f"

       rm output_answer output_me res 
           

       ./lexer $f | ./parser | ./semantsoln > output_answer 2>> output_answer
       ./mysemant $f > output_me 2>> output_me

        cat output_me | sort > output_me_done
        cat output_answer | sort > output_answer_done

       diff output_answer_done output_me_done > res     
       
       if [ -s res ]
       then

           echo "FAIL."
       else
           echo "PASS."

           work=$(($work+1))
       fi ;

   done



   echo "$work of $amt work"


    fi ;
