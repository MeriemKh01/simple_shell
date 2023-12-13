#include "shell.h"

char *obtain_history_file(info_t *info)
{
        char *buf, *dir;

        dir = findEnvironmentVariable(info, "HOME=");
        if (!dir)
                return (NULL);
        buf = malloc(sizeof(char) * (str_length(dir) + str_length(HIST_FILE) + 2));
        if (!buf)
                return (NULL);
        buf[0] = 0;
        copyString(buf, dir, str_length(dir));
        str_concatenate(buf, "/");
        str_concatenate(buf, HIST_FILE);
        return (buf);
}

int record_history(info_t *info)
{
        ssize_t fd;
        char *filename = obtain_history_file(info);
        list_t *node = NULL;

        if (!filename)
                return (-1);

        fd = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0644);
        free(filename);
        if (fd == -1)
                return (-1);
        for (node = info->history; node; node = node->next)
        {
                _output_string_to_fd(node->str, fd);
                _output_to_fd('\n', fd);
        }
        _output_to_fd(BUF_FLUSH, fd);
        close(fd);
        return (1);
}

int retrieve_history(info_t *info)
{
        int i, last = 0, linecount = 0;
        ssize_t fd, rdlen, fsize = 0;
        struct stat st;
        char *buf = NULL, *filename = obtain_history_file(info);

        if (!filename)
                return (0);

        fd = open(filename, O_RDONLY);
        free(filename);
        if (fd == -1)
                return (0);
        if (!fstat(fd, &st))
                fsize = st.st_size;
        if (fsize < 2)
                return (0);
        buf = malloc(sizeof(char) * (fsize + 1));
        if (!buf)
                return (0);
        rdlen = read(fd, buf, fsize);
        buf[fsize] = 0;
        if (rdlen <= 0)
                return (free(buf), 0);
        close(fd);
        for (i = 0; i < fsize; i++)
                if (buf[i] == '\n')
                {
                        buf[i] = 0;
                        create_history_list(info, buf + last, linecount++);
                        last = i + 1;
                }
        if (last != i)
                create_history_list(info, buf + last, linecount++);
        free(buf);
        info->histcount = linecount;
        while (info->histcount-- >= HIST_MAX)
                remove_node_at_index(&(info->history), 0);
        reorder_history(info);
        return (info->histcount);
}

int create_history_list(info_t *info, char *buf, int linecount)
{
        list_t *node = NULL;

        if (info->history)
                node = info->history;
        add_node_to_end(&node, buf, linecount);

        if (!info->history)
                info->history = node;
        return (0);
}

int reorder_history(info_t *info)
{
        list_t *node = info->history;
        int i = 0;

        while (node)
        {
                node->num = i++;
                node = node->next;
        }
        return (info->histcount = i);
}
