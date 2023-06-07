#include "cshell.h"
#include <stdio.h>
#include <string.h>
const int MAX_INPUT = 100;

void operate()
{
  // TODO
}

void interactiveMode(CShell cshell)
{
  char input[MAX_INPUT];
  while(!strcmp(input, "exit"))
  {
    operate();
  }
}


int main( int argc, char *argv[] )
{
  CShell cshell;
  if(argc == 1){
    interactiveMode(cshell);
  }
  return 0;
}