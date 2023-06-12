#ifndef CSHELL_H
#define CSHELL_H

#include <time.h>
#include <string.h>
#include <errno.h>
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
  char name[9];
  char bye[4];
} CShell;

void CShell_init(CShell *cshell)
{
  cshell->currLogs = 0;
  cshell->currEnvs = 0;
  strcpy(cshell -> name, "cshell$ ");
  strcpy(cshell -> bye, "Bye!\n");
}

void CShell_appendEnv(CShell *cshell, const char *name, const char *value)
{
  EnvVar newEnv;
  newEnv.name = strdup(name);
  newEnv.value = strdup(value);
   for(int i = 0; i < cshell -> currEnvs; i++)
    {
        if(strcmp(cshell -> envs[i].name, newEnv.name) == 0)
        {
          free(cshell->envs[i].value); 
          cshell->envs[i].value = strdup(value); 
          free(newEnv.name); 
          free(newEnv.value); 
          return;
        }
    }
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
    
    printf("%s", ctime(&log.time));
    printf(" %s\n", log.name);
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
        if(strcmp(cshell -> envs[j].name, &command[1][1]) == 0)
        {
           printf("%s\n", cshell -> envs[j].value);
           return;
        }
      }
      fprintf(stderr,"Error: No Environment Variable %s found.", command[1]);
      printf("%s", "\n");
      return;
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
  else if (strcmp(input, "blue") == 0)
  {
    printf("\x1B[34m");
  }
  else{
    fprintf(stderr, "%s\n", "unsupported theme");
  }
}

#endif
