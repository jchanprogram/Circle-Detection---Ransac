#ifndef CIRCLE_H
#define CIRCLE_H

#include <stdio.h>
#include <iostream>

#include "Tool.h"
#include <opencv2/opencv.hpp>

class Circle
{
    public:
        Circle();
        Circle(unsigned int radius, cv::Point center);
        Circle(cv::Point a, cv::Point b, cv::Point c);
        virtual ~Circle();

        float calculPerimetre() const;
        float calculSurface() const;
        void afficherCaract();

        unsigned int getRadius() const;
        cv::Point getCenter() const;
        void setRadius(unsigned int radius);
        void setCenter(cv::Point a);

        //Circle detectorRansac(std::vector<cv::Point> contour, int nbIteration, int erreur);

protected:

private:

    unsigned int m_radius;
    cv::Point m_center;
};


#endif // CIRCLE_H
