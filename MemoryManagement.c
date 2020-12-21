/*   Programmer: David Pratt Jr.
 *
 *   For this situation I made 
 *   a function for each modification that I wanted to make
 *   to the number that the user provided. I also provided
 *   a recursive solution to running the program so that it
 *   can run as long as the user wants. And a way for the 
 *   user to exit the program
 *
 */

#include <stdlib.h>
#include <stdio.h>

unsigned intToBinary(int num) 
{  
  if (num == 0) 
  {
    return 0;  
  }
  
  return (num % 2) + 10 * intToBinary(num / 2);
}

void* intToHex(int num) 
{
  printf("%x\n", num);
}

int leftShiftSixteen(int num) 
{
  return num << 16;
}


int runProgram() 
{

  printf("Enter a number less than 1024 or -1 to exit: \n");
  
  int input;
  scanf("%d", &input);

  if (input == -1) 
  {
    return 0;
  }
  else if (input < -1 || input >= 1024) 
  {
    printf("That number is out of range. Please try again\n\n");
    runProgram();
  }
  else 
  {
    printf("\n\n");
    printf("%d in binary: %d\n", input, intToBinary(input));
    printf("%d in hex: ", input);
    intToHex(input);
    printf("%d bit shifted right by 16: %d\n\n", input, leftShiftSixteen(input));
    
    runProgram();
  }
  
  return 0;
}

int main() 
{

  runProgram();

}
