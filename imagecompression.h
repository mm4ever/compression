#ifndef IMAGECOMPRESSION_H
#define IMAGECOMPRESSION_H

/**
 * @brief mappingImageToArray   将图像信息映射到待压缩数组
 * @param pImageHeight          图像原始数据
 * @param imageInfo             每张待压缩图片在内存地址中的索引和长度信息
 * @param pMappingData          映射后的图片信息存放地址
 * @return                      返回映射后待压缩数据长度
 */
int mappingImageToArray(float* pImageHeight,
                        int imageInfo[],
                        unsigned char* pMappingData);

/**
 * @brief exportByteDataToFile  将传入的数据导出为文件
 * @param outputName            数据导出的文件名
 * @param pMappingData          待导出的数据头指针
 * @param arrayLength           待导出的数据长度
 */
void exportByteDataToFile( char outpurName[],
                           unsigned char* pMappingData,
                           int arrayLength);

void exportFloatDataToFile( char outpurName[],
                            float* pDiffData,
                            int arrayLength);
void importDataFromFile();

/**
 * @brief compressImage         压缩图像信息到文件
 * @param pPictureData          所有图片的高度信息
 * @param pMappingData          存放每张图片压缩后的信息
 * @param perPicLenArr          存放每张图片长度的数组
 * @param imageCount            图片数量
 */
void compressImage(float *pImageHeight,
                   unsigned char *pMappingData,
                   int perPicLenArr[],
                   int imageCount);

void decompressImage();


#endif // IMAGECOMPRESSION_H
