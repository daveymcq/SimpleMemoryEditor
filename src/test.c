#include <stdio.h>
#include <Windows.h>

#include "lib/mcqlibc.h"

int main(int argc, char **argv)
{
    double a, b, c, d, e;

    a = b = c = d = e = (argv[1]) ? (double)StringToDouble(argv[1]) : -1000.0;

    printf("Data Size: %ld\n", (long)sizeof(a));
    printf("PID: %ld\n", GetCurrentProcessId());

    printf("%p: %llf\n", &a, a);
    printf("%p: %llf\n", &b, b);
    printf("%p: %llf\n", &c, c);
    printf("%p: %llf\n", &d, d);
    printf("%p: %llf\n", &e, e);

    getchar();

    b = d = 2010;

    printf("%p: %llf\n", &a, a);
    printf("%p: %llf\n", &b, b);
    printf("%p: %llf\n", &c, c);
    printf("%p: %llf\n", &d, d);
    printf("%p: %llf\n", &e, e);

    getchar();

    b = d = 20144;

    printf("%p: %llf\n", &a, a);
    printf("%p: %llf\n", &b, b);
    printf("%p: %llf\n", &c, c);
    printf("%p: %llf\n", &d, d);
    printf("%p: %llf\n", &e, e);

    getchar();
    b = d = 20777;

    printf("%p: %llf\n", &a, a);
    printf("%p: %llf\n", &b, b);
    printf("%p: %llf\n", &c, c);
    printf("%p: %llf\n", &d, d);
    printf("%p: %llf\n", &e, e);

    getchar();

    return 0;
}
