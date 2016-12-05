#include "detect.h"


void traverse(cv::Mat img, cv::Mat rgbd, int w, int h, int wo, int ho, std::vector<cv::Mat> conv1, std::vector<cv::Mat> conv2, std::vector<std::vector<double>> probabilityMat ){
    // rgbd, block width, block height, width overlapping, height overlapping
    int lefttop_x = 0;
    int lefttop_y = 0;
    //dimension of rgbd: rows:720 cols:1280
    int x_count = rgbd.rows/(w-wo) - 1; // 6
    int y_count = rgbd.cols/(h-ho) - 1; // 11

    cv::Mat block;

    cv::namedWindow("frame");
    cv::imshow("frame", img);
    cv::waitKey(100);

    // Main traverse loop
    for(int x = 0; x < x_count; x++){
        for(int y = 0; y < y_count; y++){


            lefttop_x = x * (w - wo);
            lefttop_y = y * (h - ho);
            block = cutBlock(rgbd, lefttop_x, lefttop_y, w, h);
            double val = calculateResult(block, conv1, conv2, probabilityMat);
            //std::cout << "2**************** current block x: " << lefttop_x << " current block y: " << lefttop_y << " is TL?: " << result <<std::endl;
            std::ostringstream strs;
            strs << std::fixed << std::setprecision(3) << val;
            std::string str = strs.str();

            cv::Scalar fontcolor;
            cv::Scalar rectcolor;
            int rectStride;
            if(val < -60){
                fontcolor = cv::Scalar(0,255,0,255);
                rectcolor = cv::Scalar(0,255,0);
                rectStride = 8;
            }
            else{
                fontcolor = cv::Scalar(0,0,255,255);
                rectcolor = cv::Scalar(0,0,0);
                rectStride = 0.5;
            }
            cv::rectangle( img, cv::Point( lefttop_y, lefttop_x ),
                       cv::Point( lefttop_y + w, lefttop_x + h ),
                       rectcolor, rectStride, 8, 0 );
            cv::putText(img, "rate: " + str, cv::Point2f(lefttop_y + w*0.6, lefttop_x + h*0.95), cv::FONT_HERSHEY_PLAIN, 0.8,  fontcolor, 1.5);
            cv::imshow("frame", img);
            cv::waitKey(10);
        }
    }
}

cv::Mat cutBlock(cv::Mat rgbd, int lefttop_x, int lefttop_y, int w, int h ){
    cv::Mat block(w, h, CV_64FC4, cv::Scalar(0, 0, 0, 0));
    rgbd.convertTo(rgbd, CV_64FC4); // change unsigned int to float

    for(int i = 0; i < w; i++){
        for(int j = 0; j < h; j++){
            /*if (lefttop_x == 600){
                std::cout << "oooooorigin: " <<i << " " << j << std::endl;
                std::cout << "plus offset: " <<i + lefttop_x << " " << j + lefttop_y << std::endl;
                std::cout << "dimension of rgbd: rows:" << rgbd.rows << " cols:" << rgbd.cols << std::endl;
                std::cout << "value of rgbd: " << rgbd.at<cv::Vec4b>(i+lefttop_x, j+lefttop_y) << std::endl;
            }*/
            block.at<cv::Vec4b>(i,j) = rgbd.at<cv::Vec4b>(i+lefttop_x, j+lefttop_y);
        }
    }
    return block;
}

double calculateResult(cv::Mat block, std::vector<cv::Mat> conv1, std::vector<cv::Mat> conv2, std::vector<std::vector<double>> probabilityMat ){
    cv::Mat square(100, 100, CV_64FC4, cv::Scalar(0, 0, 0, 0));
    cv::Mat res1, res2, res3;
    cv::Size size(100,100);
    resize(block, square,size); // resize image
    square.convertTo(square, CV_64FC4); // change unsigned int to float

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

    cv::Mat output(1, 576, CV_64F, cv::Scalar(0));
    std::vector<cv::Mat> splitLayer;
    cv::split(res2, splitLayer);
    for(int j = 0; j < splitLayer.size(); j++){
        for(int x = 0; x < 6; x++){
            for(int y = 0; y < 6; y++){
                output.at<double>(0, j*36 + x*6 + y) = splitLayer.at(j).at<double>(x,y);
            }
        }
    }
    double positiveVal = 0;
    double negativeVal = 0;
    for(int i = 0; i < probabilityMat.at(0).size(); i++){ // 576
        //std::cout << "=========================================== output: " << output.at<double>(0, i) << std::endl;
        positiveVal += (double) probabilityMat.at(0).at(i) * output.at<double>(0, i);
        negativeVal += (double) probabilityMat.at(1).at(i) * output.at<double>(0, i);
    }

    bool isTL = (positiveVal > negativeVal)? true:false;
    double portion = (double) positiveVal/negativeVal;
    double diff = positiveVal - negativeVal;

    std::cout << "portion: " << portion << " po: " << positiveVal << " ne: " << negativeVal << " diff: " << diff << std::endl;
    return diff;
}
