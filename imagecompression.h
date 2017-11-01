#ifndef IMAGECOMPRESSION_H
#define IMAGECOMPRESSION_H

/**
 * @brief findLimitValueInArr   计算每张图片的长度,最大值,最小值
 * @param pImageHeight          图像原始数据存放的内存地址
 * @param imgLength             每张图像长度存放的地址
 * @param totalDataLen          前N张图像的长度总和
 * @param imageCount            图像总数量
 */
void findLimitValueInArr(unsigned char* pImageHeight,
                         int imgLength[],
                         int totalDataLen[],
                         int imageCount);





#endif // IMAGECOMPRESSION_H
