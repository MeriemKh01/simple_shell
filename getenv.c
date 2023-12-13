#include "shell.h"

char **fetchEnvironment(info_t *info)
{
        if (!info->environ || info->env_changed)
        {
                info->environ = convert_list_to_strings(info->env);
                info->env_changed = 0;
        }

        return (info->environ);
}

int removeEnvVariable(info_t *info, char *var)
{
        list_t *node = info->env;
        size_t i = 0;
        char *p;

        if (!node || !var)
                return (0);

        while (node)
        {
                p = start_prefix(node->str, var);
                if (p && *p == '=')
                {
                        info->env_changed = remove_node_at_index(&(info->env), i);
                        i = 0;
                        node = info->env;
                        continue;
                }
                node = node->next;
                i++;
        }
        return (info->env_changed);
}

int setEnvVariable(info_t *info, char *var, char *value)
{
        char *buf = NULL;
        list_t *node;
        char *p;

        if (!var || !value)
                return (0);

        buf = malloc(str_length(var) + str_length(value) + 2);
        if (!buf)
                return (1);
        copyString(buf, var, str_length(var));
        str_concatenate(buf, "=");
        str_concatenate(buf, value);
        node = info->env;
        while (node)
        {
                p = start_prefix(node->str, var);
                if (p && *p == '=')
                {
                        free(node->str);
                        node->str = buf;
                        info->env_changed = 1;
                        return (0);
                }
                node = node->next;
        }
        add_node_to_end(&(info->env), buf, 0);
        free(buf);
        info->env_changed = 1;
        return (0);
}
