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
   /* else
    {
        int ncos = 12;
        int powercos = 5;
        double y = 0;
        double **razn_table_cos = malloc((powercos + 1) * sizeof(double*));
        double *xcos = malloc(ncos * sizeof(double));
        double *ycos = malloc(ncos * sizeof(double));
        double *xchcos = malloc((powercos + 1) * sizeof(double));
        double *ychcos = malloc((powercos + 1) * sizeof(double));
        if (ycos && xcos && razn_table_cos && xchcos && ychcos)
        {
            create_table(-1, 0.3, ncos, xcos, ycos, fcos);
            //print_table(xcos, ycos, ncos);   //
            sort(ycos, xcos, ncos);
            //print_table(xcos, ycos, ncos);    //
            choose(ycos, xcos, ncos, y, powercos, ychcos, xchcos);
            //print_table(xchcos, ychcos, powercos + 1);    //
            razn_table_cos[0] = xchcos;
            razn_count(razn_table_cos, powercos, ychcos);
            double notre = count_result(razn_table_cos, ychcos, powercos, y);
            printf("For cosx - x\nKoren': %lf\n", notre);
        }
    }*/

    return rc;
}
