#ifndef DEFINES_H
#define DEFINES_H

#define OK 0
#define ERR_PARAM -1
#define ERR_IO -2
#define ERR_MEM -3
#define ERR_FILE -4
#define LACK_OF_DATA -5
#define DIV0 -6


#define EXTRP  1
#define FOUND 10


#include <math.h>
#define EPS 1e-10
#define IS_EQUAL(x, y) (fabs((x) - (y)) <= EPS)


typedef struct table
{
    double **z;
    double *x;
    double *y;
    int xn;
    int xy;
} table_t;


#endif // DEFINES_H
