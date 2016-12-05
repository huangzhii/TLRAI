#ifndef DETECT_H
#define DETECT_H

#endif // DETECT_H

#include <iostream>
#include <QVector>
#include <stdio.h>      /* printf */
#include <math.h>
#include <opencv/cv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "convnets.h"
#include <sstream>


void traverse(cv::Mat img, cv::Mat rgbd, int w, int h, int wo, int ho, std::vector<cv::Mat> conv1, std::vector<cv::Mat> conv2, std::vector<std::vector<double>> probabilityMat );
cv::Mat cutBlock(cv::Mat rgbd, int lefttop_x, int lefttop_y, int w, int h );
double calculateResult(cv::Mat block, std::vector<cv::Mat> conv1, std::vector<cv::Mat> conv2, std::vector<std::vector<double>> probabilityMat );
