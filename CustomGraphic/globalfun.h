#ifndef GLOBALFUN_H
#define GLOBALFUN_H

#include <QObject>
#include <QImage>
#include <QQuickItem>
//#include <opencv2/opencv.hpp>

class GlobalFun : public QObject
{
    Q_OBJECT
public:
    explicit GlobalFun(QObject *parent = nullptr);

//    // QImage 转 cv::Mat
//    static cv::Mat convertQImageToMat(QImage &image);

//    // cv::Mat 转 QImage
//    static QImage convertMatToQImage(const cv::Mat mat);



signals:

};

#endif // GLOBALFUN_H
