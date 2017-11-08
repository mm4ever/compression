#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "datageneration.h"
#include "imagecompression.h"


#define IMAGE_COUNT 1000                        // 生成图片数据数组的个数
#define MIN_IMG_LEN 2048                        // 单张图片的最小长度
#define MAX_IMG_LEN 5000                        // 单张图片的最大长度
#define MIN_IMG_HIGHT 0.00                      // 图片的最小高度
#define MAX_IMG_HIGHT 5000.00                   // 图片的最大高度
#define MAX_COMPRESS_LEN 10000                  // 待压缩数组最大长度


int main()
{
    clock_t start,finish;
    double runTime;
    start = clock();

    //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    //peter 171101 生成数据
    int imgLength[IMAGE_COUNT+1];               // 声明存放每张图像长度的数组
    float* pImageHeight;                        // 随机生成的图像数据存放地址

    // 随机生成每张图片的长度,并统计图像长度总和
    generateLengthData(imgLength,IMAGE_COUNT+1,MIN_IMG_LEN,MAX_IMG_LEN);   
    // 存放所有图像高度信息的内存空间
    pImageHeight = malloc(imgLength[IMAGE_COUNT] * sizeof(float));
    if ( NULL == pImageHeight )
    {
        printf("malloc error!\n");
        return 1;
    }
    // 随机生成每张图片的高度信息
    generateHeightData(pImageHeight,imgLength[IMAGE_COUNT],MIN_IMG_HIGHT,MAX_IMG_HIGHT);
    //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------



    //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    //peter 171103 压缩图片
    unsigned char* pMappingData;                // 映射后的图像数据存放地址

    // 存放所有图像高度信息(映射后)的内存空间
    pMappingData = malloc(MAX_COMPRESS_LEN*sizeof(unsigned char));
    if ( NULL == pMappingData )
    {
        printf("malloc error!\n");
        return 1;
    }
    compressImage(pImageHeight,pMappingData,imgLength,IMAGE_COUNT);
    //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------



    //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    //peter 171106 解压图片
    float* pDecompressData;                     // 解压后的高度信息存放地址

    // 存放映射前后高度数据差值的内存空间
    pDecompressData = malloc(imgLength[IMAGE_COUNT] * sizeof(float));
    if ( NULL == pDecompressData )
    {
        printf("malloc error!\n");
        return 1;
    }
    decompressImage(pImageHeight,pDecompressData,imgLength,IMAGE_COUNT);
    //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

    free(pImageHeight);
    free(pDecompressData);
    free(pMappingData);
    pImageHeight = NULL;
    pDecompressData = NULL;
    pMappingData = NULL;

    finish = clock();
    runTime = (double)(finish-start) / CLOCKS_PER_SEC;
    printf("%lf\n",runTime);

    return 0;
}

















