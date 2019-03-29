#include "process.h"
#include "defines.h"
#include <stdlib.h>
//#include <stdio.h>

int search_place(double *row, int len, double search, int *left, int *right)
{
    int mid;
    *left = 0, *right = len;
    while (*left <= *right)
    {
        mid = (*left + *right) / 2;
        if (IS_EQUAL(row[mid], search))
            return mid; // искомый элемент уже есть в массиве узлов
        if (row[mid] > search)
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
    if (*right >= len || *left < 0)
        return EXTRP; // экстраполяция

    return FOUND; // найдены индексы ближайших элементов
}

int get_indexes(int *left, int *right, int init_size, int selected_size)
{
    int index = 0;
    // один большой костыль )0)))0)
    while ((index < selected_size) && (*left >= 0 || *right < init_size))
    {
        if (*right < init_size)
        {
            index++;
            if (index < selected_size)
                (*right)++;
        }
        if (*left >= 0 && index < selected_size)
        {
            index++;
            if (((*right < init_size && index < selected_size - 1) || (*right >= init_size && index < selected_size)) && (*left) != 0)
                (*left)--;
        }
    }
    if (*right - *left != selected_size)
        (*right)++;
    if (index != selected_size)
        return LACK_OF_DATA;
    return OK;
}

void place_by_indexes(table_t *d_init, table_t *d_select, int pos_l_y, int pos_r_y, int pos_l_x, int pos_r_x)
{
    int index_i = 0;
    int index_j = 0;
    for (int i = pos_l_x; i < pos_r_x && index_i < d_select->xn; i++, index_i++)
        d_select->x[index_i] = d_init->x[i];
    for (int i = pos_l_y; i < pos_r_y && index_j < d_select->xy; i++, index_j++)
        d_select->y[index_j] = d_init->y[i];
    index_i = 0;
    index_j = 0;
    for (int i = pos_l_x; i < pos_r_x && index_i < d_select->xn; i++, index_i++)
    {
        index_j = 0;
        for (int j = pos_l_y; j < pos_r_y && index_j < d_select->xy; j++, index_j++)
        {
            d_select->z[index_i][index_j] = d_init->z[i][j];
        }
    }
}

int take_dots(table_t *d_init, table_t *d_select, int pos_l_y, int pos_r_y, int pos_l_x, int pos_r_x)
{
    int rc = OK;
    //printf("surrounding x: %d %d\n", pos_l_x, pos_r_x);
    //printf("surrounding y: %d %d\n", pos_l_y, pos_r_y);
    rc = get_indexes(&pos_l_x, &pos_r_x, d_init->xn, d_select->xn);
    if (rc == OK)
    {
        //printf("x indexes: %d - %d\n", pos_l_x, pos_r_x);
        rc = get_indexes(&pos_l_y, &pos_r_y, d_init->xy, d_select->xy);
        if (rc == OK)
        {
            //printf("y indexes: %d - %d\n", pos_l_y, pos_r_y);
            place_by_indexes(d_init, d_select, pos_l_y, pos_r_y, pos_l_x, pos_r_x);
        }
    }
    return rc;
}

double find(double *argument, double *value, int size, int degree, double search, int *rc)
{
    // x = search
    *rc = OK;
    double div;
    double res = 0.0;
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
            double mult = 1.0;
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

double double_interpolation(table_t *selected, double x_inp, double y_inp, int *rc)
{
    *rc = OK;
    double result = 0.0;
    double *new_z = calloc(selected->xn, sizeof(double));
    if (new_z)
    {
        for (int i = 0; i < selected->xn && *rc == OK; i++)
            new_z[i] = find(selected->y, selected->z[i], selected->xy, selected->xy - 1, y_inp, rc);
        if (*rc == OK)
        {
            result = find(selected->x, new_z, selected->xn, selected->xn - 1, x_inp, rc);
        }
        free(new_z);
    }
    else
        *rc = ERR_MEM;
    return result;
}

int calculate(table_t *input_data, table_t *selected, double x_inp, double y_inp, double *result)
{
    int rc = OK;
    int rc_y = OK;
    int rc_x = OK;
    int y_left, y_right;
    int x_left, x_right;

    rc_y = search_place(input_data->y, input_data->xy, y_inp, &y_left, &y_right);
    if (rc_y != EXTRP)
    {
        rc_x = search_place(input_data->x, input_data->xn, x_inp, &x_left, &x_right);
        if (rc_x != EXTRP)
        {
            if (rc_x != FOUND && rc_y != FOUND)
                *result = input_data->z[rc_x][rc_y];
            else
            {
                if (rc_x == FOUND && rc_y == FOUND)
                {
                    rc = take_dots(input_data, selected, y_left, y_right, x_left, x_right);
                }
                else if (rc_y != FOUND && rc_x == FOUND)
                {
                    // or ny == 0
                    selected->xy = 1;
                    rc = get_indexes(&x_left, &x_right, input_data->xn, selected->xn);
                    if (rc == OK)
                        place_by_indexes(input_data, selected, rc_y, rc_y + 1, x_left, x_right);
                }
                else if (rc_y == FOUND && rc_x != FOUND)
                {
                    // or nx == 0
                    selected->xn = 1;
                    rc = get_indexes(&y_left, &y_right, input_data->xy, selected->xy);
                    if (rc == OK)
                        place_by_indexes(input_data, selected, y_left, y_right, rc_x, rc_x + 1);
                }
                if (rc == OK)
                {
                    *result = double_interpolation(selected, x_inp, y_inp, &rc);
                }
            }
        }
        else
            rc = EXTRP;
    }
    return rc;
}
