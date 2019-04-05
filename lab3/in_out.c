#include <stdlib.h>
#include <stdio.h>
#include "in_out.h"


double func(double x, double y)
{
    return x * x + y * y;
}

int input(int *nx, int *ny, double *x, double *y, table_t data)
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
    if (*nx > data.nx || *ny > data.ny)
    {
        printf("Lack of data!\n");
        return LACK_OF_DATA;
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
    return OK;
}


void create_file(double fromx, double tox, double stepx, double fromy, double toy, double stepy)
{
    FILE *f = fopen("in.txt", "w");
    int nx = (tox - fromx) / stepx;
    int ny = (toy - fromy) / stepy;
    double y = fromy;
    fprintf(f, "%d %d\n", nx, ny);
    for (int i = 0; i < nx; i++)
    {
        for (int j = 0; j < ny; j++)
        {
            fprintf(f, "%lf %lf %lf\n", fromx, y, func(fromx, y));
            y += stepy;
        }
        y = fromy;
        fromx += stepx;
    }
    fclose(f);
}


// ввод данных из файла
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
        buf = allocate_matrix(n, m);
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
                data->nx = n;
                data->ny = m;
            }
            else
            {
                free(x);
                free(y);
                free_matrix(buf);
            }
        }
        else
            rc = ERR_MEM;
    }
    else
        rc = ERR_IO;
    return rc;
}

// вывод таблицы на экран
void print_table(table_t data)
{
    printf("%9s ", "x\\y| ");
    for (int i = 0; i < data.ny; i++)
        printf("%7.3lf ", (data.y)[i]);
    printf("\n");
    for (int i = 0; i < data.ny * 3; i++)
        printf(" - ");
    printf("\n");
    for (int i = 0; i < data.nx; i++)
    {
        for (int j = 0; j < data.ny; j++)
        {
            if (j == 0)
                printf("%7.3lf| ", (data.x)[i]);
            printf("%7.3lf ", (data.z)[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// выделение памяти под матрицу
double **allocate_matrix(int n, int m)
{
    double **matrix = malloc(n * sizeof(double *));
    if (matrix)
    {
        double *data = calloc(n * m, sizeof(double));
        if (data)
        {
            for (int i = 0; i < n; i++)
                matrix[i] = data + i * m;
        }
        else
        {
            free(matrix);
            return NULL;
        }
    }
    return matrix;
}

void free_matrix(double **matrix)
{
    free(matrix[0]);
    free(matrix);
}
