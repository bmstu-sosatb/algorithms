#include <stdio.h>
#include "defines.h"
#include "in_out.h"
#include "func.h"

int main(void)
{
    int m, rc = OK;
    double T0, Tw;
    double nt_array[N + 1];
    double P;

    rc = input(&T0, &Tw, &m);
    P = count_P(nt_array, T0, Tw, m, 3, 25);
    printf("\n%lf", P);
    return rc;
}
