#include <stdio.h>
#include <Windows.h>

#include "lib/mcqlibc.h"

int main(int argc, char **argv)
{
    int a, b, c, d, e;

    a = b = c = d = e = (argv[1]) ? (float)StringToDouble(argv[1]) : -1000.0;

    printf("Data Size: %ld\n", (long)sizeof(a));
    printf("PID: %ld\n", GetCurrentProcessId());

    printf("%p: %d\n", &a, a);
    printf("%p: %d\n", &b, b);
    printf("%p: %d\n", &c, c);
    printf("%p: %d\n", &d, d);
    printf("%p: %d\n", &e, e);

    getchar();

    b = d = 2010;

    printf("%p: %d\n", &a, a);
    printf("%p: %d\n", &b, b);
    printf("%p: %d\n", &c, c);
    printf("%p: %d\n", &d, d);
    printf("%p: %d\n", &e, e);

    getchar();

    b = d = 20144;

    printf("%p: %d\n", &a, a);
    printf("%p: %d\n", &b, b);
    printf("%p: %d\n", &c, c);
    printf("%p: %d\n", &d, d);
    printf("%p: %d\n", &e, e);

    getchar();
    b = d = 20777;

    printf("%p: %d\n", &a, a);
    printf("%p: %d\n", &b, b);
    printf("%p: %d\n", &c, c);
    printf("%p: %d\n", &d, d);
    printf("%p: %d\n", &e, e);

    getchar();

    return 0;
}
