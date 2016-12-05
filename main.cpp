#include "mainwindow.h"
#include <QApplication>
#include "convnets.h"
#include "detect.h"
#include <opencv/cv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <fstream>
#include <iostream>
#include <stdio.h>      /* printf */

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
    cv::FileNode kptFileNode2 = fs2["probMat2"];
    cv::read(kptFileNode2, probMat2);
    fs2.release();
    probabilityMat.push_back(probMat1);
    probabilityMat.push_back(probMat2);

    //std::cout << "Probability Mat get." << std::endl;
    //Get one single image:

    int index = 5;

    std::string imgPath = "../TLRAI/left_img/" + std::to_string( index ) + ".png";
    cv::Mat img = cv::imread(imgPath);
    std::string depthPath = "../TLRAI/depth256/" + std::to_string( index ) + ".png";
    cv::Mat depth = cv::imread(depthPath);
    depth.convertTo(depth, CV_8UC1);
    cv::Mat rgbd;
    std::vector<cv::Mat> splitBGR, splitDepth;
    cv::split(img, splitBGR);
    cv::split(depth, splitDepth);
    splitBGR.push_back(splitDepth[0]);
    cv::merge(splitBGR, rgbd);
/*
    for(int i = 0; i < 1280; i++){
        for(int j = 0; j < 720; j++){
            std::cout << rgbd.at<cv::Vec4b>(i,j) << std::endl;
        }
    }
*/

    //traverse
    traverse(img, rgbd, 200, 200, 100, 100, conv1, conv2, probabilityMat ); // rgbd, block width, block height, width overlapping, height overlapping



/*
    cv::namedWindow("image");
    cv::imshow("image", img);
    cv::waitKey(10);
*/
    return a.exec();
}
