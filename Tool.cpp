#include <stdio.h>
#include <iostream>

#include <random>
#include <chrono>

#include "Tool.h"
#include "Circle.h"

/* Calcul equation de la droite supportant la médiatrice d'un coté */

std::vector<double> calculCoefficientDroiteMediatrice(cv::Point a, cv::Point b)
{
	/* Vecteur directeur de la médiatrice a pour coordonnées [-(yb-ya);(xb-xa)] */
	/* La médiatrice passe par le mileu du segment [AB] de coordonnée [(xb+xa)/2;(yb+ya)/2] */
	/* La droite est définie par Ax+By=C */

	std::vector<double> CoefficientDroiteMediatrice{0,0,0};

	CoefficientDroiteMediatrice[0] = 2 * (b.x - a.x);
	CoefficientDroiteMediatrice[1] = 2 * (b.y - a.y);
	CoefficientDroiteMediatrice[2] = b.x*b.x + b.y*b.y - a.x*a.x-a.y*a.y;

	return CoefficientDroiteMediatrice;
}

cv::Point calculCenterCircumscribingTriangle(std::vector<double> mediatrice1, std::vector<double> mediatrice2)
{
	/* Le centre du cercle circonscrit se trouve sur l'intersection de deux médiatrice */
	/* Les coordonnées (x,y) du centre se trouve en resolvant une equation à deux inconnue */
	/* A1*x+B1*y=C1 */
	/* A2*x+B2*y=C2 */
	/* On résout l'équation avec une méthode de déterminant */
	cv::Point center(0,0);
	double determinantEquation = ((double)mediatrice1[0] * mediatrice2[1] - (double)mediatrice1[1] * mediatrice2[0]);

	if (determinantEquation == 0)
	{
		//std::cout << "Les points ne forment pas un triangle " << std::endl;
		center.x = 0;
		center.y = 0;
	}

	else
	{
		center.x = ((double)mediatrice1[2] * mediatrice2[1] - (double)mediatrice1[1] * mediatrice2[2])/ determinantEquation;
		center.y = ((double)mediatrice1[0] * mediatrice2[2] - (double)mediatrice1[2] * mediatrice2[0] )/ determinantEquation;
	}

	return center;
}

double calculDistance(cv::Point a, cv::Point b)
{
	return sqrt(pow(b.x-a.x,2) + pow(b.y - a.y, 2));
}
