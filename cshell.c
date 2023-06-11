#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "cshell.h"

void modifyInput(char command[MAX_ARGS][MAX_INPUT], char input[])
{
  int i = 0;
  char *token = strtok(input, " ");
  while (token != NULL)
  {
    strcpy(command[i], token);
    i++;
    token = strtok(NULL, " ");
  }
  strcpy(command[i], "\0");
}

void operate(CShell *cshell, char input[])
{
  char command[MAX_ARGS][MAX_INPUT];
  modifyInput(command, input);
  pid_t pid = fork();
  if (pid < 0)
  {
    exit(1);
  }
  else if (pid != 0)
  {
    wait(NULL);
  }

  if (pid == 0 && strcmp(command[0], "log") == 0)
  {
    CShell_printLogs(cshell);
    exit(0);
  }
  else if (pid == 0 && strcmp(command[0], "print") == 0)
  {
    CShell_print(cshell, command);
    exit(0);
  }
  else if (pid == 0 && strcmp(command[0], "theme") == 0)
  {
    CShell_theme(cshell, command[1]);
    exit(0);
  }else if(pid == 0){
    system(input);
    exit(0);
  }

  if (command[0][0] == '$')
  {
    char name[MAX_INPUT];
    char value[MAX_INPUT];
    strcpy(name, strtok(command[0], "="));
    strcpy(value, strtok(NULL, "="));
    CShell_appendEnv(cshell, name + 1, value);
  }
}

void interactiveMode(CShell *cshell)
{
  char input[MAX_INPUT];
  printf("%s", cshell->name);

  while (fgets(input, sizeof(input), stdin))
  {
    input[strcspn(input, "\n")] = '\0';
    if(strcmp(input, "exit") == 0)
    {
      break;
    }
    CShell_appendLogs(cshell, input);
    operate(cshell, input);
    printf("%s", cshell->name);
  }
}

int main(int argc, char *argv[])
{
  CShell *cshell = malloc(sizeof(CShell));
  CShell_init(cshell);

  if (argc == 1)
  {
    interactiveMode(cshell);
  }
  CShell_cleanup(cshell);
  return 0;
}
