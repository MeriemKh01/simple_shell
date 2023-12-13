#include "shell.h"

int _custom_exit(info_t *info)
{
        int exitcheck;

        if (info->argv[1])  /* If there is an exit arguement */
        {
                exitcheck = strToInt(info->argv[1]);
                if (exitcheck == -1)
                {
                        info->status = 2;
                        PrintError(info, "Illegal number: ");
                        _custom_output_string(info->argv[1]);
                        _custom_output_char('\n');
                        return (1);
                }
                info->err_num = strToInt(info->argv[1]);
                return (-2);
        }
        info->err_num = -1;
        return (-2);
}

int _custom_cd(info_t *info)
{
        char *s, *dir, buffer[1024];
        int chdir_ret;

        s = getcwd(buffer, 1024);
        if (!s)
                printString("TODO: >>getcwd failure emsg here<<\n");
        if (!info->argv[1])
        {
                dir = findEnvironmentVariable(info, "HOME=");
                if (!dir)
                        chdir_ret = /* TODO: what should this be? */
                                chdir((dir = findEnvironmentVariable(info, "PWD=")) ? dir : "/");
                else
                        chdir_ret = chdir(dir);
        }
        else if (str_compare(info->argv[1], "-") == 0)
        {
                if (!findEnvironmentVariable(info, "OLDPWD="))
                {
                        printString(s);
                        putCharacter('\n');
                        return (1);
                }
                printString(findEnvironmentVariable(info, "OLDPWD=")), putCharacter('\n');
                chdir_ret = /* TODO: what should this be? */
                        chdir((dir = findEnvironmentVariable(info, "OLDPWD=")) ? dir : "/");
        }
        else
                chdir_ret = chdir(info->argv[1]);
        if (chdir_ret == -1)
        {
                PrintError(info, "can't cd to ");
                _custom_output_string(info->argv[1]), _custom_output_char('\n');
        }
        else
        {
                setEnvVariable(info, "OLDPWD", findEnvironmentVariable(info, "PWD="));
                setEnvVariable(info, "PWD", getcwd(buffer, 1024));
        }
        return (0);
}

int _custom_help(info_t *info)
{
        char **arg_array;

        arg_array = info->argv;
        printString("help call works. Function not yet implemented \n");
        if (0)
                printString(*arg_array); /* temp att_unused workaround */
        return (0);
}
