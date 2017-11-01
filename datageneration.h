#ifndef DATAGENERATION_H
#define DATAGENERATION_H

/**
 * @brief generateLengthData 指定图片数并随机生成每张图片的长度
 * @param imgLength[]        存放所有图像长度的内存地址
 * @param imageCount         所有图像的张数
 * @param minValue           图像长度的最小值
 * @param maxValue           图像长度的最大值
 */
void generateLengthData( int imgLength[],
                         int imageCount,
                         int minValue,
                         int maxValue);

/**
 * @brief generateHeightData 指定图片数并生成每张图片的高度
 * @param pImageHeight       存放所有图像高度的内存地址
 * @param imgLength          所有图像的总长度
 * @param minValue           图像高度的最小值
 * @param maxValue           图像高度的最大值
 */
void generateHeightData( float* pImageHeight,
                         int imageLength,
                         float minValue,
                         float maxValue );

#endif // DATAGENERATION_H
