#ifndef PROC_H
#define PROC_H

void gauss(double **matrix, double *right_part, int n);
void create_matrix(double **matrix, double *right_part, double *conc, double *K_zv, int *zarr, int n);

#endif // PROC_H
