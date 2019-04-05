#include <stdlib.h>
#include "func.h"


int find_place(double *arr, int n, double elem, int *left, int *right)
{
    int mid;
    *left = 0, *right = n;
    while (*left <= *right)
    {
        mid = (*left + *right) / 2;
        if (IS_EQUAL(arr[mid], elem))
            return mid; // искомый элемент уже есть в массиве узлов
        if (arr[mid] > elem)
            *right = mid - 1;
        else
            *left = mid + 1;
    }
    if (*left > *right)
    {
        int tmp = *left;
        *left = *right;
        *right = tmp;
    }
    if (*right >= n || *left < 0)
        return EXTRP; // экстраполяция
    return FOUND; // найдены индексы ближайших элементов
}

int find_indexes(int *left, int *right, int n, int nchosen)
{
    int index = 0;
    while ((index < nchosen) && (*left >= 0 || *right < n))
    {
        if (*right < n)
        {
            index++;
            if (index < nchosen)
                (*right)++;
        }
        if (*left >= 0 && index < nchosen)
        {
            index++;
            if (((*right < n && index < nchosen - 1) || (*right >= n && index < nchosen)) && (*left) != 0)
                (*left)--;
        }
    }
    if (*right - *left != nchosen)
        (*right)++;
    if (index != nchosen)
        return LACK_OF_DATA;
    return OK;
}

void place_by_indexes(table_t *data, table_t *chosen, int yleft, int yright, int xleft, int xright)
{
    int ind_i = 0;
    int ind_j = 0;
    for (int i = xleft; i < xright && ind_i < chosen->nx; i++, ind_i++)
        chosen->x[ind_i] = data->x[i];
    for (int i = yleft; i < yright && ind_j < chosen->ny; i++, ind_j++)
        chosen->y[ind_j] = data->y[i];
    ind_i = 0;
    ind_j = 0;
    for (int i = xleft; i < xright && ind_i < chosen->nx; i++, ind_i++)
    {
        ind_j = 0;
        for (int j = yleft; j < yright && ind_j < chosen->ny; j++, ind_j++)
        {
            chosen->z[ind_i][ind_j] = data->z[i][j];
        }
    }
}



int choose_points(table_t *data, table_t *chosen, int yleft, int yright, int xleft, int xright)
{
    int rc = OK;
    //printf("surrounding x: %d %d\n", xleft, xright);
    //printf("surrounding y: %d %d\n", yleft, yright);
    rc = find_indexes(&xleft, &xright, data->nx, chosen->nx);
    if (rc == OK)
    {
        //printf("x indexes: %d - %d\n", xleft, xright);
        rc = find_indexes(&yleft, &yright, data->ny, chosen->ny);
        if (rc == OK)
        {
            //printf("y indexes: %d - %d\n", yleft, yright);
            place_by_indexes(data, chosen, yleft, yright, xleft, xright);
        }
    }
    return rc;
}

// считаем коэффициенты
double find(double *argument, double *value, int size, int degree, double search, int *rc)
{
    // x = search
    *rc = OK;
    double div;
    double res = 0;
    double *coefficients = calloc(degree + 1, sizeof(double));
    if (coefficients)
    {
        coefficients[0] = value[0];
        int n = degree;
        for (int i = 1; i < degree + 1 && *rc == OK; i++)
        {
            for (int j = 0; j < n && *rc == OK; j++)
            {
                div = argument[0] - argument[degree + 1 - n];
                if (fabs(div) <= EPS)
                    *rc = DIV0;
                else
                    value[j] = (value[j] - value[j + 1]) / (div);
            }
            coefficients[i] = value[0];
            n--;
        }
        if (*rc == OK)
        {
            res = coefficients[0];
            int c = 0;
            double mult = 1;
            for (int i = 1; i < degree + 1 && c < size; i++)
            {
                mult *= (search - argument[c]);
                res += mult * coefficients[i];
                c++;
            }
        }
        /*printf("************");
        for (int i = 0; i < degree + 1; i++)
            printf("%.6lf\n", coefficients[i]);
        printf("\n");
        printf("*************");*/
        free(coefficients);
    }
    else
        *rc = ERR_MEM;
    return res;
}

// интерполяция
double double_interpolation(table_t *chosen, double x, double y, int *rc)
{
    *rc = OK;
    double result = 0;
    double *new_z = calloc(chosen->nx, sizeof(double));
    if (new_z)
    {
        for (int i = 0; i < chosen->nx && *rc == OK; i++)
            new_z[i] = find(chosen->y, chosen->z[i], chosen->ny, chosen->ny - 1, y, rc);
        if (*rc == OK)
        {
            result = find(chosen->x, new_z, chosen->nx, chosen->nx - 1, x, rc);
        }
        free(new_z);
    }
    else
        *rc = ERR_MEM;
    return result;
}

// общая функция вычисления
int calculate(table_t *data, table_t *chosen, double x, double y, double *result)
{
    int rc = OK;
    int rc_y = OK;
    int rc_x = OK;
    int yleft, yright;
    int xleft, xright;

    rc_y = find_place(data->y, data->ny, y, &yleft, &yright);
    if (rc_y != EXTRP)
    {
        rc_x = find_place(data->x, data->nx, x, &xleft, &xright);
        if (rc_x != EXTRP)
        {
            if (rc_x != FOUND && rc_y != FOUND)
                *result = data->z[rc_x][rc_y];
            else
            {
                if (rc_x == FOUND && rc_y == FOUND)
                {
                    rc = choose_points(data, chosen, yleft, yright, xleft, xright);
                }
                else if (rc_y != FOUND && rc_x == FOUND)
                {
                    // or ny == 0
                    chosen->ny = 1;
                    rc = find_indexes(&xleft, &xright, data->nx, chosen->nx);
                    if (rc == OK)
                        place_by_indexes(data, chosen, rc_y, rc_y + 1, xleft, xright);
                }
                else if (rc_y == FOUND && rc_x != FOUND)
                {
                    // or nx == 0
                    chosen->nx = 1;
                    rc = find_indexes(&yleft, &yright, data->ny, chosen->ny);
                    if (rc == OK)
                        place_by_indexes(data, chosen, yleft, yright, rc_x, rc_x + 1);
                }
                if (rc == OK)
                {
                    *result = double_interpolation(chosen, x, y, &rc);
                }
            }
        }
        else
            rc = EXTRP;
    }
    return rc;
}
