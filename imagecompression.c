#include "imagecompression.h"

void makeDirectory( const char path[] )
{
    DIR *dirPath = opendir (path);
    struct dirent *filePath = NULL;
    char fileName[FILENAME_MAX];

    if ( dirPath != NULL )
    {
        while ( (filePath = readdir(dirPath)) )    //while循环读取目录中的文件名
        {
            if( strcmp(filePath->d_name,".") == 0 ||
                    strcmp(filePath->d_name,"..") == 0 )
            {
                continue;
            }
            sprintf(fileName,"%s%s",path,filePath->d_name);
            remove(fileName);
        }
    }
    else
    {
        mkdir(path,0777);
    }
    (void) closedir (dirPath);               //关闭文件夹
}

int mappingImageToArray( float *pImageHeight,
                         int *headerDataArr,
                         unsigned char *pMappingData )
{
    int min,max;
    int index = 1;              // 图像高度压缩到数组中的索引
    int picIdx,picLen;          // 当前图像高度信息在内存中的偏移量
    *(pMappingData+0) = headerDataArr[0];           //将图像数放入byte数组第一位
    for ( int i = 0; i <  headerDataArr[0]; ++i )    //轮循每张图片
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
                           int arrayLength )
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

void compressImage( float *pImageHeight,
                    unsigned char *pMappingData,
                    int picLenArr[],
                    int imageCount )
{
    char writeName[FILENAME_MAX];
    int picOffset = 0;                                  // 图片在内存地址中的偏移
    int curArrLen = 0;                                  // 待压缩数组长度
    int curPicCnt = 0;                                  // 待压缩图片数量
    int picLen = 0;                                     // 待压缩数组长度
    int temp[4] = {1,0,0,0};                            // 临时表头数组
    int headerDataArr[3*MAX_COMPRESS_CNT+1];            // 表头数组
    headerDataArr[0] = 0;

    makeDirectory(EXPORT_PATH);                         // 创建存放压缩图片的文件夹
    for ( int i = 0; i < imageCount; ++i )
    {
        sprintf(writeName,"%s",EXPORT_PATH);
        if ( COMPRESS_LENGTH >= picLenArr[i] )
        {
            headerDataArr[0]++;
            headerDataArr[curPicCnt*3+1] = picOffset;
            headerDataArr[curPicCnt*3+2] = picLenArr[i];
            headerDataArr[curPicCnt*3+3] = i;
            picLen += picLenArr[i];
            curPicCnt++;
        }
        else
        {
            temp[1] = picOffset;                            // 当前图片的地址偏移量
            temp[2] = picLenArr[i];                         // 当前图片的长度
            temp[3] = i;                                    // 当前图片的索引
            sprintf(writeName,"%schip%.3d",writeName,i);
            curArrLen = mappingImageToArray( pImageHeight,temp,pMappingData);
            exportByteDataToFile(writeName,pMappingData,curArrLen);
        }
        if ( COMPRESS_LENGTH<=picLen || (0!=curPicCnt && i==imageCount-1) )
        {
            for ( int j = 0; j < curPicCnt; ++j )
            {
                if( 0 == j )
                {
                    sprintf (writeName,"%schip%.3d",writeName,headerDataArr[3*j+3]);
                }
                else
                {
                    sprintf(writeName,"%s_chip%.3d",writeName,headerDataArr[3*j+3]);
                }
            }
            curArrLen = mappingImageToArray( pImageHeight,headerDataArr,pMappingData);
            exportByteDataToFile(writeName,pMappingData,curArrLen);

            picLen = 0;
            curPicCnt = 0;
            headerDataArr[0] = 0;
        }
        picOffset += picLenArr[i];                       // 当前图片在内存地址中的索引
    }
}

void getNumberFromeString( char string[],
                           int number[] )
{
    int count=0;
    int index=0;
    int temp=0;

    if(string == NULL)
    {
        printf("error!\n");
    }

    while(string[index])
    {
        while(string[index] && (string[index]<'0' || string[index]>'9'))
        {
            index++;
        }
        if(!string[index])
        {
            break;
        }
        number[count] = 0;
        while(string[index] && (string[index]>='0' && string[index]<='9'))
        {
            temp = string[index] - '0';
            number[count] = 10*number[count] + temp;
            index++;
        }
        count++;
    }
}

