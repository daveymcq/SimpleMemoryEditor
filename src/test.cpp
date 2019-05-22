#include <stdio.h>
#include <Windows.h>

int main(int argc, char **argv)
{
    double a, b, c, d, e;

    a = b = c = d = e = (argv[1]) ? (double)atof(argv[1]) : -1000.0;

    printf("Data Size: %ld\n", (long)sizeof(a));
    printf("PID: %ld\n", GetCurrentProcessId());

    printf("%p: %f\n", &a, a);
    printf("%p: %f\n", &b, b);
    printf("%p: %f\n", &c, c);
    printf("%p: %f\n", &d, d);
    printf("%p: %f\n", &e, e);

    getchar();

    b = d = 2010.1235;

    printf("%p: %f\n", &a, a);
    printf("%p: %f\n", &b, b);
    printf("%p: %f\n", &c, c);
    printf("%p: %f\n", &d, d);
    printf("%p: %f\n", &e, e);

    getchar();

    b = d = 20144.1235;

    printf("%p: %f\n", &a, a);
    printf("%p: %f\n", &b, b);
    printf("%p: %f\n", &c, c);
    printf("%p: %f\n", &d, d);
    printf("%p: %f\n", &e, e);

    getchar();
    b = d = 20777.1235;

    printf("%p: %f\n", &a, a);
    printf("%p: %f\n", &b, b);
    printf("%p: %f\n", &c, c);
    printf("%p: %f\n", &d, d);
    printf("%p: %f\n", &e, e);

    getchar();

    return 0;
}
