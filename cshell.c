#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
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

void nonbuiled(char input[])
{
  int pipefd[2];
  pid_t pid;
  char buffer[MAX_INPUT];
  if (pipe(pipefd) == -1)
  {
    return;
  }

  pid = fork();
  if (pid < 0)
  {
    return;
  }
  else if (pid == 0)
  {
    close(pipefd[0]);
    dup2(pipefd[1], STDOUT_FILENO);
    dup2(pipefd[1], STDERR_FILENO);
    int code = system(input);
    if (code == -1 || WEXITSTATUS(code) == 127)
    {
      exit(1);
    }
    close(pipefd[1]);
    exit(0);
  }
  else
  {
    close(pipefd[1]);
    int status;
    waitpid(pid, &status, 0);
    ssize_t n = read(pipefd[0], buffer, sizeof(buffer));
    if (status == 0 && n > 0)
    {
      fwrite(buffer, 1, n, stdout);
    }
    else
    {
      fprintf(stderr, "Missing keyword or command, or permission problem\n");
    }
    close(pipefd[0]);
  }
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
  }
  else if (pid == 0 && command[0][0] != '$')
  {
    nonbuiled(input);
    exit(0);
  }
  else if (pid == 0)
  {
    exit(0);
  }

  if (command[0][0] == '$')
  {
    char name[MAX_INPUT];
    char value[MAX_INPUT];
    int i = 0;
    while (command[0][i] != '\0')
    {
      if (command[0][i] == '=')
      {
        strcpy(name, strtok(command[0], "="));
        strcpy(value, strtok(NULL, "="));
        CShell_appendEnv(cshell, name + 1, value);
        return;
      }
      i++;
    }
    fprintf(stderr, "%s\n", "Variable value expected");
  }
}

void interactiveMode(CShell *cshell)
{
  char input[MAX_INPUT];
  printf("%s", cshell->name);

  while (fgets(input, sizeof(input), stdin))
  {
    input[strcspn(input, "\n")] = '\0';
    if (strcmp(input, "exit") == 0)
    {
      printf("%s", cshell->bye);
      break;
    }
    CShell_appendLogs(cshell, input);
    operate(cshell, input);
    printf("%s", cshell->name);
  }
}

void scriptMode(CShell *cshell, char input[])
{
  char lines[MAX_ARGS][MAX_INPUT];
  char *line = NULL;
  size_t len = 0;
  int i = 0;
  FILE *script = fopen(input, "r");
  if (script)
  {
    while ((getline(&line, &len, script)) != -1)
    {
      stpcpy(lines[i], line);
      i++;
    }
    fclose(script);
    for (int j = 0; j < i; j++)
    {
      if (j != i - 1)
      {
        lines[j][strcspn(lines[j], "\n")] = '\0';
      }
      if (strcmp(lines[j], "exit") == 0)
      {
        printf("%s", cshell->bye);
        break;
      }
      printf("%s %s\n", cshell->name, lines[j]);
      CShell_appendLogs(cshell, lines[j]);
      operate(cshell, lines[j]);
    }
  }
  else
  {
    fprintf(stderr, "Unable to read script file: %s\n", input);
    exit(1);
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
  else if (argc == 2)
  {
    scriptMode(cshell, argv[1]);
  }

  CShell_cleanup(cshell);
  return 0;
}
