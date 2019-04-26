#include <math.h>
#include "defines.h"
#include "func.h"

double fn(double P, double T)
{
    return (NEK * P / T);
}

double fT(double T0, double Tw, int m, double z)
{
    return T0 + (Tw - T0) * pow(z, m);
}

void form_nt_array(double *nt_array, double T0, double Tw, int m, double P)
{
    double step;
    step = 1.0 / N;

    for (int i = 0; i < N + 1; i++)
    {
        double T = fT(T0, Tw, m, step * i);
        nt_array[i] = fn(P, T);
    }
}

double integral(double *nt_array)
{
    double f1, f2;
    double step = 1.0 / N;
    double intg = 0;
    f1 = 0;
    for (int i = 1; i < N + 1; i++)
    {
        f2 = nt_array[i] * i * step;
        intg += (f1 + f2) * step / 2;
        f1 = f2;
    }
    return intg;
}

double count_P(double *nt_array, double T0, double Tw, int m, double P1, double P2)
{
    double P = 1;
    double F, F1;
    while (fabs(P2 - P1) / P >= EPS)
    {
        P =  (P1 + P2) / 2;
        form_nt_array(nt_array, T0, Tw, m, P);
        F = (NEK * PBEG) / TBEG - 2 * integral(nt_array);
        form_nt_array(nt_array, T0, Tw, m, P1);
        F1 = (NEK * PBEG) / TBEG - 2 * integral(nt_array);
        if (F * F1 < 0)
            P2 = P;
        else
            P1 = P;
    }
    return P;
}
