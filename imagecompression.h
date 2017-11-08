#ifndef IMAGECOMPRESSION_H
#define IMAGECOMPRESSION_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

#define MAX_COMPRESS_CNT 5                  // 压缩的最大文件数
#define COMPRESS_LENGTH 4096                // 图像长度大于4096直接压缩,小于则多个图像一起压缩
#define MAX_DECOMPRESS_LEN 10000            // 待压缩数组最大长度

#define EXPORT_PATH "./data/"               // 导出压缩文件的路径
#define DIFF_PATH "./diff/"                 // 导出差值文件的路径

/**
 * @brief makeDirectory         创建文件夹
 *                              1.首先检查文件夹是否存在,不存在则创建
 *                              2.如果存在,则删除文件夹下所有文件
 * @param path                  待创建的文件夹路径
 * @return                      N/A
 */
void makeDirectory( const char path[] );

/**
 * @brief getNumberFromeString  获取字符串中的数字
 *                              传入字符串,将字符串中的数字提取存放到数组中
 * @param string                传入的字符串,待检查其中的数字
 * @param number                传入的数组,用于存放获取到的字符串中的数字
 * @return                      N/A
 */
void getNumberFromeString( char string[],
                           int number[] );

/**
 * @brief mappingImageToArray   将图像信息映射到待压缩数组
 *                              1.获取当前图像的长度,求出最大/小值,求出高度的映射值
 *                              2.将图片的各信息存入映射数组
 * @param pImageHeight          图像原始数据
 * @param headerDataArr         每张待压缩图片在内存地址中的索引和长度信息
 * @param pMappingData          映射后的图片信息存放地址
 * @return                      返回映射后待压缩数据长度
 */
int mappingImageToArray( float* pImageHeight,
                         int headerDataArr[],
                         unsigned char* pMappingData );

/**
 * @brief exportByteDataToFile  将传入的数据导出为文件
 * @param outputName            数据导出的文件名
 * @param pMappingData          待导出的数据头指针
 * @param arrayLength           待导出的数据长度
 * @return                      N/A
 */
void exportByteDataToFile( char outpurName[],
                           unsigned char* pMappingData,
                           int arrayLength );

/**
 * @brief exportByteDataToFile  将传入的数据导出为文件
 * @param outputName            数据导出的文件名
 * @param pMappingData          待导出的数据头指针
 * @param arrayLength           待导出的数据长度
 * @return                      N/A
 */
void exportFloatDataToFile( char outputName[],
                            float* pDiffData,
                            int arrayLength );

/**
 * @brief importDataFromFile    读取指定路径的文件
 * @param fileName              带有路径的文件名称
 * @param pDecompressData       解压文件后所有数据存放的地址
 * @param pCacheData            临时存放解压数据的地址
 * @param picLenArr             图像长度的数组
 * @return                      N/A
 */
void importDataFromFile( char fileName[],
                         float* pDecompressData,
                         int* pCacheData,
                         int picLenArr[] );

/**
 * @brief compressImage         压缩图像信息并导出到文件
 *                              if(图像长度<4096)
 *                                  {将图片存入待压缩数组}
 *                              else (图像长度>4096)
 *                                  {直接压缩图片}
 *                              if(待压缩图片长度>4096或是最后一张图像且有待压缩数组)
 *                                  {将长度大于4096的数组压缩
 *                                  或最后一张时将待压缩的数组压缩}
 * @param pPictureData          所有图片的原始高度信息
 * @param pMappingData          存放每张图片压缩后的信息
 * @param perPicLenArr          存放每张图片长度的数组
 * @param imageCount            图片数量
 * @return                      N/A
 */
void compressImage( float *pImageHeight,
                    unsigned char *pMappingData,
                    int picLenArr[],
                    int imageCount );

/**
 * @brief decompressImage       解压缩图像信息并计算与原图片高度的差值
 *                              1.打开文件夹(判断是否存在)
 *                              2.打开文件,并将文件中的值存入内存
 *                              3.根据读取的文件信息,将图像高度信息还原
 *                              4.将还原的高度信息与原数据比较差值
 *                              5.打印每张图片的高度值的差值
 * @param pPictureData          所有图片的原始高度信息
 * @param pDiffData             存放解压后与原图片高度差值的信息
 * @param perPicLenArr          存放每张图片长度的数组
 * @param imageCount            图片数量
 * @return                      N/A
 */
void decompressImage( float *pImageHeight,
                      float *pDiffData,
                      int perPicLenArr[],
                      int imageCount );


#endif // IMAGECOMPRESSION_H
