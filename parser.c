#include "shell.h"

int check_command(info_t *info, char *path)
{
        struct stat st;

        (void)info;
        if (!path || stat(path, &st))
                return (0);

        if (st.st_mode & S_IFREG)
        {
                return (1);
        }
        return (0);
}

char *copy_characters(char *pathstr, int start, int stop)
{
        static char buf[1024];
        int i = 0, k = 0;

        for (k = 0, i = start; i < stop; i++)
                if (pathstr[i] != ':')
                        buf[k++] = pathstr[i];
        buf[k] = 0;
        return (buf);
}

char *search_in_path(info_t *info, char *pathstr, char *cmd)
{
        int i = 0, curr_pos = 0;
        char *path;

        if (!pathstr)
                return (NULL);
        if ((str_length(cmd) > 2) && start_prefix(cmd, "./"))
        {
                if (check_command(info, cmd))
                        return (cmd);
        }
        while (1)
        {
                if (!pathstr[i] || pathstr[i] == ':')
                {
                        path = copy_characters(pathstr, curr_pos, i);
                        if (!*path)
                                str_concatenate(path, cmd);
                        else
                        {
                                str_concatenate(path, "/");
                                str_concatenate(path, cmd);
                        }
                        if (check_command(info, path))
                                return (path);
                        if (!pathstr[i])
                                break;
                        curr_pos = i;
                }
                i++;
        }
        return (NULL);
}
