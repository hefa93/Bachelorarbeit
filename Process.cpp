#include <Process.h>
#include <stdio.h>
#include <opencv2\flann.hpp>
#include <opencv2\calib3d.hpp>
#include <opencv2\imgproc.hpp>
#include <opencv2\imgcodecs.hpp>
#include <opencv2\xfeatures2d.hpp>
#include <opencv2\features2d.hpp>
#include <iostream>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\core\mat.hpp>


using namespace cv::xfeatures2d;

Process::Process() {
};

Process::~Process() { 
};

void Process::processImage(cv::Mat3b& mat, cv::Mat& mat1, cv::Mat& mat2, cv::Mat& mat3) {

	cv::cvtColor(mat,mat1, CV_RGB2GRAY);
	
	
	/*************Konturenerkennung mit farblicher Kennzeichnung************
	
	cv::threshold(mat1, mat1, 128, 255, CV_THRESH_BINARY);

	std::vector<std::vector<cv::Point> > contours;
	cv::Mat contourOutput = mat1.clone();
	cv::findContours(contourOutput, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);

	cv::Mat contourImage(mat1.size(), CV_8UC3, cv::Scalar(0, 0, 0));
	cv::Scalar colors[3];
	colors[0] = cv::Scalar(0, 0, 255);
	colors[1] = cv::Scalar(0, 0, 255);
	colors[2] = cv::Scalar(0, 0, 255);
	for (size_t idx = 0; idx < contours.size(); idx++) {
		cv::drawContours(contourImage, contours, idx, colors[idx % 3]);
		
	}
		cv::imshow("Contour", contourImage);
		cvMoveWindow("Contour", 0, 0);
		***********************************ENDE**************************/
	 
	 
	 std::vector< cv::KeyPoint > keypoints1, keypoints2;

	 cv::Mat Descriptor1, Descriptor2;

	
	 
	 cv::Ptr<SURF> detector = SURF::create(400);
	 
	 detector->detectAndCompute(mat2, cv::Mat(), keypoints2, Descriptor2);

	 detector->detectAndCompute(mat1, cv::Mat(), keypoints1, Descriptor1);
	 
	 cv::FlannBasedMatcher matcher;
	 std::vector< cv::DMatch > matches;
	 matcher.match(Descriptor1, Descriptor2, matches);
	 
	 double max_dist = 0; 
	 double min_dist = 100;

	 for (int i = 0; i < Descriptor1.rows; i++)
	 {
		 double dist = matches[i].distance;
		 if (dist < min_dist) min_dist = dist;
		 if (dist > max_dist) max_dist = dist;
	 }

	 std::vector< cv::DMatch > good_matches;

	 for (int i = 0; i < Descriptor1.rows; i++)
	 {
		 if (matches[i].distance <= std::max(2 * min_dist, 0.02))
		 {
			 good_matches.push_back(matches[i]);
		 }
	 }

	 cv::Mat img_matches;
	 drawMatches(mat1, keypoints1, mat2, keypoints2,
		 good_matches, img_matches, 0, 0,
		 std::vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);


	 //-- Localize the object
	 std::vector<cv::Point2f> obj;
	 std::vector<cv::Point2f> scene;

	 for (int i = 0; i < good_matches.size(); i++)
	 {
		 //-- Get the keypoints from the good 

		 obj.push_back(keypoints1[good_matches[i].queryIdx].pt);
		 scene.push_back(keypoints2[good_matches[i].trainIdx].pt);
	 }

	 cv::Mat H = findHomography(scene, obj,    CV_RANSAC);

	 //-- Get the corners from the image_1 ( the object to be "detected" )
	 std::vector<cv::Point2f> obj_corners(4);
	 obj_corners[0] = cvPoint(0, 0); obj_corners[1] = cvPoint(mat1.cols, 0);
	 obj_corners[2] = cvPoint(mat1.cols, mat1.rows); obj_corners[3] = cvPoint(0, mat1.rows);
	 std::vector<cv::Point2f> scene_corners(4);

	 perspectiveTransform(obj_corners, scene_corners,  H);

	 

	 //-- Draw lines between the corners (the mapped object in the scene - image_2 )
	 line(img_matches, scene_corners[0] , scene_corners[1] , cv::Scalar(0, 255, 0), 4);
	 line(img_matches, scene_corners[1] , scene_corners[2] , cv::Scalar(0, 255, 0), 4);
	 line(img_matches, scene_corners[2] , scene_corners[3] , cv::Scalar(0, 255, 0), 4);
	 line(img_matches, scene_corners[3] , scene_corners[0] , cv::Scalar(0, 255, 0), 4);

	 mat3 = img_matches;

	 //-- Show detected matches
	 //imshow("Good Matches & Object detection", img_matches);

	

}
	




	 /*drawKeypoints(mat1, keypoints1, mat1, 0);
	 drawKeypoints(mat2, keypoints2, mat2, 0);
	 */
	 


