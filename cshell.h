#ifndef CSHELL_H
#define CSHELL_H

#include <time.h>
#include <string.h>
#include <stdlib.h>

#define MAX_SIZE 999
#define LIMIT_SIZE 999
#define MAX_INPUT 256
#define MAX_ARGS 10

typedef struct
{
  char *name;
  time_t time;
} Command;

typedef struct
{
  char *name;
  char *value;
} EnvVar;

typedef struct
{
  int currLogs;
  int currEnvs;
  Command logs[LIMIT_SIZE];
  EnvVar envs[LIMIT_SIZE];
  char name[8];
} CShell;

void CShell_init(CShell *cshell)
{
  cshell->currLogs = 0;
  cshell->currEnvs = 0;
  strcpy(cshell -> name, "cshell$ ");
}

void CShell_appendEnv(CShell *cshell, const char *name, const char *value)
{
  EnvVar newEnv;
  newEnv.name = strdup(name);
  newEnv.value = strdup(value);
  cshell->envs[cshell->currEnvs] = newEnv;
  cshell->currEnvs++;
}

void CShell_appendLogs(CShell *cshell, const char *name)
{
  time_t t = time(NULL);
  Command newCom;
  newCom.name = strdup(name);
  newCom.time = t;
  cshell->logs[cshell->currLogs] = newCom;
  cshell->currLogs++;
}

void CShell_printLogs(CShell *cshell)
{
  for (int i = 0; i < cshell->currLogs; i++)
  {
    Command log = cshell->logs[i];
    printf("%ld", log.time);
    printf("%s", log.name);
  }
}

void CShell_print(CShell *cshell, char command[MAX_ARGS][MAX_INPUT])
{
  char args[MAX_SIZE];
  strcpy(args, "");
  int i = 0;
  while(strcmp(command[i], "\0") != 0)
  {
    if(i == 1 && command[1][0] == '$')
    {
      for(int j = 0; j < cshell -> currEnvs; j++)
      {
        if(strcmp(cshell -> envs[j].name, &command[0][1]) == 0)
        {
           printf("%s\n", cshell -> envs[i].value);
           return;
        }
      }
    }
    if(i != 0)
    {
       strcat(args, command[i]);
       strcat(args, " ");
    }
    i += 1;
  }
  printf("%s\n", args);
}

void CShell_theme(CShell *cShell, char *input)
{
  if (strcmp(input, "red") == 0)
  {
    printf("\x1B[31m");
  }
  else if (strcmp(input, "green") == 0)
  {
    printf("\x1B[32m");
  }
  else if (strcmp(input, "yellow") == 0)
  {
    printf("\x1B[33m");
  }
  else if (strcmp(input, "blue") == 0)
  {
    printf("\x1B[34m");
  }
  else if (strcmp(input, "magent") == 0)
  {
    printf("\x1B[35m");
  }
  else if (strcmp(input, "cyan") == 0)
  {
    printf("\x1B[36m");
  }
  else if (strcmp(input, "white") == 0)
  {
    printf("\x1B[37m");
  }
  else if (strcmp(input, "black") == 0)
  {
    printf("\x1B[30m");
  }
}

void CShell_cleanup(CShell *cShell)
{
  free(cShell);
}

#endif
