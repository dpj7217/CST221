#!/bin/bash

usersFile=$1
groupName=$2
operationFlag=$3


function removeUsers() {

  #next read file line by line to get users
  while IFS= read -r line;
  do
    #delimite line by _ character to get username, encrypted password
    currUser=($line)
     
    sudo su -c "userdel -r ${currUser[0]}"

  done < $usersFile
}

function addUsers() {
  

  
  #next read file line by line to get users
  while IFS= read -r line;
  do
  
    #helper variables
    count=0
    
    #delimite line by _ character to get username, encrypted password
    currUser=($line)
    
    sudo su -c "useradd ${currUser[0]} -s /bin/bash -m -p ${currUser[1]}"
    
        
  done < $usersFile
  

}



#first check to make sure file exists
if [[ ! -f $usersFile ]] 
then
  echo "No file found in that directory with that name"
  exit -1
fi  
  
#next read operation flag to determine which to do
if [[ $operationFlag = "-r" ]]
then
  #remove users from system if they exist
  removeUsers
  
elif [[ $operationFlag = "-a" ]]
then
  #add users to system with groupname
  addUsers
  
else 
  echo "Incorrect flag specificied please try again"
  exit -1
fi
  
  




