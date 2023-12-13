#include "shell.h"

int strToInt(char *s)
{
        int i = 0;
        unsigned long int result = 0;

        if (*s == '+')
                s++;  /* TODO: why does this make main return 255? */
        for (i = 0;  s[i] != '\0'; i++)
        {
                if (s[i] >= '0' && s[i] <= '9')
                {
                        result *= 10;
                        result += (s[i] - '0');
                        if (result > INT_MAX)
                                return (-1);
                }
                else
                        return (-1);
        }
        return (result);
}

void PrintError(info_t *info, char *estr)
{
        _custom_output_string(info->fname);
        _custom_output_string(": ");
        PrintDecimal(info->line_count, STDERR_FILENO);
        _custom_output_string(": ");
        _custom_output_string(info->argv[0]);
        _custom_output_string(": ");
        _custom_output_string(estr);
}

int PrintDecimal(int input, int fd)
{
        int (*_putCharacter)(char) = putCharacter;
        int i, count = 0;
        unsigned int _abs_, current;

        if (fd == STDERR_FILENO)
                _putCharacter = _custom_output_char;
        if (input < 0)
        {
                _abs_ = -input;
                _putCharacter('-');
                count++;
        }
        else
                _abs_ = input;
        current = _abs_;
        for (i = 1000000000; i > 1; i /= 10)
        {
                if (_abs_ / i)
                {
                        _putCharacter('0' + current / i);
                        count++;
                }
                current %= i;
        }
        _putCharacter('0' + current);
        count++;

        return (count);
}

char *ConvertNumber(long int num, int base, int flags)
{
        static char *array;
        static char buffer[50];
        char sign = 0;
        char *ptr;
        unsigned long n = num;

        if (!(flags & CONVERT_UNSIGNED) && num < 0)
        {
                n = -num;
                sign = '-';

        }
        array = flags & CONVERT_LOWERCASE ? "0123456789abcdef" : "0123456789ABCDEF";
        ptr = &buffer[49];
        *ptr = '\0';

        do      {
                *--ptr = array[n % base];
                n /= base;
        } while (n != 0);

        if (sign)
                *--ptr = sign;
        return (ptr);
}

void RemoveComments(char *buf)
{
        int i;

        for (i = 0; buf[i] != '\0'; i++)
                if (buf[i] == '#' && (!i || buf[i - 1] == ' '))
                {
                        buf[i] = '\0';
                        break;
                }
}