void importDataFromFile( char fileName[],
                         float *pDecompressData,
                         int* pImgData,
                         int picLenArr[])
{
    FILE *file = fopen( fileName, "rb+");;  // 打开文件
    //轮循文件中每个值，将值赋给变量value,直至返回EOF(-1)
    for ( int i = 0,value; (value = getc(file)) != EOF; ++i )
    {   // 依次将文件中的值放入开辟的内存空间
        *(pImgData+i) = value;
    }
    fclose(file);                               // 关闭文件

    int picOffset;                              // 当前图像在存放图像信息的内存中的偏移量
    int len,min,max;                            // 文件中压缩的图像长度.最小值.最大值
    int pixelOffset = 1;                        // 高度信息偏移量,第一位存放图像数量
    int fileIndex[FILENAME_MAX];                // 文件所对应图像在图像长度数组的索引
    getNumberFromeString(fileName,fileIndex);
    for ( int i = 0; i < *pImgData; ++i )          // *pData为文件中压缩的图像数
    {
        // 通过保存在表头中的值，算出像素点个数、最小/大值
        len = (*(pImgData+pixelOffset+0)<<8)+*(pImgData+pixelOffset+1);
        min = (*(pImgData+pixelOffset+2)<<8)+*(pImgData+pixelOffset+3);
        max = (*(pImgData+pixelOffset+4)<<8)+*(pImgData+pixelOffset+5);
        pixelOffset += 6;

        picOffset = 0;
        // 通过获取的文件中的数字，算出对应图片在内存中的位置
        for ( int j = 0; j < fileIndex[i]; ++j)
        {
            picOffset += picLenArr[j];
        }

        for ( int j = 0; j < len; ++j )         // 轮循当前图片
        {
            // 读取压缩文件中的高度信息,解压并存入开辟的内存中
            *(pDecompressData+picOffset+j) = (*(pImgData+pixelOffset)) *
                    ( max - min ) / 255.0 + min;
            pixelOffset++;
        }
    }
}

void exportFloatDataToFile( char outputName[],
                            float *pDiffData,
                            int arrayLength )
{
    FILE *file = fopen( outputName, "w");               // 打开文件，不存在则创建
    if ( file == NULL )
    {
        printf("open file error!\n");                   // 打开失败，打印错误信息
    }
    if (strstr(outputName, ".txt") == NULL)             // ^^!=NULL means exist
    {   //输出为二进制
        fwrite (pDiffData , sizeof(float), arrayLength, file);
    }
    else
    {   //输出为文本
        for ( int i = 0; i < arrayLength; ++i )         // 轮循数组
        {
            fprintf(file,"%.2f\n",*(pDiffData+i));     // 将数据写入文件
        }
    }
    fclose(file);                                       // 关闭打开的文件

}

void decompressImage( float *pImageHeight,
                      float *pDecompressData,
                      int picLenArr[],
                      int imageCount )
{
    int* pImgData = malloc(MAX_DECOMPRESS_LEN*sizeof(int));
    if ( NULL == pImgData )
    {
        printf("malloc pImgData error!\n");
    }

    char fileName[FILENAME_MAX];                // 待操作的文件名
    DIR *directPath = opendir(EXPORT_PATH);     // 打开目标文件夹
    if ( directPath == NULL )                   // 如果目录不存在，打印错误信息
    {
        perror ("Couldn't open the directory!");
    }

    struct dirent *filePath = NULL;             // 待打开的文件路径
    while (( filePath = readdir(directPath) ))  // 依次读取每个文件
    {
        //如果是当前目录"."或上层目录".."则继续循环，否则向下执行程序
        if( strcmp(filePath->d_name,".") == 0 ||
                strcmp(filePath->d_name,"..") == 0 )
        {
            continue;
        }
        // 将文件名加上路径，赋值给fileName
        sprintf(fileName,"%s%s",EXPORT_PATH,filePath->d_name);
        // 读取文件,并将读取到的高度信息存入内存中
        importDataFromFile(fileName,pDecompressData,pImgData,picLenArr);
    }
    (void) closedir (directPath);               //关闭文件夹
    free(pImgData);
    pImgData = NULL;

    makeDirectory(DIFF_PATH);                   //创建存放差值的文件夹
    float* pDiff = malloc(MAX_DECOMPRESS_LEN*sizeof(float));
    if( NULL == pDiff )
    {
        printf("malloc pDiff error!\n");
    }

    int pixelOffset = 0;
    for(int i = 0; i < imageCount; ++i )    //导出当前图片的差值
    {
        sprintf(fileName,"%schip%.3d_diff.txt",DIFF_PATH,i);
        for(int j = 0; j < picLenArr[i]; ++j )
        {
            *(pDiff+j) = *(pImageHeight+pixelOffset) - *(pDecompressData+pixelOffset);
            pixelOffset++;
        }
        exportFloatDataToFile(fileName,pDiff,picLenArr[i]);
    }
    free(pDiff);
    pDiff = NULL;
}






