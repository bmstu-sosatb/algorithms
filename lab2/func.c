#include <stdio.h>
#include "in_out.h"
#include "func.h"
#include "errors.h"

// проверка экстраполяции
int check_in_table(double x, double *xarr, int n)
{
    if (x < xarr[0] || x > xarr[n - 1])
        return EXTRP;
    else return NOTEXTRP;
}


// функции заполнения массивов

void create_h(int n, double *harr, double *xarr)
{
    for (int i = 1; i < n; i++)
    {
        harr[i] = xarr[i] - xarr[i - 1];
    }
}

void create_A(int n, double *Aarr, double *harr)
{
    for (int i = 2; i < n; i++)
    {
        Aarr[i] = harr[i - 1];
    }
}

void create_B(int n, double *Barr, double *harr)
{
    for (int i = 2; i < n; i++)
    {
        Barr[i] = -2 * (harr[i - 1] + harr[i]);
    }
}

void create_D(int n, double *Darr, double *harr)
{
    for (int i = 2; i < n; i++)
    {
        Darr[i] = harr[i];
    }
}

void create_F(int n, double *Farr, double *harr, double *yarr)
{
    for (int i = 2; i < n; i++)
    {
        Farr[i] = -3 * ((yarr[i] - yarr[i - 1]) / harr[i] - (yarr[i - 1] - yarr[i - 2]) / harr[i - 1]) ;
    }
}

// нахождение места х в таблице
int find_xplace(int x, double *xarr, int n)
{
    for (int i = 0; i < n - 1; i++)
    {
        if (xarr[i] <= x && x < xarr[i + 1])
        {
            return i + 1;
        }
    }
    return n - 1;
}


int process(int n, double *xarr, double *yarr, double x)
{
    double harr[n];
    double Aarr[n];
    double Barr[n];
    double Darr[n];
    double Farr[n];
    double eta[n + 1];
    double ksi[n + 1];
    double U[n + 1];
    double a[n], d[n], b[n];
    U[n] = 0;
    U[0] = 0;
    ksi[2] = 0;
    eta[2] = 0;
    create_h(n, harr, xarr);
    create_A(n, Aarr, harr);
    create_B(n, Barr, harr);
    create_D(n, Darr, harr);
    create_F(n, Farr, harr, yarr);
    //print_array(harr, n);
    //print_array(Aarr, n);
    //print_array(Barr, n);
    //print_array(Darr, n);
    //print_array(Farr, n);

    ksi[3] = Darr[2] / Barr[2];
    eta[3] = Farr[2] / Barr[2];

    for (int i = 3; i < n; i++)
    {
        ksi[i + 1] = Darr[i] / (Barr[i] - Aarr[i] * ksi[i]);
    }

    for (int i = 3; i < n; i++)
    {
        eta[i + 1] = (Aarr[i] * eta[i] + Farr[i]) / (Barr[i] - Aarr[i] * ksi[i]);
    }

    for (int i = n - 1; i > 0; i--)
    {
        U[i] = ksi[i + 1] * U[i + 1] + eta[i + 1];
    }
//    printf("ksi and eta\n");
//    print_array(ksi, n);
//    print_array(eta, n);

    for (int i = 1; i < n; i++)
    {
        a[i] = yarr[i - 1];
    }

    for (int i = 1; i < n; i++)
    {
        d[i] = (U[i + 1] - U[i]) / (3 * harr[i]);
    }

    for (int i = 1; i < n; i++)
    {
        b[i] = (yarr[i] - yarr[i - 1])/ harr[i] - (harr[i] * (U[i + 1] + 2 * U[i])) / 3;
    }
//    printf("a[i], d[i], b[i], c[i]\n");
//    print_array(a, n);
//    print_array(d, n);
//    print_array(b, n);
//    print_array(U, n);
    int i = find_xplace(x, xarr, n);
    //printf("pos is %d\n", i );
    double x_xi = (x - xarr[i - 1]);
    double result = a[i] + b[i] * x_xi + U[i] * x_xi * x_xi + d[i] * x_xi * x_xi * x_xi;
    printf("\na = %f, b = %f, c = %f, d = %f\n", a[i], b[i], U[i], d[i]);
    printf("\nWith spline: y(%f) = %f\n", x, result);
    printf("Exact meaning: y(%f) = %f\n", x, f(x));
    return result;

}
