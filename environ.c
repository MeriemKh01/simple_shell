#include "shell.h"

int displayEnvironmentInfo(info_t *info)
{
        display_list_content(info->env);
        return (0);
}

char *findEnvironmentVariable(info_t *info, const char *name)
{
        list_t *node = info->env;
        char *p;

        while (node)
        {
                p = start_prefix(node->str, name);
                if (p && *p)
                        return (p);
                node = node->next;
        }
        return (NULL);
}

int customizeEnvironmentVariable(info_t *info)
{
        if (info->argc != 3)
        {
                _custom_output_string("Incorrect number of arguements\n");
                return (1);
        }
        if (setEnvVariable(info, info->argv[1], info->argv[2]))
                return (0);
        return (1);
}

int removeEnvironmentVariable(info_t *info)
{
        int i;

        if (info->argc == 1)
        {
                _custom_output_string("Too few arguements.\n");
                return (1);
        }
        for (i = 1; i <= info->argc; i++)
                removeEnvVariable(info, info->argv[i]);

        return (0);
}

int fillEnvironmentList(info_t *info)
{
        list_t *node = NULL;
        size_t i;

        for (i = 0; environ[i]; i++)
                add_node_to_end(&node, environ[i], 0);
        info->env = node;
        return (0);
}
