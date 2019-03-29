#include <stdio.h>
#include <stdlib.h>

#include "errors.h"
#include "func.h"
#include "in_out.h"


int main(void)
{
    double *xarr = NULL, *yarr = NULL;
    int rc = OK;
    double from = 0, to = 5,step = 1;
    int n = (to - from) / step + 1;
    int extr;
    double x;
    (void) extr;
    xarr = malloc(n * sizeof(double));
    yarr = malloc(n * sizeof(double));
    if (!xarr || !yarr)
        rc = ERR_MEM;
    else
    {
        create_table(from, to, step, xarr, yarr, &n, f);
//        n = count_points();
//        read_from_file(xarr, yarr, n);
        sort(xarr, yarr, n);
        print_table(xarr, yarr, n);
        printf("\n");
        input(&x);
        extr = check_in_table(x, xarr, n);
        if (extr)
            printf("EXTRAPOLYATSIYA!\n");
        else
        {
            process(n, xarr, yarr, x);
        }
    }
    free(xarr);
    free(yarr);
    if (rc == ERR_MEM)
        printf("Memory error!");

    return rc;
}
