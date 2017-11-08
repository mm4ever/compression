#ifndef IMAGECOMPRESSION_H
#define IMAGECOMPRESSION_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

#define MAX_COMPRESS_CNT 5
#define COMPRESS_LENGTH 4096
#define MAX_DECOMPRESS_LEN 10000                  // 待压缩数组最大长度

#define EXPORT_PATH "./data/"
#define DIFF_PATH "./diff/"

/**
 * @brief makeDirectory         检查文件夹是否存在,不存在则创建;
 * @param path                  待创建的文件夹路径
 */
void makeDirectory( const char path[] );

/**
 * @brief getNumberFromeString  获取字符串中的数字
 * @param string                传入的字符串
 * @param number                传入的数组,用于存放获取到的字符串中的数字
 * @return                      N/A
 */
void getNumberFromeString( char string[],
                           int number[] );

/**
 * @brief mappingImageToArray   将图像信息映射到待压缩数组
 * @param pImageHeight          图像原始数据
 * @param imageInfo             每张待压缩图片在内存地址中的索引和长度信息
 * @param pMappingData          映射后的图片信息存放地址
 * @return                      返回映射后待压缩数据长度
 */
int mappingImageToArray( float* pImageHeight,
                         int imageInfo[],
                         unsigned char* pMappingData );

/**
 * @brief exportByteDataToFile  将传入的数据导出为文件
 * @param outputName            数据导出的文件名
 * @param pMappingData          待导出的数据头指针
 * @param arrayLength           待导出的数据长度
 */
void exportByteDataToFile( char outpurName[],
                           unsigned char* pMappingData,
                           int arrayLength );

/**
 * @brief exportByteDataToFile  将传入的数据导出为文件
 * @param outputName            数据导出的文件名
 * @param pMappingData          待导出的数据头指针
 * @param arrayLength           待导出的数据长度
 */
void exportFloatDataToFile( char outputName[],
                            float* pDiffData,
                            int arrayLength );


void importDataFromFile( char fileName[],
                         float* pDecompressData,
                         int* pCacheData,
                         int picLenArr[] );

/**
 * @brief compressImage         压缩图像信息并导出到文件
 * @param pPictureData          所有图片的原始高度信息
 * @param pMappingData          存放每张图片压缩后的信息
 * @param perPicLenArr          存放每张图片长度的数组
 * @param imageCount            图片数量
 */
void compressImage( float *pImageHeight,
                    unsigned char *pMappingData,
                    int picLenArr[],
                    int imageCount );

/**
 * @brief decompressImage       解压缩图像信息并计算与原图片高度的差值
 * @param pPictureData          所有图片的原始高度信息
 * @param pDiffData             存放解压后与原图片高度差值的信息
 * @param perPicLenArr          存放每张图片长度的数组
 * @param imageCount            图片数量
 */
void decompressImage( float *pImageHeight,
                      float *pDiffData,
                      int perPicLenArr[],
                      int imageCount );


#endif // IMAGECOMPRESSION_H
