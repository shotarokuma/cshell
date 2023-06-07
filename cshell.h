#ifndef CSHELL_H
#define CSHELL_H

#include <time.h>
#include <string.h>
#include <stdlib.h>

#define MAX_SIZE 999
#define LIMIT_SIZE 999

typedef struct {
  char* name;
  time_t time;
} Command;

typedef struct {
  char* name;
  char* value;
} EnvVar;

typedef struct {
  int currLogs;
  int currEnvs;
  Command logs[LIMIT_SIZE];
  EnvVar envs[LIMIT_SIZE];
} CShell;

void CShell_init(CShell* cshell);
void CShell_appendEnv(CShell* cshell, const char* name, const char* value);
void CShell_appendLogs(CShell* cshell, const char* name);

void CShell_init(CShell* cshell) {
  cshell->currLogs = 0;
  cshell->currEnvs = 0;
}

void CShell_appendEnv(CShell* cshell, const char* name, const char* value) {
  EnvVar newEnv;
  newEnv.name = strdup(name);
  newEnv.value = strdup(value);
  cshell->envs[cshell->currEnvs] = newEnv;
  cshell->currEnvs++;
}

void CShell_appendLogs(CShell* cshell, const char* name) {
  time_t t = time(NULL);
  Command newCom;
  newCom.name = strdup(name);
  newCom.time = t;
  cshell->logs[cshell->currLogs] = newCom;
  cshell->currLogs++;
}

#endif
