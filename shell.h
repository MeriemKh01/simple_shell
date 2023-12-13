#ifndef _SHELL_H_
#define _SHELL_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>

/* for read/write buffers */
#define readBuffer_SIZE 1024
#define READ_BUF_SIZE 1024
#define WRITE_BUF_SIZE 1024
#define BUF_FLUSH -1

/* for command chaining */
#define CMD_NORM        0
#define CMD_OR          1
#define CMD_AND         2
#define CMD_CHAIN       3

/* for ConvertNumber() */
#define CONVERT_LOWERCASE       1
#define CONVERT_UNSIGNED        2

/* 1 if using system getline() */
#define USEcustomGetline 0
#define USE_STRTOK 0

#define HIST_FILE       ".simple_shell_history"
#define HIST_MAX        4096

extern char **environ;


/**
 * struct liststr - singly linked list
 * @num: the number field
 * @str: a string
 * @next: points to the next node
 */
typedef struct liststr
{
        int num;
        char *str;
        struct liststr *next;
} list_t;

/**
 *struct passinfo - contains pseudo-arguements to pass into a function,
 *              allowing uniform prototype for function pointer struct
 *@arg: a string generated from getline containing arguements
 *@argv: an array of strings generated from arg
 *@path: a string path for the current command
 *@argc: the argument count
 *@line_count: the error count
 *@err_num: the error code for exit()s
 *@linecount_flag: if on count this line of input
 *@fname: the program filename
 *@env: linked list local copy of environ
 *@environ: custom modified copy of environ from LL env
 *@history: the history node
 *@alias: the alias node
 *@env_changed: on if environ was changed
 *@status: the return status of the last exec'd command
 *@cmd_buf: address of pointer to cmd_buf, on if chaining
 *@cmd_buf_type: CMD_type ||, &&, ;
 *@readfd: the fd from which to read line input
 *@histcount: the history line number count
 */
typedef struct passinfo
{
        char *arg;
        char **argv;
        char *path;
        int argc;
        unsigned int line_count;
        int err_num;
        int linecount_flag;
        char *fname;
        list_t *env;
        list_t *history;
        list_t *alias;
        char **environ;
        int env_changed;
        int status;

        char **cmd_buf; /* pointer to cmd ; chain buffer, for memory mangement */
        int cmd_buf_type; /* CMD_type ||, &&, ; */
        int readfd;
        int histcount;
} info_t;

#define INFO_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
        0, 0, 0}

/**
 *struct builtin - contains a builtin string and related function
 *@type: the builtin command flag
 *@func: the function
 */
typedef struct builtin
{
        char *type;
        int (*func)(info_t *);
} builtin_table;


/* toem_shloop.c */
int hsh(info_t *, char **);
int find_builtin_command(info_t *);
void find_shell_command(info_t *);
void fork_shell_command(info_t *);

/* toem_parser.c */
int check_command(info_t *, char *);
char *copy_characters(char *, int, int);
char *search_in_path(info_t *, char *, char *);

/* loophsh.c */
int loophsh(char **);

/* toem_errors.c */
void _custom_output_string(char *);
int _custom_output_char(char);
int _output_to_fd(char c, int fd);
int _output_string_to_fd(char *str, int fd);

/* toem_string.c */
int str_length(char *);
int str_compare(char *, char *);
char *start_prefix(const char *, const char *);
char *str_concatenate(char *, char *);

/* toem_string1.c */
char *copyString(char *, char *, int);
char *duplicateString(const char *);
void printString(char *);
int putCharacter(char);

/* toem_exits.c */
char *copyString(char *, char *, int);
char *concatenateString(char *, char *, int);
char *findCharInString(char *, char);

/* toem_tokenizer.c */
char **splitStringWords(char *, char *);
char **splitStringWords2(char *, char);

/* toemcustom_realloc.c */
char *_custom_set(char *, char, unsigned int);
void free_strings(char **);
void *custom_realloc(void *, unsigned int, unsigned int);

/* toem_memory.c */
int free_and_null(void **);

/* toem_atoi.c */
int checkInteractiveMode(info_t *);
int isDelimiter(char, char *);
int isAlphabetic(int);
int customAtoi(char *);

/* toem_errors1.c */
int strToInt(char *);
void PrintError(info_t *, char *);
int PrintDecimal(int, int);
char *ConvertNumber(long int, int, int);
void RemoveComments(char *);

/* toem_builtin.c */
int _custom_exit(info_t *);
int _custom_cd(info_t *);
int _custom_help(info_t *);

/* toem_builtin1.c */
int displayHistory(info_t *);
int manageAlias(info_t *);

/*toem_Getline.c */
ssize_t getInput(info_t *);
int customGetline(info_t *, char **, size_t *);
void interruptHandler(int);

/* toem_getinfo.c */
void initializeInfo(info_t *);
void configureInfo(info_t *, char **);
void releaseInfo(info_t *, int);

/* toem_environ.c */
char *findEnvironmentVariable(info_t *, const char *);
int displayEnvironmentInfo(info_t *);
int customizeEnvironmentVariable(info_t *);
int removeEnvironmentVariable(info_t *);
int fillEnvironmentList(info_t *);

/* toemf_getenv.c */
char **fetchEnvironment(info_t *);
int removeEnvVariable(info_t *, char *);
int setEnvVariable(info_t *, char *, char *);

/* toem_history.c */
char *obtain_history_file(info_t *info);
int record_history(info_t *info);
int retrieve_history(info_t *info);
int create_history_list(info_t *info, char *buf, int linecount);
int reorder_history(info_t *info);

/* toem_lists.c */
list_t *include_node(list_t **, const char *, int);
list_t *add_node_to_end(list_t **, const char *, int);
size_t display_list_content(const list_t *);
int remove_node_at_index(list_t **, unsigned int);
void clear_list(list_t **);

/* toem_lists1.c */
size_t determine_list_length(const list_t *);
char **convert_list_to_strings(list_t *);
size_t display_list_elements(const list_t *);
list_t *find_matching_node(list_t *, char *, char);
ssize_t retrieve_node_index(list_t *, list_t *);

/* toem_vars.c */
int is_command_chain(info_t *, char *, size_t *);
void check_command_chain(info_t *, char *, size_t *, size_t, size_t);
int replaceAliases(info_t *);
int replaceVariables(info_t *);
int replaceString(char **, char *);

#endif
