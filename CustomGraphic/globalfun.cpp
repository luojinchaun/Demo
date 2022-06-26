#include "globalfun.h"

GlobalFun::GlobalFun(QObject *parent) : QObject(parent)
{

}

//cv::Mat GlobalFun::convertQImageToMat(QImage &image)
//{
//    //引用传递 (函数中image 地址 与传递进来的地址一样)
//    cv::Mat mat;
//    switch (image.format())
//    {
//    case QImage::Format_ARGB32:
//    case QImage::Format_RGB32:
//    case QImage::Format_ARGB32_Premultiplied:				//此处若使用 image.bits()  最终导致转换失败
//        mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
//        break;
//    case QImage::Format_RGB888:
//        mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
//        cv::cvtColor(mat, mat, cv::COLOR_BGR2RGB);
//        break;
//    case QImage::Format_Indexed8:
//        mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
//        break;
//    default: mat = cv::Mat(); break;
//    }

//    return mat;
//}

//QImage GlobalFun::convertMatToQImage(const cv::Mat mat)
//{
//    // 8-bits unsigned, NO. OF CHANNELS = 1
//    if (mat.type() == CV_8UC1)
//    {
//        QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
//        // Set the color table (used to translate colour indexes to qRgb values)
//        image.setColorCount(256);
//        for (int i = 0; i < 256; i++)
//        {
//            image.setColor(i, qRgb(i, i, i));
//        }
//        // Copy input Mat
//        uchar *pSrc = mat.data;
//        for (int row = 0; row < mat.rows; row++)
//        {
//            uchar *pDest = image.scanLine(row);
//            memcpy(pDest, pSrc, mat.cols);
//            pSrc += mat.step;
//        }
//        return image;					//         Index1
//    }
//    // 8-bits unsigned, NO. OF CHANNELS = 3
//    else if (mat.type() == CV_8UC3)
//    {
//        // Copy input Mat
//        const uchar *pSrc = (const uchar*)mat.data;
//        // Create QImage with same dimensions as input Mat
//        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
//        return image.rgbSwapped();		//         Index2
//    }
//    else if (mat.type() == CV_8UC4)
//    {
//        // Copy input Mat
//        const uchar *pSrc = (const uchar*)mat.data;
//        // Create QImage with same dimensions as input Mat
//        // 此处的ARGB32可以转为透明色
//        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
//        return image.copy();			//         Index3
//    }
//    else
//    {
//        return QImage();
//    }
//}


