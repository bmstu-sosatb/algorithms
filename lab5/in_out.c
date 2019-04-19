#include <stdio.h>
#include "defines.h"
#include "in_out.h"

int input(double *T0, double *Tw, int *m)
{
    printf("Input T0: ");
    while (scanf("%lf", T0) != 1 || *T0 < 0)
    {
        printf("T0 must be double! Try again: ");
        fflush(stdin);
    }
    printf("Input Tw: ");
    while (scanf("%lf", Tw) != 1 || *Tw < 0)
    {
        printf("Tw must be double! Try again: ");
        fflush(stdin);
    }
    printf("Input m: ");
    while (scanf("%d", m) != 1)
    {
        printf("m must be int! Try again: ");
        fflush(stdin);
    }
    return OK;
}

