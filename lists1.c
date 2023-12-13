#include "shell.h"

size_t determine_list_length(const list_t *h)
{
        size_t i = 0;

        while (h)
        {
                h = h->next;
                i++;
        }
        return (i);
}

char **convert_list_to_strings(list_t *head)
{
        list_t *node = head;
        size_t i = determine_list_length(head), j;
        char **strs;
        char *str;

        if (!head || !i)
                return (NULL);
        strs = malloc(sizeof(char *) * (i + 1));
        if (!strs)
                return (NULL);
        for (i = 0; node; node = node->next, i++)
        {
                str = malloc(str_length(node->str) + 1);
                if (!str)
                {
                        for (j = 0; j < i; j++)
                                free(strs[j]);
                        free(strs);
                        return (NULL);
                }

                str = copyString(str, node->str, str_length(node->str));
                strs[i] = str;
        }
        strs[i] = NULL;
        return (strs);
}

size_t display_list_elements(const list_t *h)
{
        size_t i = 0;

        while (h)
        {
                printString(ConvertNumber(h->num, 10, 0));
                putCharacter(':');
                putCharacter(' ');
                printString(h->str ? h->str : "(nil)");
                printString("\n");
                h = h->next;
                i++;
        }
        return (i);
}

list_t *find_matching_node(list_t *node, char *prefix, char c)
{
        char *p = NULL;

        while (node)
        {
                p = start_prefix(node->str, prefix);
                if (p && ((c == -1) || (*p == c)))
                        return (node);
                node = node->next;
        }
        return (NULL);
}

ssize_t retrieve_node_index(list_t *head, list_t *node)
{
        size_t i = 0;

        while (head)
        {
                if (head == node)
                        return (i);
                head = head->next;
                i++;
        }
        return (-1);
}
