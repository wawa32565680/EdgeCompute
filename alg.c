#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define COUNT 5


double mean(double*);
int main(){
    FILE *file, *writeFile; //兩個檔案
    file = fopen("data.dat", "r"); //開啟來源data檔，模式為讀取
    if (file == NULL){   //確認檔案開啟成功
        printf("Open Fail\n");
        fclose(file);
        return 0;
    }
    writeFile = fopen("dataOut.dat", "a+");  //開啟結果檔案，模式為寫入
    char str[32] = ""; //儲存從來源檔獨到的字串
    char buffer[10] = ""; //儲存字串中的數字
    fgets(str, 32, file); //從來源檔讀取第一行 "number,InfluxDBValue"
    //以下依照演算法
    double tempdata[5] = {0.0};
    double data[5] = {0.0};
    double _mean, num, last_avg, next_val, avg_val = 0;
    num = 0.0;
    int i = 0, j;
    //number,InfluxDBValue
    fprintf(writeFile, "number,InfluxDBValue\n");
    do{
        memset(buffer, 0, 10); //清除儲存數字的字串以重新讀取新內容
        memset(str, 0, 32); //清除儲存來源檔的字串以重新讀取新內容
        fgets(str, 32, file); //從來源檔讀取一行字串
        if (*str != 0){  // 如果有讀到內容
            char *pointer = strchr(str, ',') + 1;
            strncpy(buffer, pointer, strchr(pointer, '\n') - pointer); //抓出數字
            // 以下依照演算法
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
                        fprintf(writeFile, "%d,%.1f\n", i, atof(buffer)); //將內容寫入結果檔
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
                            fprintf(writeFile, "%d,%.1f\n", i, atof(buffer)); //將內容寫入結果檔
                    }
                }
            }
            i++; //因使用 while 迴圈，需自行將 i 累加
        }
    } while (*str != 0); //若讀取到內容則繼續，讀取不到則結束
    
    
    fclose(writeFile);
    fclose(file);
}

double mean(double *arr){ //平均值的函式
    double sum = 0.0;
    char i;
    for (i = 0 ; i < 5 ; i++)
        sum += arr[i];
    return sum / 5.0;
}
