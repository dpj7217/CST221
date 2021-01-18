#!/bin/bash
# attempt bash

password=$1
passwordLen=${#password}
 
if [[ $passwordLen -gt 8 ]] 
  then
    echo "Passoword is long enough"
  
    if [[ "$password" =~ [0-9] ]] 
      then 
        echo "password has a number. Good" 
        if [[ "$password" =~ [@#$%\&*+-=] ]]
          then
            echo "Password has a special character. It cannot contain @, #, $, %, &, *, +, -, or =. Please try again."
            exit 1
        fi
    else 
      echo "Password must contain a number. Please try again"
      exit 1
    fi
  else 
    echo "Password is too short. Please try again"
    exit 1
fi 

echo "Your password is secure"
