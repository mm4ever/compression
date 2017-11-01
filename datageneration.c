#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "datageneration.h"

void generateLengthData(int imgLength[],
                        int imageCount,
                        int minValue,
                        int maxValue)
{
    srand( (unsigned)time( NULL ));              //初始化随机数
    imgLength[imageCount-1] = 0;
    for (int i = 0; i < imageCount-1; ++i)         //随机生成长度信息
    {
        imgLength[i] = rand()%(maxValue-minValue+1)+minValue;
        imgLength[imageCount-1] += imgLength[i];
    }
}

void generateHeightData(float *pImageHeight,
                        int imageLength,
                        float minValue,
                        float maxValue)
{
    srand( (unsigned)time( NULL ) );             //初始化随机数

    for ( int i = 0; i < imageLength; ++i )      //随机生成高度信息
    {
        *(pImageHeight+i) = (float)
                (rand()%(100*((int)maxValue-(int)minValue)+1))/100+minValue;
    }
}
