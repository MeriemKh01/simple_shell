#include "shell.h"

int displayHistory(info_t *info)
{
        display_list_elements(info->history);
        return (0);
}

int removeAlias(info_t *info, char *str)
{
        char *p, c;
        int ret;

        p = findCharInString(str, '=');
        if (!p)
                return (1);
        c = *p;
        *p = 0;
        ret = remove_node_at_index(&(info->alias),
                retrieve_node_index(info->alias, find_matching_node(info->alias, str, -1)));
        *p = c;
        return (ret);
}

int addAlias(info_t *info, char *str)
{
        char *p;

        p = findCharInString(str, '=');
        if (!p)
                return (1);
        if (!*++p)
                return (removeAlias(info, str));

        removeAlias(info, str);
        return (add_node_to_end(&(info->alias), str, 0) == NULL);
}

int printAliasString(list_t *node)
{
        char *p = NULL, *a = NULL;

        if (node)
        {
                p = findCharInString(node->str, '=');
                for (a = node->str; a <= p; a++)
                        putCharacter(*a);
                putCharacter('\'');
                printString(p + 1);
                printString("'\n");
                return (0);
        }
        return (1);
}

int manageAlias(info_t *info)
{
        int i = 0;
        char *p = NULL;
        list_t *node = NULL;

        if (info->argc == 1)
        {
                node = info->alias;
                while (node)
                {
                        printAliasString(node);
                        node = node->next;
                }
                return (0);
        }
        for (i = 1; info->argv[i]; i++)
        {
                p = findCharInString(info->argv[i], '=');
                if (p)
                        addAlias(info, info->argv[i]);
                else
                        printAliasString(find_matching_node(info->alias, info->argv[i], '='));
        }

        return (0);
}
