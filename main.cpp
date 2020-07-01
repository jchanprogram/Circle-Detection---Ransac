#include <stdio.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>


#include "Circle.h"
#include "Tool.h"

#include <algorithm>
#include <random>
#include <chrono>


#include <opencv2/opencv.hpp>


Circle detectorRansac(std::vector<cv::Point> contour, int nbIteration, double erreur)
{
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::mt19937 generator(seed);
	unsigned valeur = generator();
	Circle bestCircle;
	
	if (contour.size() > 3)
	{
		int ensemblePoint = 3;
		int ensemblePointMax = 0;
		double error = 0;
		std::vector<cv::Point> tempContour = contour;
		for (int j = 0; j < nbIteration; j++)
		{

			std::shuffle(tempContour.begin(), tempContour.end(), generator);
			Circle tempCircle = Circle(tempContour[0], tempContour[1], tempContour[2]);


			for (int i = 3; i < tempContour.size(); i++)
			{
				error = abs(calculDistance(tempContour[i], tempCircle.getCenter())-tempCircle.getRadius());

				if (error <= erreur)
				{
					ensemblePoint++;
				}
			}
			if (ensemblePointMax < ensemblePoint && ensemblePoint > (int)tempContour.size()*0.95 && ensemblePoint>3) //0.95 pour piece1 0.95 piece 3
			{
				
				
					ensemblePointMax = ensemblePoint;
					bestCircle.setCenter(tempCircle.getCenter());
					bestCircle.setRadius(tempCircle.getRadius());
			}
			ensemblePoint = 3;
		}
	}

	return bestCircle;
}


int main(int argc, char** argv)
{
			
	/* Ouverture de l'image à traiter */
	

	cv::String image_path = "C:/yourfolder/";
	cv::String imageName = "piece1.jpg";

	cv::Mat imagePiece = cv::imread(image_path + imageName);
	if (imagePiece.empty()) // Check for failure
	{
		std::cout << "Could not open or find the image" << std::endl;
		system("pause"); //wait for any key press
		return -1;
	}

	cv::String windowNamePiece = "Piece Originale"; //Name of the window
	cv::namedWindow(windowNamePiece); // Create a window
	cv::moveWindow(windowNamePiece, 0, 0);
	imshow(windowNamePiece, imagePiece); // Show our image inside the created window.

	cv::waitKey(0); // Wait for any keystroke in the window

	/* Traitement de l'image */

	cv::Mat imagePieceGray = cv::imread(image_path + imageName, cv::IMREAD_GRAYSCALE);

	cv::String windowNamePieceGray = "Piece Originale Gray"; //Name of the window
	cv::namedWindow(windowNamePieceGray); // Create a window
	cv::moveWindow(windowNamePieceGray, 515, 0);
	imshow(windowNamePieceGray, imagePieceGray); // Show our image inside the created window.

	//cv::waitKey(0); // Wait for any keystroke in the window
	cv::Mat imagePieceGray2;
	cv::normalize(imagePieceGray, imagePieceGray2, 0, 255, cv::NORM_MINMAX);
	cv::String windowNamePieceGray2 = "Piece Originale Gray 2"; //Name of the window
	cv::namedWindow(windowNamePieceGray2); // Create a window
	cv::moveWindow(windowNamePieceGray2, 1030, 0);
	imshow(windowNamePieceGray2, imagePieceGray2); // Show our image inside the created window.

	cv::waitKey(0); // Wait for any keystroke in the window

	cv::GaussianBlur(imagePieceGray2, imagePieceGray2, cv::Size(13, 13), 0);  // 13 piece 1 // 13 piece3
	int edgeThresh = 30; // 30 piece 1 // 50 piece 3
	cv::Mat imagePieceGrayEdge;
	Canny(imagePieceGray2, imagePieceGrayEdge, edgeThresh, edgeThresh*3, 3);

	cv::String windowNamePieceGrayEdge = "Piece Originale Gray Edge"; //Name of the window
	cv::namedWindow(windowNamePieceGrayEdge); // Create a window
	cv::moveWindow(windowNamePieceGrayEdge, 0, 350);
	imshow(windowNamePieceGrayEdge, imagePieceGrayEdge);

	cv::waitKey(0);

	/***************************************/
	/*  RANSAC   */
	int ss = 5;
	cv::dilate(imagePieceGrayEdge, imagePieceGrayEdge, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(ss, ss))); //9 pour piece 1  // 5 piece 3
	cv::erode(imagePieceGrayEdge, imagePieceGrayEdge, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(ss, ss)));

	std::vector<std::vector<cv::Point> > contours;

	cv::findContours(imagePieceGrayEdge, contours, cv::RETR_LIST, cv::CHAIN_APPROX_NONE);

	cv::Mat outputContours = cv::Mat::zeros(imagePieceGray2.rows, imagePieceGray2.cols, CV_8UC3);
	cv::Scalar color(255, 0, 0);
	drawContours(outputContours, contours, -1, color, 1, 8);

	cv::String windowNamePieceGrayEdgeDraw = "Draw Gray Edge"; //Name of the window
	cv::namedWindow(windowNamePieceGrayEdgeDraw); // Create a window
	cv::moveWindow(windowNamePieceGrayEdgeDraw, 1050, 700);
	imshow(windowNamePieceGrayEdgeDraw, outputContours);
	cv::waitKey(0);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	

	// Fonction
	
	std::vector<Circle> detectedCircle;

	for (int i = 0; i < contours.size(); i++)
	{
		Circle tempDetectedCircle = detectorRansac(contours[i], 1000, 2);//1000.2 pour piece 1  // 1000 2 piece3
		if (tempDetectedCircle.getRadius() != 0 && tempDetectedCircle.getRadius()<std::min(imagePieceGray2.rows/2, imagePieceGray2.cols/2))// on limite les rayon detecte a la dimension la plus petite sur 2
		{
			detectedCircle.push_back(tempDetectedCircle);
		}
		std::cout << i << std::endl;
	}


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////

	cv::Mat outputRansac= cv::imread(image_path + imageName);
	for (int i = 0; i < detectedCircle.size(); i++)
	{
		circle(outputRansac, detectedCircle[i].getCenter(), 3, cv::Scalar(0, 255, 0), -1, 8, 0);
		circle(outputRansac, detectedCircle[i].getCenter(), detectedCircle[i].getRadius(), cv::Scalar(0, 0, 255), 3, 8, 0);
	}
	cv::String windowNameRansac = "RANSAC"; //Name of the window
	cv::namedWindow(windowNameRansac); // Create a window
	cv::moveWindow(windowNameRansac, 1030, 0);
	imshow(windowNameRansac, outputRansac); // Show our image inside the created window.

	/***************************************/
	//cv::imwrite(image_path+"RANSAC_"+ imageName, outputRansac);
	cv::waitKey(0);
	return 0;
}