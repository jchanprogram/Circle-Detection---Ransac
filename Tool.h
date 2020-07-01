#ifndef TOOL_H
#define TOOL_H

#include <stdio.h>
#include <iostream>
#include<math.h>

#include <vector>

#include "Circle.h"

#include <opencv2/opencv.hpp>

std::vector<double> calculCoefficientDroiteMediatrice(cv::Point a, cv::Point b);
cv::Point calculCenterCircumscribingTriangle(std::vector<double> mediatrice1, std::vector<double> mediatrice2);
double calculDistance(cv::Point a, cv::Point b);

//Circle detectorRansac(std::vector<cv::Point> contour, int nbIteration, int erreur);


#endif // TOOL_H