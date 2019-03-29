#include <stdio.h>
#include <stdlib.h>

#include "defines.h"
#include "allocate.h"
#include "process.h"



int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        info();
        return ERR_PARAM;
    }
    int rc = OK;
    table_t input_data;
    input_data.x = NULL;
    input_data.y = NULL;
    input_data.z = NULL;
    input_data.xn = 0;
    input_data.xy = 0;
    table_t selected;
    selected.x = NULL;
    selected.y = NULL;
    selected.z = NULL;
    int nx;
    int ny;
    double x_inp;
    double y_inp;
    double result = 0;

    FILE *input_file = fopen(argv[1], "r");
    if (input_file)
    {
        rc = read_file(input_file, &input_data);
        if (rc == OK)
        {
            print_data(input_data);
            printf("input nx ny: ");
            if (scanf("%d %d", &nx, &ny) == 2 && nx >= 0 && ny >= 0)
            {
                if (nx < input_data.xn && ny < input_data.xy)
                {
                    printf("input x y: ");
                    if (scanf("%lf %lf", &x_inp, &y_inp) == 2)
                    {
                        selected.xn = nx + 1;
                        selected.xy = ny + 1;
                        selected.x = calloc(selected.xn, sizeof(double));
                        selected.y = calloc(selected.xy, sizeof(double));
                        selected.z = allocate(selected.xn, selected.xy);
                        if (selected.x && selected.y && selected.z && rc == OK)
                        {
                            rc = calculate(&input_data, &selected, x_inp, y_inp, &result);
                            switch (rc)
                            {
                            case OK:
                                printf("result:\ninterpolation: z = %.6lf\nreal f: z = %.6lf\n", result, f(x_inp, y_inp));
                                break;
                            case ERR_MEM:
                                printf("memory errors occured, please try again!\n");
                                break;
                            case EXTRP:
                                printf("EXTRP occured, calculations stopped!\n");
                                break;
                            case DIV0:
                                printf("this method isn't suitable for such calculations!\n");
                                break;
                            default:
                                break;
                            }
                            free_matrix(selected.z, nx + 1);
                            free(selected.x);
                            free(selected.y);
                        }
                    }
                    else
                        rc = ERR_IO;
                }
                else
                {
                    printf("there is no enough data for calculations!\n");
                    rc = LACK_OF_DATA;
                }
            }
            else
                rc = ERR_IO;
            free_matrix(input_data.z, input_data.xn);
            free(input_data.x);
            free(input_data.y);
        }
        else
            printf("error (%d)\n", rc);
        fclose(input_file);
    }
    else
    {
        printf("error in openning file, please try again\n");
        rc = ERR_FILE;
    }
    if (rc == ERR_IO)
        printf("input error :(\n");
    return rc;
}
