#ifndef FUNC_H
#define FUNC_H

#include "defines.h"
int find_place(double *arr, int n, double elem, int *left, int *right);
int find_indexes(int *left, int *right, int n, int nchosen);
void place_by_indexes(table_t *data, table_t *chosen, int yleft, int yright, int xleft, int xright);
int choose_points(table_t *data, table_t *chosen, int yleft, int yright, int xleft, int xright);
double find(double *argument, double *value, int size, int degree, double search, int *rc);
double double_interpolation(table_t *chosen, double x, double y, int *rc);
int calculate(table_t *data, table_t *chosen, double x, double y, double *result);


#endif // FUNC_H
