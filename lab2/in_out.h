#ifndef IN_OUT_H
#define IN_OUT_H

double f(double x);
double fcos(double x);
void sort(double *xarr, double *yarr, int n);
void create_table(double from, double to, double step, double *xarr, double *yarr, int *n, double (*func)(double));
void print_table(double *x, double *y, int n);
void input( double *x);
void print_array(double *array, int n);
int count_points(void);
void read_from_file(double *xarr, double *yarr, int n);

#endif // IN_OUT_H
