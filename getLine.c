#include "shell.h"

ssize_t bufferInput(info_t *info, char **buf, size_t *len)
{
        ssize_t r = 0;
        size_t len_p = 0;

        if (!*len) /* if nothing left in the buffer, fill it */
        {
                /*bfree((void **)info->cmd_buf);*/
                free(*buf);
                *buf = NULL;
                signal(SIGINT, interruptHandler);
#if USE_GETLINE
                r = getline(buf, &len_p, stdin);
#else
                r = customGetline(info, buf, &len_p);
#endif
                if (r > 0)
                {
                        if ((*buf)[r - 1] == '\n')
                        {
                                (*buf)[r - 1] = '\0'; /* remove trailing newline */
                                r--;
                        }
                        info->linecount_flag = 1;
                        RemoveComments(*buf);
                        create_history_list(info, *buf, info->histcount++);
                        /* if (findCharInString(*buf, ';')) is this a command chain? */
                        {
                                *len = r;
                                info->cmd_buf = buf;
                        }
                }
        }
        return (r);
}

ssize_t getInput(info_t *info)
{
        static char *buf; /* the ';' command chain buffer */
        static size_t i, j, len;
        ssize_t r = 0;
        char **buf_p = &(info->arg), *p;

        putCharacter(BUF_FLUSH);
        r = bufferInput(info, &buf, &len);
        if (r == -1) /* EOF */
                return (-1);
        if (len)        /* we have commands left in the chain buffer */
        {
                j = i; /* init new iterator to current buf position */
                p = buf + i; /* get pointer for return */

                check_command_chain(info, buf, &j, i, len);
                while (j < len) /* iterate to semicolon or end */
                {
                        if (is_command_chain(info, buf, &j))
                                break;
                        j++;
                }
                
                if (i == j)
                    {
                        i = j = len = 0; /* reset position and length */
                        info->cmd_buf_type = CMD_NORM;
                        return -1; /* Indicate empty line */
                    }

                i = j + 1; /* increment past nulled ';'' */
                if (i >= len) /* reached end of buffer? */
                {
                        i = len = 0; /* reset position and length */
                        info->cmd_buf_type = CMD_NORM;
                }

                *buf_p = p; /* pass back pointer to current command position */
                return (str_length(p)); /* return length of current command */
        }

        *buf_p = buf; /* else not a chain, pass back buffer from customGetline() */
        return (r); /* return length of buffer from customGetline() */
}

ssize_t readBuffer(info_t *info, char *buf, size_t *i)
{
        ssize_t r = 0;

        if (*i)
                return (0);
        r = read(info->readfd, buf, readBuffer_SIZE);
        if (r >= 0)
                *i = r;
        return (r);
}

int customGetline(info_t *info, char **ptr, size_t *length)
{
        static char buf[readBuffer_SIZE];
        static size_t i, len;
        size_t k;
        ssize_t r = 0, s = 0;
        char *p = NULL, *new_p = NULL, *c;

        p = *ptr;
        if (p && length)
                s = *length;
        if (i == len)
                i = len = 0;

        r = readBuffer(info, buf, &len);
        if (r == -1 || (r == 0 && len == 0))
                return (-1);

        c = findCharInString(buf + i, '\n');
        k = c ? 1 + (unsigned int)(c - buf) : len;
        new_p = custom_realloc(p, s, s ? s + k : k + 1);
        if (!new_p) /* MALLOC FAILURE! */
                return (p ? free(p), -1 : -1);

        if (s)
                concatenateString(new_p, buf + i, k - i);
        else
                copy_String(new_p, buf + i, k - i + 1);

        s += k - i;
        i = k;
        p = new_p;

        if (length)
                *length = s;
        *ptr = p;
        return (s);
}

void interruptHandler(__attribute__((unused))int sig_num)
{
        printString("\n");
        printString("$ ");
        putCharacter(BUF_FLUSH);
}
