#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h>
#include <limits.h>
#include <errno.h>
#include <stdarg.h>
#include <stdbool.h>

#define BUFFER_SIZE 1024
#define DELIMITER " \t\r\n\a"

#define MAX_COMMAND_LENGTH 1024
#define MAX_ARGUMENTS 64
#define MAX_ALIASES 64
#define MAX_PIPELINE_COMMANDS 16
#define MAX_ARGS 10

typedef struct
{
	char name[MAX_COMMAND_LENGTH];
	char value[MAX_COMMAND_LENGTH];
} Alias;

extern char **environ;

/* Function prototypes */
char *_strncpy(char *dest, const char *src, size_t n);
char *_strstr(const char *haystack, const char *needle);
int _printf(const char *format, ...);
int _putchar(char c);
void _perror(const char *message);
int _atoi(char*s);
int _strlen(const char *s);
char *_strcpy(char *dest, const char *src);
int _strcmp(char *s1, char *s2);
char *_strcat(char *dest, char *src);
unsigned int _strspn(char *s, char *accept);
int _strcspn(char *str1, char *str2);
char *_strchr(char *s, char c);
char *_strtok_r(char *str, const char *delim, char **saveptr);
void *_realloc(void *ptr, size_t size);
int _strncmp(const char *s1, const char *s2, size_t n);
char *_strdup(const char *str);
char *_strtok(char *str, const char *delim);
void prompt(void);
ssize_t read_command(char *buffer, size_t buffer_size);
char **split_input(char *input);
void print_error(const char *cmd, const char *msg);
char *_getenv(const char *name);
char *find_path(void);
char *_path(char **args);
char **split_path(const char *path);
char *str_concat(const char *str1, const char *str2, char delimiter);
void free_split(char **array);
int execute(char **args);
void execute_env(void);
ssize_t custom_getline(char *buffer, size_t buffer_size);
void execute_exit(char *status);
void execute_alias(char *name, char *value);
void execute_setenv(char *variable, char *value);
void execute_unsetenv(char *variable);
void execute_cd(char *directory);
char *replace_variable(char *input);
void execute_pipeline(char *program_name, char **commands, int num_commands);
int execute_command(char *program_name, char *command, char *args[]);
int main(int argc, char *argv[]);
void parse_arguments(char *command, char *args[]);
void replace_variables(char *args[]);
char *replace_substring(const char *string, const char *pattern, const char *replacement);
char *expand_variables(const char *command, int last_status);
void handle_special_variables(char *args[], int last_status);
char **split_logical_operators(const char *string, const char *delimiter);
int execute_logical_operators(char *program_name, char **commands, int and_operator);
void tokenize_command(char *command, char *args[], const char *delimiters);
char *find_next_token(char *str, const char *delimiters);
void execute_unalias(char *alias_name);
void execute_help(char *program_name);
int execute_shell(char program_name);

void shell_loop(void);

#endif /* SHELL_H */
