#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "imagecompression.h"


void findLimitValueInArr(unsigned char *pImageHeight,
                         int headerArr[],
                         int imgLength[],
                         int imageCount)
{
    int min;
    int max;
    int index=0;
    for ( int i = 0; i < imageCount; ++i )
    {
        min = *(pImageHeight+index);
        max = *(pImageHeight+index);

        for ( int j = 0; j < imgLength[i]; ++j )
        {
            if ( min >= *(pImageHeight+index+j) )
            {   // 判断高度是否小于min,将较小的值赋给min
                min = *(pImageHeight+index+j);
            }
            if ( max <= *(pImageHeight+index+j) )
            {   // 判断高度是否大于max,将较大的值赋给max
                max = *(pImageHeight+index+j);
            }
        }
        headerArr[6*i+0] = imgLength[i] >> 8;  //图像长度的高位
        headerArr[6*i+1] = imgLength[i];       //图像长度的低位
        headerArr[6*i+2] = min >> 8;           //最小值高位
        headerArr[6*i+3] = min;                //最小值低位
        headerArr[6*i+4] = max >> 8;           //最大值高位
        headerArr[6*i+5] = max;                //最大值低位
    }
}
