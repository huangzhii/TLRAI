#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //Get Convnet:
    std::vector<cv::Mat> conv1 = convGenerator(7, 8, 1); // 7x7, 8 filters. The inside QVector is RGBD 4 layer, second QVector is # of filters.
    std::vector<cv::Mat> conv2 = convGenerator(5, 16, 2); // 5x5, 16 filters, 8 layers each.
    //Get ProbabilityMat:
    std::vector<double> probMat1, probMat2;
    std::vector<std::vector<double>> probabilityMat;
    cv::FileStorage fs("../TLRConvNet/probMat1.yml", cv::FileStorage::READ);
    cv::FileNode kptFileNode = fs["probMat1"];
    cv::read(kptFileNode, probMat1);
    fs.release();
    cv::FileStorage fs2("../TLRConvNet/probMat2.yml", cv::FileStorage::READ);
    cv::FileNode kptFileNode2 = fs2["probMat1"];
    cv::read(kptFileNode2, probMat2);
    fs2.release();
    probabilityMat.push_back(probMat1);
    probabilityMat.push_back(probMat2);

    //Get one single image:


    return a.exec();
}
