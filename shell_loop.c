#include "shell.h"

int hsh(info_t *info, char **av)
{
        ssize_t r = 0;
        int builtin_ret = 0;

        while (r != -1 && builtin_ret != -2)
        {
                initializeInfo(info);
                if (checkInteractiveMode(info))
                        printString("$ ");
                _custom_output_char(BUF_FLUSH);
                r = getInput(info);
                if (r != -1)
                {
                        configureInfo(info, av);
                        builtin_ret = find_builtin_command(info);
                        if (builtin_ret == -1)
                                find_shell_command(info);
                }
                else if (checkInteractiveMode(info))
                        putCharacter('\n');
                releaseInfo(info, 0);
        }
        record_history(info);
        releaseInfo(info, 1);
        if (!checkInteractiveMode(info) && info->status)
                exit(info->status);
        if (builtin_ret == -2)
        {
                if (info->err_num == -1)
                        exit(info->status);
                exit(info->err_num);
        }
        return (builtin_ret);
}

int find_builtin_command(info_t *info)
{
        int i, built_in_ret = -1;
        builtin_table builtintbl[] = {
                {"exit", _custom_exit},
                {"env", displayEnvironmentInfo},
                {"help", _custom_help},
                {"history", displayHistory},
                {"setenv", customizeEnvironmentVariable},
                {"unsetenv", removeEnvironmentVariable},
                {"cd", _custom_cd},
                {"alias", manageAlias},
                {NULL, NULL}
        };

        for (i = 0; builtintbl[i].type; i++)
                if (str_compare(info->argv[0], builtintbl[i].type) == 0)
                {
                        info->line_count++;
                        built_in_ret = builtintbl[i].func(info);
                        break;
                }
        return (built_in_ret);
}

void find_shell_command(info_t *info)
{
        char *path = NULL;
        int i, k;

        info->path = info->argv[0];
        if (info->linecount_flag == 1)
        {
                info->line_count++;
                info->linecount_flag = 0;
        }
        for (i = 0, k = 0; info->arg[i]; i++)
                if (!isDelimiter(info->arg[i], " \t\n"))
                        k++;
        if (!k)
                return;

        path = search_in_path(info, findEnvironmentVariable(info, "PATH="), info->argv[0]);
        if (path)
        {
                info->path = path;
                fork_shell_command(info);
        }
        else
        {
                if ((checkInteractiveMode(info) || findEnvironmentVariable(info, "PATH=")
                        || info->argv[0][0] == '/') && check_command(info, info->argv[0]))
                        fork_shell_command(info);
                else if (*(info->arg) != '\n')
                {
                        info->status = 127;
                        PrintError(info, "not found\n");
                }
        }
}

void fork_shell_command(info_t *info)
{
        pid_t child_pid;

        child_pid = fork();
        if (child_pid == -1)
        {
                /* TODO: PUT ERROR FUNCTION */
                perror("Error:");
                return;
        }
        if (child_pid == 0)
        {
                if (execve(info->path, info->argv, fetchEnvironment(info)) == -1)
                {
                        releaseInfo(info, 1);
                        if (errno == EACCES)
                                exit(126);
                        exit(1);
                }
                /* TODO: PUT ERROR FUNCTION */
        }
        else
        {
                wait(&(info->status));
                if (WIFEXITED(info->status))
                {
                        info->status = WEXITSTATUS(info->status);
                        if (info->status == 126)
                                PrintError(info, "Permission denied\n");
                }
        }
}
