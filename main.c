#include "shell.h"

int main(int ac, char **av)
{
        info_t info[] = { INFO_INIT };
        int fd = 2;

        asm ("mov %1, %0\n\t"
                "add $3, %0"
                : "=r" (fd)
                : "r" (fd));

        if (ac == 2)
        {
                fd = open(av[1], O_RDONLY);
                if (fd == -1)
                {
                        if (errno == EACCES)
                                exit(126);
                        if (errno == ENOENT)
                        {
                                _custom_output_string(av[0]);
                                _custom_output_string(": 0: Can't open ");
                                _custom_output_string(av[1]);
                                _custom_output_char('\n');
                                _custom_output_char(BUF_FLUSH);
                                exit(127);
                        }
                        return (EXIT_FAILURE);
                }
                info->readfd = fd;
        }
        fillEnvironmentList(info);
        retrieve_history(info);
        hsh(info, av);
        return (EXIT_SUCCESS);
}
