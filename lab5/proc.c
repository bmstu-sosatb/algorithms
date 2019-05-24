#include <stdio.h>
#include <math.h>
#include "defines.h"
#include "proc.h"


void create_matrix(double **matrix, double *right_part, double *conc, double *K_zv, int *zarr, int n)
{
    for (int i = 0; i < n - 2; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (j == 0)
                matrix[i][j] = 1;
            if (j == i + 1)
                matrix[i][j] = -1;
            if (j == i + 2)
                matrix[i][j] = 1;
        }
        right_part[i] = log(K_zv[i]) - conc[0] - conc[i + 2] + conc[i + 1];
    }
    for (int j = 0; j < n; j++)
        matrix[n - 1][j] = exp(conc[j]);
    matrix[n - 2][0] = -matrix[n - 1][0];
    matrix[n - 2][1] = 0;
    right_part[n - 2] = matrix[n - 1][0];
    for (int j = 2; j < n; j++)
    {
        matrix[n - 2][j] = matrix[n - 1][j] * zarr[j - 1];
        right_part[n - 2] -= matrix[n - 2][j];
    }
}

void gauss(double **matrix, double *right_part, int n)
{
    double d;
    int flag = 0;
    for (int i = 0; i < n - 1; i++)
    {
        for (int icur = i + 1; icur < n; icur++)
        {
            if (matrix[i][i] != 0)
            {
                d = - matrix[icur][i] / matrix[i][i];
                for (int j = i; j < n; j++)
                    matrix[icur][j] += d * matrix[i][j];
                right_part[icur] += d * right_part[i];
            }
        }
    }
    flag = 0;
    for (int i = 0; i < n; i++)
    {
        if (matrix[i][i] != 0)
        {
            right_part[i] /= matrix[i][i];
            for (int j = n - 1; j > i - 1; j--)
                matrix[i][j] /= matrix[i][i];
        }
        else
            flag = 1;
    }
    if (flag)
        printf('Zero division error');
}

double dihotomiya(double from, double to)
{
    double mid = (from + to) / 2;
    while (mid > EPS)
    {

    }
}
