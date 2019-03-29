#include <stdlib.h>
#include "allocate.h"

double f(double x, double y)
{
    return x * x + y * y;
}


void input(int *nx, int *ny, double *x, double *y)
{
    printf("Input NX: ");
    while (scanf("%d", nx) != 1 || *nx < 0)
    {
        printf("NX must be int(nx >= 0)! Try again: ");
        fflush(stdin);
    }
    printf("Input NY: ");
    while (scanf("%d", ny) != 1 || *ny < 0)
    {
        printf("NY must be int(ny >= 0)! Try again: ");
        fflush(stdin);
    }
    printf("Input X: ");
    while (scanf("%lf", x) != 1)
    {
        printf("X must be double! Try again: ");
        fflush(stdin);
    }
    printf("Input Y: ");
    while (scanf("%lf", y) != 1)
    {
        printf("Y must be double! Try again: ");
        fflush(stdin);
    }
}

















void info(void)
{
    printf("<app.exe input.txt>\n");
}

int read_file(FILE *f, table_t *data)
{
    int rc = OK;
    double **buf = NULL;
    double *x = NULL;
    double *y = NULL;
    double tmp_x;
    double tmp_y;
    double tmp_z;
    int n, m;

    if (fscanf(f, "%d %d", &n, &m) == 2 && n > 0 && m > 0)
    {
        buf = allocate(n, m);
        x = calloc(n, sizeof(double));
        y = calloc(m, sizeof(double));
        if (buf && x && y)
        {
            for (int i = 0; i < n && rc == OK; i++)
            {
                for (int j = 0; j < m && rc == OK; j++)
                {
                    if (fscanf(f, "%lf %lf %lf", &tmp_x, &tmp_y, &tmp_z) == 3)
                    {
                        if (i == 0)
                            y[j] = tmp_y;
                        x[i] = tmp_x;
                        buf[i][j] = tmp_z;
                    }
                    else
                        rc = ERR_IO;
                }
            }
            if (rc == OK)
            {
                data->x = x;
                data->y = y;
                data->z = buf;
                data->xn = n;
                data->xy = m;
            }
            else
            {
                free(x);
                free(y);
                free_matrix(buf, n);
            }
        }
        else
            rc = ERR_MEM;
    }
    else
        rc = ERR_IO;
    return rc;
}

void print_data(table_t data)
{
    printf("%9s ", "x\\y| ");
    for (int i = 0; i < data.xy; i++)
        printf("%7.3lf ", (data.y)[i]);
    printf("\n");
    for (int i = 0; i < data.xy * 3; i++)
        printf(" - ");
    printf("\n");
    for (int i = 0; i < data.xn; i++)
    {
        for (int j = 0; j < data.xy; j++)
        {
            if (j == 0)
                printf("%7.3lf| ", (data.x)[i]);
            printf("%7.3lf ", (data.z)[i][j]);
        }
        printf("\n");
    }
}


double **allocate(int n, int m)
{
    double **matrix = malloc((n + 1) * sizeof(double *));
    if (matrix)
    {
        double *data = calloc(n * m, sizeof(double));
        if (data)
        {
            for (int i = 0; i < n; i++)
                matrix[i] = data + i * m;
            matrix[n] = data;
        }
        else
        {
            free(matrix);
            return NULL;
        }
    }
    return matrix;
}

void free_matrix(double **mtr, int n)
{
    free(mtr[n]);
    free(mtr);
}
