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
    double conc[] = { -1, 2, -1, -10, -25, -35 };
    double alpha = 0, gamma = 0;
    double K_zv[4];
    int zarr[] = { 0, 1, 2, 3, 4 };

    rc = input(&T0, &Tw, &m);
    P = count_P(nt_array, T0, Tw, m, 3, 25);
    printf("\n%lf", P);
    return rc;
}