void Process::processNextImage(cv::Mat3b& mat, cv::Mat& mat1, cv::Mat& mat2, cv::Mat& mat3) {

	cv::cvtColor(mat, mat1, CV_RGB2GRAY);


	std::vector< cv::KeyPoint > keypoints1, keypoints2;

	cv::Mat Descriptor1, Descriptor2;



	cv::Ptr<SURF> detector = SURF::create(400);

	detector->detectAndCompute(mat2, cv::Mat(), keypoints2, Descriptor2);

	detector->detectAndCompute(mat1, cv::Mat(), keypoints1, Descriptor1);

	cv::FlannBasedMatcher matcher;
	std::vector< cv::DMatch > matches;
	matcher.match(Descriptor1, Descriptor2, matches);

	double max_dist = 0;
	double min_dist = 100;

	for (int i = 0; i < Descriptor1.rows; i++)
	{
		double dist = matches[i].distance;
		if (dist < min_dist) min_dist = dist;
		if (dist > max_dist) max_dist = dist;
	}

	std::vector< cv::DMatch > good_matches;

	for (int i = 0; i < Descriptor1.rows; i++)
	{
		if (matches[i].distance <= std::max(2 * min_dist, 0.02))
		{
			good_matches.push_back(matches[i]);
		}
	}

	cv::Mat img_matches = mat3;;
	drawMatches(mat1, keypoints1, mat2, keypoints2,
		good_matches, img_matches, 0, 0,
		std::vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);


	//-- Localize the object
	std::vector<cv::Point2f> obj;
	std::vector<cv::Point2f> scene;

	for (int i = 0; i < good_matches.size(); i++)
	{
		//-- Get the keypoints from the good 

		obj.push_back(keypoints1[good_matches[i].queryIdx].pt);
		scene.push_back(keypoints2[good_matches[i].trainIdx].pt);
	}

	cv::Mat H = findHomography(scene, obj, CV_RANSAC);

	//-- Get the corners from the image_1 ( the object to be "detected" )
	std::vector<cv::Point2f> obj_corners(4);
	obj_corners[0] = cvPoint(0, 0); obj_corners[1] = cvPoint(mat1.cols, 0);
	obj_corners[2] = cvPoint(mat1.cols, mat1.rows); obj_corners[3] = cvPoint(0, mat1.rows);
	std::vector<cv::Point2f> scene_corners(4);

	perspectiveTransform(obj_corners, scene_corners, H);



	//-- Draw lines between the corners (the mapped object in the scene - image_2 )
	line(img_matches, scene_corners[0], scene_corners[1], cv::Scalar(255, 0, 0), 4);
	line(img_matches, scene_corners[1], scene_corners[2], cv::Scalar(255, 0, 0), 4);
	line(img_matches, scene_corners[2], scene_corners[3], cv::Scalar(255, 0, 0), 4);
	line(img_matches, scene_corners[3], scene_corners[0], cv::Scalar(255, 0, 0), 4);

	mat3 = img_matches;

}