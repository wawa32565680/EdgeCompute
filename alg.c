#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define COUNT 5


double mean(double*);
int main(){
    FILE *file, *writeFile;;
    file = fopen("data.dat", "r");
    if (file == NULL){
        printf("Open Fail\n");
        fclose(file);
        return 0;
    }
    writeFile = fopen("dataOut.dat", "a+");
    char str[32] = "";
    char buffer[10] = "";
    fgets(str, 32, file);
    double tempdata[5] = {0.0};
    double data[5] = {0.0};
    double _mean, num, last_avg, next_val, avg_val = 0;
    num = 0.0;
    int i = 0, j;
    //number,InfluxDBValue
    fprintf(writeFile, "number,InfluxDBValue\n");
    do{
        memset(buffer, 0, 10);
        memset(str, 0, 32);
        fgets(str, 32, file);
        if (*str != 0){
            char *pointer = strchr(str, ',') + 1;
            strncpy(buffer, pointer, strchr(pointer, '\n') - pointer); //抓出數字
            if (i < COUNT)
                tempdata[i] = atof(buffer);
            else{
                j = i % COUNT;
                tempdata[j] = atof(buffer);
                if (j == COUNT - 1)
                    data[j] = fabs(tempdata[j] - tempdata[0]);
                else
                    data[j] = fabs(tempdata[j] - tempdata[j + 1]);
                _mean = mean(data);
                if (_mean > 1.0){
                    if (num == 0.0){
                        last_avg = _mean;
                        num += 1.0;
                        fprintf(writeFile, "%d,%.1f\n", i, atof(buffer));
                    }
                    else{
                        next_val = _mean;
                        num += 1.0;
                        if (num > 2.0)
                            last_avg = avg_val;
                        avg_val = (last_avg * ((num - 1.0) / num) + next_val * (1.0 / num));
                        if (_mean > (avg_val * 3.5))
                            avg_val = last_avg;
                        if (_mean > avg_val)
                            fprintf(writeFile, "%d,%.1f\n", i, atof(buffer));
                    }
                }
            }
            i++;
        }
    } while (*str != 0);
    
    
    fclose(writeFile);
    fclose(file);
}

double mean(double *arr){
    double sum = 0.0;
    char i;
    for (i = 0 ; i < 5 ; i++)
        sum += arr[i];
    return sum / 5.0;
}