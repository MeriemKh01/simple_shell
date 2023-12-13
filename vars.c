#include "shell.h"

int is_command_chain(info_t *info, char *buf, size_t *p)
{
        size_t j = *p;

        if (buf[j] == '|' && buf[j + 1] == '|')
        {
                buf[j] = 0;
                j++;
                info->cmd_buf_type = CMD_OR;
        }
        else if (buf[j] == '&' && buf[j + 1] == '&')
        {
                buf[j] = 0;
                j++;
                info->cmd_buf_type = CMD_AND;
        }
        else if (buf[j] == ';') /* found end of this command */
        {
                buf[j] = 0; /* replace semicolon with null */
                info->cmd_buf_type = CMD_CHAIN;
        }
        else
                return (0);
        *p = j;
        return (1);
}

void check_command_chain(info_t *info, char *buf, size_t *p, size_t i, size_t len)
{
        size_t j = *p;

        if (info->cmd_buf_type == CMD_AND)
        {
                if (info->status)
                {
                        buf[i] = 0;
                        j = len;
                }
        }
        if (info->cmd_buf_type == CMD_OR)
        {
                if (!info->status)
                {
                        buf[i] = 0;
                        j = len;
                }
        }

        *p = j;
}

int replaceAliases(info_t *info)
{
        int i;
        list_t *node;
        char *p;

        for (i = 0; i < 10; i++)
        {
                node = find_matching_node(info->alias, info->argv[0], '=');
                if (!node)
                        return (0);
                free(info->argv[0]);
                p = findCharInString(node->str, '=');
                if (!p)
                        return (0);
                p = duplicateString(p + 1);
                if (!p)
                        return (0);
                info->argv[0] = p;
        }
        return (1);
}

int replaceVariables(info_t *info)
{
        int i = 0;
        list_t *node;

        for (i = 0; info->argv[i]; i++)
        {
                if (info->argv[i][0] != '$' || !info->argv[i][1])
                        continue;

                if (!str_compare(info->argv[i], "$?"))
                {
                        replaceString(&(info->argv[i]),
                                duplicateString(ConvertNumber(info->status, 10, 0)));
                        continue;
                }
                if (!str_compare(info->argv[i], "$$"))
                {
                        replaceString(&(info->argv[i]),
                                duplicateString(ConvertNumber(getpid(), 10, 0)));
                        continue;
                }
                node = find_matching_node(info->env, &info->argv[i][1], '=');
                if (node)
                {
                        replaceString(&(info->argv[i]),
                                duplicateString(findCharInString(node->str, '=') + 1));
                        continue;
                }
                replaceString(&info->argv[i], duplicateString(""));

        }
        return (0);
}

int replaceString(char **old, char *new)
{
        free(*old);
        *old = new;
        return (1);
}
