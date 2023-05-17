#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>

#define BUFFER_SIZE 1024
#define DELIMITER " \t\r\n\a"

extern char **environ;

/* Function prototypes */
void prompt(void);
ssize_t read_command(char *buffer, size_t buffer_size);
int execute_command(char *command, char **args);
char **split_input(char *input);
void print_error(const char *command, const char *error);
char *_getenv(const char *name);
char *find_path(void);
char *_path(char **args);
char **split_path(const char *path);
char *str_concat(const char *str1, const char *str2, char delimiter);
void free_split(char **array);
int execute(char **args);

void shell_loop(void);

#endif /* SHELL_H */
