#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define THRESHOLD 150;

int int2bit(int value, int pos)
{
    if (value == 0)
        return 0;
    pos = 7 - pos;
    if (pos == 0)
        return 1;
    return 1 << pos;
}

char *getvalue(FILE *f)
{
    char *a = (char *)malloc(sizeof(char) * 4);
    int c = 0;
    while (1)
    {
        char b = fgetc(f);
        if (b == ' ' || b == '\n' || b == EOF)
        {
            a[c] = '\0';
            break;
        }
        else
        {
            a[c] = b;
        }
        c++;
    }
    return a;
}

int main(int argc, const char **argv)
{
    FILE *file;
    if (fopen_s(&file, argv[1], "r"))
    {
        printf("No in file\n");
        return 1;
    }

    FILE *out;
    if (fopen_s(&out, argv[2], "w+"))
    {
        printf("No out file\n");
        return 1;
    }

    int threshold = argc > 3 ? atoi(argv[3]) : THRESHOLD;

    char ehh[10];
    fgets(ehh, 10, file);
    fgets(ehh, 10, file);
    fgets(ehh, 10, file);

    fprintf(out,
            "#include \"config.h\"\n"
            "#include <avr/pgmspace.h>\n"
            "const unsigned char IMAGE_DATA[] PROGMEM = { \n    ");
    for (int line = 0; line < 298; line++)
    {
        for (int block = 0; block < 16; block++)
        {
            int result = 0;
            for (int pixel = 0; pixel < 8; pixel++)
            {
                char *raw = getvalue(file);
                if (strlen(raw) == 0)
                    raw = getvalue(file);

                int value = atoi(raw) > threshold ? 1 : 0;
                free(raw);
                result |= int2bit(value, pixel);
            }
            fprintf(out, "0X%02X,", result);
        }
        fprintf(out, "\n    ");
    }
    fprintf(out, "\n};\n");

    return 0;
}