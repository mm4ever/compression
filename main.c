#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>

#include "datageneration.h"

#define IMAGE_COUNT 1000                    // 生成图片数据数组的个数
#define MIN_IMAGE_LENGTH 2048               // 单张图片的最小长度
#define MAX_IMAGE_LENGTH 5000               // 单张图片的最大长度
#define MIN_IMAGE_HEIGHT 0.00               // 图片的最小高度
#define MAX_IAMGE_HEIGHT 5000.00            // 图片的最大高度

#define HEADER_LENGTH 6                     // 每张图片的表头长度


int main()
{
    mkdir("./data",0777);
    mkdir("./diff",0777);

    float* pImageHeight;                        // 随机生成的图像数据存放地址
    float* pDiffData;                           // 映射前后的数据差值存放地址
    unsigned char* pMappingData;                // 映射后的图像数据存放地址
    int imgLength[IMAGE_COUNT+1];                 // 存放每张图像长度的数组
    //int imgTotalLen[IMAGE_COUNT];               // 存放前N张图像长度总和的数组
    //int headerArr[IMAGE_COUNT*HEADER_LENGTH];   // 存放所有图片表头的数组


    // 随机生成每张图片的长度,并统计前N张图像长度总和
    generateLengthData(imgLength,IMAGE_COUNT+1,MIN_IMAGE_LENGTH,MAX_IMAGE_LENGTH);

    // 存放所有图像高度信息的内存空间
    pImageHeight = malloc(imgLength[IMAGE_COUNT] * sizeof(float));
    // 存放所有图像高度信息(映射后)的内存空间
    pMappingData = malloc(imgLength[IMAGE_COUNT] * sizeof(unsigned char));
    // 存放映射前后高度数据差值的内存空间
    pDiffData = malloc(imgLength[IMAGE_COUNT] * sizeof(float));

    //判断是否以开辟好内存
    if ( pImageHeight == NULL || pMappingData == NULL || pDiffData == NULL )
    {
        printf("创建数组失败!\n");
        return 1;
    }

    generateHeightData(pImageHeight,imgLength[IMAGE_COUNT],MIN_IMAGE_HEIGHT,MAX_IAMGE_HEIGHT);

    int index=0;
    char fileName[FILENAME_MAX];
    for( int i = 0; i < IMAGE_COUNT; ++i )
    {
        sprintf(fileName,"./data/chip_%03d.txt",i);
        FILE *file = fopen( fileName, "w");
        for( int j = 0; j < imgLength[i]; ++j )
        {
                fprintf(file,"%.2f\n",*(pImageHeight+index+j));
        }
        fclose(file);
        index += imgLength[i];
    }


    free(pImageHeight);
    free(pDiffData);
    free(pMappingData);
    pImageHeight = NULL;
    pDiffData = NULL;
    pMappingData = NULL;

    return 0;
}

















