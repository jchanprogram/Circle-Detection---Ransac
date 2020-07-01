#include <stdio.h>
#include <iostream>
#include <random>
#include <chrono>


#include "Circle.h"


Circle::Circle():m_radius(0),m_center(0,0)
{

}


Circle::Circle(unsigned int radius, cv::Point center):m_radius(radius), m_center(center)
{

}


Circle::Circle(cv::Point a, cv::Point b, cv::Point c)
{
	std::vector<double> mediatrice1 = calculCoefficientDroiteMediatrice(a,b);
	std::vector<double> mediatrice2 = calculCoefficientDroiteMediatrice(b,c);

	m_center = calculCenterCircumscribingTriangle(mediatrice1, mediatrice2);
	m_radius = calculDistance(a,m_center);
}

Circle::~Circle()
{

}

float Circle::calculPerimetre() const
{
	return 2 * CV_PI * m_radius;
}

float Circle::calculSurface() const
{
	return CV_PI * pow(m_radius, 2);
}

void Circle::afficherCaract()
{
	std::cout << "Centre du cercle : " << "[" << m_center.x << ";" << m_center.y << "]" << std::endl;
	std::cout << "Rayon du cercle : " << m_radius << std::endl;
}

unsigned int Circle::getRadius() const
{
	return m_radius;
}

cv::Point Circle::getCenter() const
{
	return m_center;
}

void Circle::setRadius(unsigned int radius)
{
	m_radius = radius;
}

void Circle::setCenter(cv::Point a)
{
	m_center.x = a.x;
	m_center.y = a.y;
}
