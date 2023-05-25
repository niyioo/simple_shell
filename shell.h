#ifndef SHELL_H
#define SHELL_H

extern char **environ;
extern char *line;
extern char **commands;
extern char *shell_name;
extern int status;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <signal.h>
#include <limits.h>


/*constants*/
#define READ_SIZE 1024
#define EXTERNAL_COMMAND 1
#define INTERNAL_COMMAND 2
#define PATH_COMMAND 3
#define INVALID_COMMAND -1

#define min(x, y) (((x) < (y)) ? (x) : (y))

/**
 *struct map - a struct that maps a command name to a function
 *
 *@command_name: name of the command
 *@func: the function that executes the command
 */

typedef struct map
{
	char *command_name;
	void (*func)(char **command);
} function_map;


/*help*/
void print_string(char *, int);
char **custom_tokenizer(char *, char *);
void custom_remove_newline(char *);
int _strlen(char *);
void _strcpy(char *, char *);

/*help1*/
int _strcmp(char *, char *);
char *_strcat(char *, char *);
int _strspn(char *, char *);
int _strcspn(char *, char *);
char *_strchr(char *, char);

/*help2*/
char *custom_strtok_r(char *, char *, char **);
int _atoi(char *);
void *custom_realloc(void *ptr, unsigned int old_size, unsigned int new_size);
void handle_ctrl_c(int);
void ignore_comment(char *);

/*util*/
int determine_command_type(char *);
void execute_command(char **, int);
char *search_command_path(char *);
void (*get_internal_func(char *))(char **);
char *_getenv(char *);

/*builtin*/
void custom_env(char **);
void custom_exit(char **);
char *custom_getline(void);
void builtin_setenv(char **command);
void builtin_unsetenv(char **command);

/*builtin1*/
void builtin_cd(char **command);
int handle_logical_operators(char **command, int prev_status);
void replace_variables(char **command);
void set_special_variable(const char *var_name, int value);
char *get_variable_value(const char *var_name);

/*main*/
extern void handle_non_interactive(void);
extern void initialize_command(char **current_command, int type_command);

#endif /*SHELL_H*/
