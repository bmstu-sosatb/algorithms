#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include "in_out.h"
#include "errors.h"


double f(double x)
{
    return x * x;
    //return cos((M_PI/2)*x);
}

double fcos(double x)
{
    return cos(x) - x;
}

void sort(double *xarr, double *yarr, int n)
{
    for (int i = 0; i < n; i++)
    {
        int flag = 0;
        for (int j = 0; j < n - 1 - i; j++)
        {
            if (xarr[j] > xarr[j + 1])
            {
                flag = 1;
                double tmp = xarr[j];
                xarr[j] = xarr[j + 1];
                xarr[j + 1] = tmp;
                tmp = yarr[j];
                yarr[j] = yarr[j + 1];
                yarr[j + 1] = tmp;
            }
        }
        if (!flag)
            break;
    }
}

// создает таблицу с начального значения from до значения to, с шагом step, помещает в массивы xarr и yarr
void create_table(double from, double to, double step, double *xarr, double *yarr, int *n, double (*func)(double))
{
    int i = 0;
    while (from <= to)
    {
        xarr[i] = from;
        yarr[i] = func(xarr[i]);
        from += step;
        i++;
    }
    *n = i;
}

// печать таблицы
void print_table(double *x, double *y, int n)
{
    printf("%-10s|%-10s\n", "x", "y(x)");
    for (int i = 0; i < 21; i++)
        printf("-");
    printf("\n");
    for (int i = 0; i < n; i++)
        printf("%-10.3lf|%-10.3lf\n", x[i], y[i]);
}

// ввод степени и значения х
void input( double *x)
{
    printf("Input X: ");
    while (scanf("%lf", x) != 1)
    {
        printf("X must be double! Try again: ");
        fflush(stdin);
    }
}

// печать массива
void print_array(double *array, int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("%f ", array[i]);
    }
    printf("\n");
}


// для чтения из файла in.txt (без ошибок)

int count_points(void)
{
    FILE *f;
    double x, y;
    int n = 0;
    f = fopen("in.txt", "r");
    while (fscanf(f, "%lf%lf", &x, &y) == 2)
        n++;
    fclose(f);
    return n;
}
void read_from_file(double *xarr, double *yarr, int n)
{
    FILE *f;
    f = fopen("in.txt", "r");
    for (int i = 0; i < n; i++)
    {
        fscanf(f, "%lf%lf", xarr + i, yarr + i);
    }
    fclose(f);
}
