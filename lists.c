#include "shell.h"

list_t *include_node(list_t **head, const char *str, int num)
{
        list_t *new_head;

        if (!head)
                return (NULL);
        new_head = malloc(sizeof(list_t));
        if (!new_head)
                return (NULL);
        _custom_set((void *)new_head, 0, sizeof(list_t));
        new_head->num = num;
        if (str)
        {
                new_head->str = duplicateString(str);
                if (!new_head->str)
                {
                        free(new_head);
                        return (NULL);
                }
        }
        new_head->next = *head;
        *head = new_head;
        return (new_head);
}

list_t *add_node_to_end(list_t **head, const char *str, int num)
{
        list_t *new_node, *node;

        if (!head)
                return (NULL);

        node = *head;
        new_node = malloc(sizeof(list_t));
        if (!new_node)
                return (NULL);
        _custom_set((void *)new_node, 0, sizeof(list_t));
        new_node->num = num;
        if (str)
        {
                new_node->str = duplicateString(str);
                if (!new_node->str)
                {
                        free(new_node);
                        return (NULL);
                }
        }
        if (node)
        {
                while (node->next)
                        node = node->next;
                node->next = new_node;
        }
        else
                *head = new_node;
        return (new_node);
}

size_t display_list_content(const list_t *h)
{
        size_t i = 0;

        while (h)
        {
                printString(h->str ? h->str : "(nil)");
                printString("\n");
                h = h->next;
                i++;
        }
        return (i);
}

int remove_node_at_index(list_t **head, unsigned int index)
{
        list_t *node, *prev_node;
        unsigned int i = 0;

        if (!head || !*head)
                return (0);

        if (!index)
        {
                node = *head;
                *head = (*head)->next;
                free(node->str);
                free(node);
                return (1);
        }
        node = *head;
        while (node)
        {
                if (i == index)
                {
                        prev_node->next = node->next;
                        free(node->str);
                        free(node);
                        return (1);
                }
                i++;
                prev_node = node;
                node = node->next;
        }
        return (0);
}

void clear_list(list_t **head_ptr)
{
        list_t *node, *next_node, *head;

        if (!head_ptr || !*head_ptr)
                return;
        head = *head_ptr;
        node = head;
        while (node)
        {
                next_node = node->next;
                free(node->str);
                free(node);
                node = next_node;
        }
        *head_ptr = NULL;
}
