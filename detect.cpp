#include "detect.h"


void traverse(cv::Mat rgbd, int w, int h, int wo, int ho, std::vector<cv::Mat> conv1, std::vector<cv::Mat> conv2, std::vector<std::vector<double>> probabilityMat ){
    // rgbd, block width, block height, width overlapping, height overlapping
    int lefttop_x = 0;
    int lefttop_y = 0;
    int x_count = rgbd.cols/(w-wo);
    int y_count = rgbd.rows/(h-ho);
    cv::Mat block;

    // Main traverse loop
    for(int x = 0; x < x_count; x++){
        for(int y = 0; y < y_count; y++){
            lefttop_x = x * (w - wo);
            lefttop_y = y * (h - ho);

            std::cout << "====22" << std::endl;
            block = cutBlock(rgbd, lefttop_x, lefttop_y, w, h);

            std::cout << "====11" << std::endl;
            bool result = calculateResult(block, conv1, conv2, probabilityMat);
            std::cout <<result <<std::endl;
        }
    }
}

cv::Mat cutBlock(cv::Mat rgbd, int lefttop_x, int lefttop_y, int w, int h ){
    cv::Mat block(w, h, CV_64FC4, cv::Scalar(0, 0, 0, 0));
    rgbd.convertTo(rgbd, CV_64FC4); // change unsigned int to float
    for(int i = 0; i < w; i++){
        for(int j = 0; i < h; j++){
            block.at<cv::Vec4b>(i,j) = rgbd.at<cv::Vec4b>(i+lefttop_x, j+lefttop_y);
        }
    }
}

bool calculateResult(cv::Mat block, std::vector<cv::Mat> conv1, std::vector<cv::Mat> conv2, std::vector<std::vector<double>> probabilityMat ){

    std::cout << "====" << std::endl;
    cv::Mat square(100, 100, CV_64FC4, cv::Scalar(0, 0, 0, 0));
    cv::Mat res1, res2, res3;
    cv::Size size(100,100);
    resize(block, square,size); // resize image
    square.convertTo(square, CV_64FC4); // change unsigned int to float

    std::cout << "====1" << std::endl;
    square = zeropadding(square, 3);
    //CONV1
    res1 = convolution(square, conv1, 8, 1);
    //RELU LAYER
    res1 = relu(res1, 1);
    //showImage(res1, "");
    //POOLING
    res1 = pooling(res1, 4, 1);
    //ZERO PADDING 2
    res1 = zeropadding(res1, 2);
    //CONV2
    res2 = convolution(res1, conv2, 16, 2);
    //showImage(res2, "../TLRConvNet/images/");
    //RELU LAYER
    res2 = relu(res2, 2);
    //POOLING
    res2 = pooling(res2, 4, 2);
    //Reshape

    std::cout << "====2" << std::endl;
    cv::Mat output(1, 576, CV_64F, cv::Scalar(0));
    std::vector<cv::Mat> splitLayer;
    cv::split(res2, splitLayer);
    for(int j = 0; j < splitLayer.size(); j++){
        for(int x = 0; x < 6; x++){
            for(int y = 0; y < 6; y++){
                output.at<double>(1, j*36 + x*5 + y) = res2.at<double>(x,y);
            }
        }
    }
    double positiveVal = 0;
    double negativeVal = 0;
    for(int i = 0; i < probabilityMat.at(0).size(); i++){ // 576
        positiveVal += (double) probabilityMat.at(0).at(i) * output.at<double>(1, i);
        negativeVal += (double) probabilityMat.at(1).at(i) * output.at<double>(1, i);
    }

    bool isTL = (positiveVal > negativeVal)? true:false;
    return isTL;
}
