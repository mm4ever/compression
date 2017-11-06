#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "imagecompression.h"

#define MAX_COMPRESS_CNT 5
#define COMPRESS_LENGTH 4096
#define EXPORT_PATH "./data/"

int mappingImageToArray(float *pImageHeight,
                        int headerDataArr[],
                        unsigned char *pMappingData)
{
    int min,max;
    int index = 1;              // 图像高度压缩到数组中的索引
    int picIdx,picLen;          // 当前图像高度信息在内存中的偏移量
    *(pMappingData+0) = headerDataArr[0];           //将图像数放入byte数组第一位
    for ( int i = 0; i < headerDataArr[0]; ++i )    //轮循每张图片
    {
        picIdx = headerDataArr[3*i+1];              // 表头中当前图像在内存的偏移量
        picLen = headerDataArr[3*i+2];              // 表头中当前图片的长度

        min = *(pImageHeight+picIdx);
        max = *(pImageHeight+picIdx);
        index += 6;

        for ( int j = 0; j < picLen; ++j )
        {
            if ( min >= *(pImageHeight+picIdx+j) )
            {   // 判断高度是否小于min,将较小的值赋给min
                min = *(pImageHeight+picIdx+j);
            }
            if ( max <= *(pImageHeight+picIdx+j) )
            {   // 判断高度是否大于max,将较大的值赋给max
                max = *(pImageHeight+picIdx+j);
            }
        }
        *(pMappingData+index-6) = picLen >> 8;     //当前图像长度高位
        *(pMappingData+index-5) = picLen;          //当前图像长度低位
        *(pMappingData+index-4) = min >> 8;        //最小值高位
        *(pMappingData+index-3) = min;             //最小值低位
        *(pMappingData+index-2) = max >> 8;        //最大值高位
        *(pMappingData+index-1) = max;             //最大值低位

        //轮循当前像素点数组，读取每个像素点高度信息，并映射到0-255
        for ( int j = 0; j < picLen; ++j)
        {
            //将当前像素点高度映射到0-255，并存入byte数组
            *(pMappingData+index+j) = (*(pImageHeight+picIdx+j)-min)*255/(max-min);
        }
        index += picLen;                           //将byte数组索引移动到下一张图片
    }
    return index;
}

void exportByteDataToFile( char outputName[],
                           unsigned char *pMappingData,
                           int arrayLength)
{
    FILE *file = fopen( outputName, "w");               // 打开文件，不存在则创建
    if ( file == NULL )
    {
        printf("open file error!\n");                   // 打开失败，打印错误信息
    }
    if (strstr(outputName, ".txt") == NULL)             // ^^!=NULL means exist
     {   //输出为二进制
         fwrite (pMappingData , sizeof(unsigned char), arrayLength, file);
     }
    else
    {   //输出为文本
        for ( int i = 0; i < arrayLength; ++i )         // 轮循数组
        {
            fprintf(file,"%d\n",*(pMappingData+i));     // 将数据写入文件
        }
    }
    fclose(file);                                       // 关闭打开的文件
}

void compressImage(float *pImageHeight,
                   unsigned char *pMappingData,
                   int perPicLenArr[],
                   int imageCount)
{
    char writeName[FILENAME_MAX];
    int index = 0;                                      // 图片在内存地址中的索引
    int curArrLen = 0;                                  // 待压缩数组长度
    int curPicCnt = 0;                                  // 待压缩图片数量
    int temp[4] = {1,0,0,0};                            // 临时表头数组
    int headerDataArr[3*MAX_COMPRESS_CNT+1];            // 表头数组
    int picLen = 0;                                     // 待压缩数组长度

    for ( int i = 0; i < imageCount; ++i )
    {
        temp[1] = index;                                // 当前图片的地址偏移量
        temp[2] = perPicLenArr[i];                      // 当前图片的长度
        temp[3] = i;                                    // 当前图片的索引
        sprintf(writeName,"%s",EXPORT_PATH);
        if ( COMPRESS_LENGTH >= perPicLenArr[i] )
        {
            headerDataArr[0] += 1;
            headerDataArr[curPicCnt*3+1] = index;
            headerDataArr[curPicCnt*3+2] = perPicLenArr[i];
            headerDataArr[curPicCnt*3+3] = i;
            picLen += perPicLenArr[i];
            curPicCnt++;
        }
        else
        {
            curArrLen = mappingImageToArray( pImageHeight,temp,pMappingData);
            sprintf(writeName,"%schip%.3d.txt",writeName,i);
            exportByteDataToFile(writeName,pMappingData,curArrLen);
        }
        if ( COMPRESS_LENGTH<=picLen || (0!=curPicCnt && i==imageCount-1) )
        {
            curArrLen = mappingImageToArray( pImageHeight,headerDataArr,pMappingData);
            for ( int j = 0; j < curPicCnt; ++j )
            {
                if( 0 == j )
                {
                    sprintf(writeName,"%schip%.3d",writeName,headerDataArr[3]);
                }
                else if(curPicCnt-1 == j)
                {
                    sprintf(writeName,"%s_chip%.3d.txt",writeName,headerDataArr[3*j+3]);
                }
                else
                {
                    sprintf(writeName,"%s_chip%.3d",writeName,headerDataArr[3*j+3]);
                }
            }
            exportByteDataToFile(writeName,pMappingData,curArrLen);
            picLen = 0;
            curPicCnt = 0;
            headerDataArr[0] = 0;
        }
        index += perPicLenArr[i];                       // 当前图片在内存地址中的索引
    }
}


