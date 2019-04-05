#include <stdio.h>
#include <stdlib.h>
#include "func.h"
#include "in_out.h"


int main(int argc, char *argv[])
{
    create_file(0, 6, 1, 0, 6, 1);
    if (argc != 2)
    {
        printf("<app.exe in.txt>\n");
        return ERR_PARAM;
    }
    int rc = OK;
    table_t data;
    data.x = NULL;
    data.y = NULL;
    data.z = NULL;
    data.nx = 0;
    data.ny = 0;
    table_t chosen;
    chosen.x = NULL;
    chosen.y = NULL;
    chosen.z = NULL;
    int nx;
    int ny;
    double x;
    double y;
    double result = 0;

    FILE *f = fopen(argv[1], "r");
    if (f)
    {
        rc = read_file(f, &data);
        if (rc == OK)
        {
            print_table(data);
            rc = input(&nx, &ny, &x, &y, data);
            if (rc ==OK)
            {
                chosen.nx = nx + 1;
                chosen.ny = ny + 1;
                chosen.x = calloc(chosen.nx, sizeof(double));
                chosen.y = calloc(chosen.ny, sizeof(double));
                chosen.z = allocate_matrix(chosen.nx, chosen.ny);
                if (chosen.x && chosen.y && chosen.z)
                {
                    rc = calculate(&data, &chosen, x, y, &result);
                    switch (rc)
                    {
                    case OK:
                        printf("\nInterpolation: z = %.6lf\nReal f: z = %.6lf\n", result, func(x, y));
                        break;
                    case ERR_MEM:
                        printf("Memory error!\n");
                        break;
                    case EXTRP:
                        printf("Extrapolation!\n");
                        break;
                    case DIV0:
                        printf("Zero division!\n");
                        break;
                    default:
                        break;
                    }
                    free_matrix(chosen.z);
                    free(chosen.x);
                    free(chosen.y);
                }
            }
            else
            {
                free_matrix(data.z);
                free(data.x);
                free(data.y);
            }
        }
        else
            printf("error (%d)\n", rc);
        fclose(f);
    }
    else
    {
        printf("error in openning file, please try again\n");
        rc = ERR_FILE;
    }
    if (rc == ERR_IO)
        printf("input error\n");
    return rc;
}
