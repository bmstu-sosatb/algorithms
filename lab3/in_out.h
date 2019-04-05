#ifndef IN_OUT_H
#define IN_OUT_H

#include "defines.h"
double func(double x, double y);
int input(int *nx, int *ny, double *x, double *y, table_t data);
void create_file(double fromx, double tox, double stepx, double fromy, double toy, double stepy);
int read_file(FILE *f, table_t *data);
void print_table(table_t data);
double **allocate_matrix(int n, int m);
void free_matrix(double **matrix);

#endif // IN_OUT_H
