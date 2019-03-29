#ifndef FUNC_H
#define FUNC_H

#include <stdbool.h>
#include <math.h>

int check_in_table(double x, double *xarr, int n);
void create_h(int n, double *harr, double *xarr);
void create_A(int n, double *Aarr, double *harr);
void create_B(int n, double *Barr, double *harr);
void create_D(int n, double *Darr, double *harr);
void create_F(int n, double *Farr, double *harr, double *yarr);
int find_xplace(int x, double *xarr, int n);
int process(int n, double *xarr, double *yarr, double x);


#endif // FUNC_H
