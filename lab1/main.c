#include <stdio.h>
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>
#define EXTRP 1
#define NOTEXTRP 0
#define OK 0
#define ERR_MEM 1

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

// создает таблицу с начального значения from, с шагом step, кол-во значений n, помещает в массивы xarr и yarr
void create_table(double from, double step, int n, double *xarr, double *yarr, double (*func)(double))
{
    for (int i = 0; i < n; i++)
    {
        xarr[i] = from + step * i;
        yarr[i] = func(xarr[i]);
    }
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
void input(int *power, double *x)
{
    printf("Input the power of polynom: ");
    while (scanf("%d", power) != 1 || *power < 0)
    {
        printf("Power must be int(power >= 0)! Try again: ");
        fflush(stdin);
    }
    printf("Input X: ");
    while (scanf("%lf", x) != 1)
    {
        printf("X must be double! Try again: ");
        fflush(stdin);
    }
}

// выбор значений из таблицы xarr,yarr,n  в массивы xchosen, ychosen для степени power и значения х
int choose(double *xarr, double *yarr, int n, double x, int power, double *xchosen, double *ychosen)
{  
    if (x < xarr[0])
    {
        for (int i = 0; i <= power; i++)
        {
            xchosen[i] = xarr[i];
            ychosen[i] = yarr[i];
        }
        return EXTRP;
    }
    if (x > xarr[n - 1])
    {
        for (int i = power; i >= 0; i--)
        {
            xchosen[i] = xarr[n - (power - i) - 1];
            ychosen[i] = yarr[n - (power - i) - 1];
        }
        return EXTRP;
    }
    int place;
    for (int i = 0; i < n - 1; i++)
        if (xarr[i] <= x && x <= xarr[i + 1])
            place = i;
    int from, to;
    if (power % 2 == 0)
    {
        from = place - power / 2;
        to = place + power / 2;
    }
    else
    {
        from = place - power / 2;
        to = place + power / 2 + 1;
    }
    if (from < 0)
    {
        to -= from;
        from = 0;
    }
    if (to > n - 1)
    {
        from -= (to - (n - 1));
        to = n - 1;
    }
    int k = 0;
    for (int i = from; i <= to; i++, k++)
    {
        xchosen[k] = xarr[i];
        ychosen[k] = yarr[i];
    }
    return NOTEXTRP;
}

// создает таблицу разделенных разностей
int razn_count(double **razn_table, int power, double *xchosen)
{
    double *buf = NULL;
    for (int i = 1; i <= power; i++)
    {
        int n = power - i + 1;
        buf = malloc(n * sizeof(double));
        if (!buf)
            return ERR_MEM;
        razn_table[i] = buf;
        buf = NULL;
        double d = xchosen[0] - xchosen[i];
        for (int j = 0; j < n; j++)
        {  
            razn_table[i][j] = (razn_table[i - 1][j] - razn_table[i - 1][j + 1]) / d;
           // printf("%lf\n", razn_table[i][j]);      //
        }
    }
    return OK;
}

// печать коэффициентов
void print_koef(double **razn_table, int power)
{
    printf("Razdelennye raznosti:\n");
    for (int i = 0; i <= power; i++)
    {
        for (int j = 0; j <= power - i; j++)
            printf("%lf ", razn_table[i][j]);
        printf("\n");
    }
}

// считается полином
double count_result(double **razn_table, double *xchosen, int power, double x)
{
    double xpart = 1;
    double y = razn_table[0][0];
    for (int i = 1; i <= power; i++)
    {
        xpart *= (x - xchosen[i - 1]);
        y += (xpart * razn_table[i][0]);
    }
    return y;
}

int main(void)
{
    double **razn_table = NULL;
    double *xarr = NULL, *yarr = NULL, *xchosen = NULL, *ychosen = NULL;
    int rc = OK;
    int n = 6;
    double from = 0, step = 1;
    int power, extr;
    double x;
    (void) extr;
    xarr = malloc(n * sizeof(double));
    yarr = malloc(n * sizeof(double));
    if (!xarr || !yarr)
        rc = ERR_MEM;
    else
    {
        create_table(from, step, n, xarr, yarr, f);
        print_table(xarr, yarr, n);
        input(&power, &x);
        xchosen = malloc((power + 1) * sizeof(double));
        ychosen = malloc((power + 1) * sizeof(double));
        razn_table = malloc ((power + 1) * sizeof(double*));
        if (!xchosen || !ychosen || !(*razn_table))
            rc = ERR_MEM;
        else
        {
            extr = choose(xarr, yarr, n, x, power, xchosen, ychosen);
            printf("Chosen meanings:\n");
            print_table(xchosen, ychosen, power + 1);    //
            razn_table[0] = ychosen;
            rc = razn_count(razn_table, power, xchosen);
            if (rc == OK)
            {
                double yreal = f(x);
                double yres = count_result(razn_table, xchosen, power, x);
                if (extr == EXTRP)
                    printf("Extrapolyatsiya.\n");
                else
                    printf("No extrapolyatsiya.\n");
                printf("Real meaning f(%.3lf) = %lf\n", x, yreal);
                printf("Meaning by polynom y(%.3lf) = %lf\n\n", x, yres);
                print_koef(razn_table, power);           //
            }
        }
    }
    free(xchosen);
    free(ychosen);
    free(xarr);
    free(yarr);
    for (int i = 1; i <= power; i++)
        free(razn_table[i]);
    free(razn_table);
    if (rc == ERR_MEM)
        printf("Memory error!");
    else
    {
        int ncos = 12;
        int powercos = 5;
        double y = 0;
        double **razn_table_cos = malloc((powercos + 1) * sizeof(double*));
        double *xcos = malloc(ncos * sizeof(double));
        double *ycos = malloc(ncos * sizeof(double));
        double *xchcos = malloc((powercos + 1) * sizeof(double));
        double *ychcos = malloc((powercos + 1) * sizeof(double));
        if (ycos && xcos && razn_table_cos && xchcos && ychcos)
        {
            create_table(-1, 0.3, ncos, xcos, ycos, fcos);
            //print_table(xcos, ycos, ncos);   //
            sort(ycos, xcos, ncos);
            //print_table(xcos, ycos, ncos);    //
            choose(ycos, xcos, ncos, y, powercos, ychcos, xchcos);
            //print_table(xchcos, ychcos, powercos + 1);    //
            razn_table_cos[0] = xchcos;
            razn_count(razn_table_cos, powercos, ychcos);
            double notre = count_result(razn_table_cos, ychcos, powercos, y);
            printf("For cosx - x\nKoren': %lf\n", notre);
        }
    }

    return rc;
}
