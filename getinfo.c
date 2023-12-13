#include "shell.h"

void initializeInfo(info_t *info)
{
        info->arg = NULL;
        info->argv = NULL;
        info->path = NULL;
        info->argc = 0;
}

void configureInfo(info_t *info, char **av)
{
        int i = 0;

        info->fname = av[0];
        if (info->arg)
        {
                info->argv = splitStringWords(info->arg, " \t");
                if (!info->argv)
                {

                        info->argv = malloc(sizeof(char *) * 2);
                        if (info->argv)
                        {
                                info->argv[0] = duplicateString(info->arg);
                                info->argv[1] = NULL;
                        }
                }
                for (i = 0; info->argv && info->argv[i]; i++)
                        ;
                info->argc = i;

                replaceAliases(info);
                replaceVariables(info);
        }
}

void releaseInfo(info_t *info, int all)
{
        free_strings(info->argv);
        info->argv = NULL;
        info->path = NULL;
        if (all)
        {
                if (!info->cmd_buf)
                        free(info->arg);
                if (info->env)
                        clear_list(&(info->env));
                if (info->history)
                        clear_list(&(info->history));
                if (info->alias)
                        clear_list(&(info->alias));
                free_strings(info->environ);
                        info->environ = NULL;
                free_and_null((void **)info->cmd_buf);
                if (info->readfd > 2)
                        close(info->readfd);
                putCharacter(BUF_FLUSH);
        }
}
