/**
 * @file 13.2.c
 * @author your name (you@domain.com)
 * @brief
 * Write a file-copy program that takes the original filename and the copy file from the
command line. Use standard I/O and the binary mode, if possible.
 * @version 0.1
 * @date 2022-09-17
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[])
{
    int byte;
    FILE *source;
    FILE *target;

    if (argc != 3)
    {
        printf("Usage: %s sourcefile targetfile\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if ((source = fopen(argv[1], "rb")) == NULL)
    {
        printf("Could not open file %s for inpu\n", argv[1]);
        exit(EXIT_FAILURE);
    }
    if ((target = fopen(argv[2], "wb")) == NULL)
    {
        printf("Could not open file %s for inpu\n", argv[2]);
        exit(EXIT_FAILURE);
    }
    while ((byte = getc(source)) != EOF)
    {
        putc(byte, target);
    }
    if (fclose(source) != 0)
        printf("Could not close file %s\n", argv[1]);
    if (fclose(target) != 0)
        printf("Could not close file %s\n", argv[2]);
    return 0;
}
