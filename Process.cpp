
#include <stdio.h>
#include <opencv2\flann.hpp>
#include <opencv2\calib3d.hpp>
#include <opencv2\imgproc.hpp>
#include <opencv2\imgcodecs.hpp>
#include <opencv2\xfeatures2d.hpp>
#include <opencv2\features2d.hpp>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\core\mat.hpp>
#include <iostream>

#include <Sensor.h>
#include <util.h>
#include <Process.h>



using namespace cv::xfeatures2d;

Process::Process() {
};

Process::~Process() { 
};

void Process::processImage(cv::Mat3b& mat, cv::Mat& mat1, cv::Mat& mat2, cv::Mat& mat3, cv::Mat& img_matches, float* height, float* xMitte, float* yMitte, cv::Mat& matHelp2, cv::Mat4b& matDEPTH, int* xCheck, int* yCheck, float* gamma, float* a, float* b) {


	cv::cvtColor(mat,mat1, CV_RGB2GRAY);
	int i = 0;
	int j = 0;
	for (i = 100; i < 380; i++) {


		for (j = 100; j < 540; j++) {
			matHelp2.at<char>(i - 100, j - 100) = mat1.at<char>(i, j);
		}

	}

	
	
	
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
	 
	 
	std::vector< cv::KeyPoint > keypoints1;
	std::vector< cv::KeyPoint > keypoints2;

	 
	cv::Mat Descriptor1;
	cv::Mat Descriptor2;

	
	 
	 cv::Ptr<SURF> detector = SURF::create(400);
	 
	


	 detector->detectAndCompute(mat2, cv::Mat(), keypoints2, Descriptor2);

	 detector->detectAndCompute(matHelp2, cv::Mat(), keypoints1, Descriptor1);
	 
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
		 if (matches[i].distance <= max(2 * min_dist, 0.02))
		 {
			 good_matches.push_back(matches[i]);
		 }
	 }

	 
	 drawMatches(matHelp2, keypoints1, mat2, keypoints2,
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

	 cv::Mat H  = findHomography(scene, obj, CV_RANSAC);

	
	 //-- Get the corners from the image_1 ( the object to be "detected" )
	 std::vector<cv::Point2f> obj_corners(4);
	 obj_corners[0] = cvPoint(0, 0); obj_corners[1] = cvPoint(mat2.cols, 0);
	 obj_corners[2] = cvPoint(mat2.cols, mat2.rows); obj_corners[3] = cvPoint(0, mat2.rows);
	 std::vector<cv::Point2f> scene_corners(4);



	 if (H.rows != 3 || H.cols != 3) {
		 return;
	 }
	 
	 perspectiveTransform(obj_corners, scene_corners, H);

	 //-- Draw lines between the corners (the mapped object in the scene - image_2 )
	 line(img_matches, scene_corners[0] , scene_corners[1] , cv::Scalar(0, 255, 0), 4);
	 line(img_matches, scene_corners[1] , scene_corners[2] , cv::Scalar(0, 255, 0), 4);
	 line(img_matches, scene_corners[2] , scene_corners[3] , cv::Scalar(0, 255, 0), 4);
	 line(img_matches, scene_corners[3] , scene_corners[0] , cv::Scalar(0, 255, 0), 4);

	

	 

	


	 *height = scene_corners[3].y - scene_corners[0].y;
	 *xMitte = scene_corners[0].x + (scene_corners[1].x - scene_corners[0].x)/2;
	 *yMitte = scene_corners[0].y +(scene_corners[3].y - scene_corners[0].y)/2;

	 line(img_matches, cv::Point2f(*xMitte, *yMitte), cv :: Point2f(*xMitte + 2, *yMitte + 2), cv::Scalar(255, 0, 0), 4);

	

	 int scene_corners1x = static_cast<int>(scene_corners[0].x);
	 int scene_corners2x = static_cast<int>(scene_corners[1].x);
	 int scene_corners3x = static_cast<int>(scene_corners[2].x);
	 int scene_corners4x = static_cast<int>(scene_corners[3].x);
	 int scene_corners1y = static_cast<int>(scene_corners[0].y);
	 int scene_corners2y = static_cast<int>(scene_corners[1].y);
	 int scene_corners3y = static_cast<int>(scene_corners[2].y);
	 int scene_corners4y = static_cast<int>(scene_corners[3].y);
	
	 *xCheck = scene_corners2x - scene_corners1x - scene_corners3x + scene_corners4x;
	 *yCheck = scene_corners4y - scene_corners1y - scene_corners3y + scene_corners2y;
	 
	 *a = scene_corners2x - scene_corners1x;
	 *b = scene_corners4y - scene_corners1y;
	 float cQuadrat = (scene_corners2x - scene_corners4x)*(scene_corners2x - scene_corners4x) + (scene_corners4y - scene_corners2y)*(scene_corners4y - scene_corners2y);

	 float ergb = (*a * *a + *b * *b - cQuadrat) / (2 * *a * *b);
	 
	 
	 *gamma = std::acos(ergb);

	 float Pi = 3.14159265359;

	 *gamma = (*gamma / (2 * Pi)) * 360;

	/* if (a < 30 || b < 30 ) return;*/

	 //-- Show detected matches
	 

	 imshow("Good Matches & Object detection", img_matches);
	 cv::moveWindow("Good Matches & Object detection", 1800, 0);
	
};
	




	 /*drawKeypoints(mat1, keypoints1, mat1, 0);
	 drawKeypoints(mat2, keypoints2, mat2, 0);
	 */
	 




void Process::SiftAlgorithm (cv::Mat& img_1, cv::Mat& img_2){



	cv::Ptr<cv::Feature2D> f2d = cv::xfeatures2d::SIFT::create();


	std::vector<cv::KeyPoint> keypoints_1, keypoints_2;
	//f2d->detect(img_1, keypoints_1);
	f2d->detect(img_2, keypoints_2);


	cv::Mat descriptors_1, descriptors_2;
	//f2d->compute(img_1, keypoints_1, descriptors_1);
	f2d->compute(img_2, keypoints_2, descriptors_2);

	cv::Mat out0;
	drawKeypoints(img_1, keypoints_1, out0);
	imshow("KeyPoint0.jpg", out0);


	/*cv::FlannBasedMatcher matcher;
	std::vector< cv::DMatch > matches;
	matcher.match(descriptors_1, descriptors_2, matches);*/

	//cv::BFMatcher matcher;
	//std::vector< cv::DMatch > matches;
	//matcher.match(descriptors_1, descriptors_2, matches);


	//double max_dist = 0;
	//double min_dist = 100;

	//for (int i = 0; i < descriptors_1.rows; i++)
	//{
	//	double dist = matches[i].distance;
	//	if (dist < min_dist) min_dist = dist;
	//	if (dist > max_dist) max_dist = dist;
	//}

	//std::vector< cv::DMatch > good_matches;

	//for (int i = 0; i < descriptors_1.rows; i++)
	//{
	//	if (matches[i].distance <= max(2 * min_dist, 0.02))
	//	{
	//		good_matches.push_back(matches[i]);
	//	}
	//}

	//cv::Mat img_matches;
	//drawMatches(img_1, keypoints_1, img_2, keypoints_2,
	//	good_matches, img_matches, 0, 0,
	//	std::vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

	////-- Localize the object
	//std::vector<cv::Point2f> obj;
	//std::vector<cv::Point2f> scene;

	//for (int i = 0; i < good_matches.size(); i++)
	//{
	//	//-- Get the keypoints from the good 

	//	obj.push_back(keypoints_1[good_matches[i].queryIdx].pt);
	//	scene.push_back(keypoints_2[good_matches[i].trainIdx].pt);
	//}

	//cv::Mat H = findHomography(scene, obj, CV_RANSAC);


	////-- Get the corners from the image_1 ( the object to be "detected" )
	//std::vector<cv::Point2f> obj_corners(4);
	//obj_corners[0] = cvPoint(0, 0); obj_corners[1] = cvPoint(img_1.cols, 0);
	//obj_corners[2] = cvPoint(img_1.cols, img_1.rows); obj_corners[3] = cvPoint(0, img_1.rows);
	//std::vector<cv::Point2f> scene_corners(4);

	//if (H.rows == 3 || H.cols == 3) {
	//	perspectiveTransform(obj_corners, scene_corners, H);

	//	std::cout << "Dimension von H korrekt";
	//}


	////-- Draw lines between the corners (the mapped object in the scene - image_2 )
	//line(img_matches, scene_corners[0], scene_corners[1], cv::Scalar(0, 255, 0), 4);
	//line(img_matches, scene_corners[1], scene_corners[2], cv::Scalar(0, 255, 0), 4);
	//line(img_matches, scene_corners[2], scene_corners[3], cv::Scalar(0, 255, 0), 4);
	//line(img_matches, scene_corners[3], scene_corners[0], cv::Scalar(0, 255, 0), 4);





	////-- Show detected matches
	//imshow("Good Matches & Object detection", img_matches);
	
};

void Process::SurfAlgorithm(cv::Mat& mat1, cv::Mat& mat2) {
	
	cv::Mat Descriptor1;
	cv::Mat Descriptor2;
	cv::Mat img_matches;


	

	std::vector< cv::KeyPoint > keypoints1;
	std::vector< cv::KeyPoint > keypoints2;





	cv::Ptr<SURF> detector = SURF::create(400);

	detector->detect(mat2, keypoints2);

	detector->compute(mat2, keypoints2, Descriptor2);

	detector->detect(mat1, keypoints1);

	detector->compute(mat1, keypoints1, Descriptor1);
	//detector->detectAndCompute(mat2, cv::Mat(), keypoints2, Descriptor2);

	//detector->detectAndCompute(mat1, cv::Mat(), keypoints1, Descriptor1);
	cv::Mat out0;
	drawKeypoints(mat1, keypoints1, out0);
	//imshow("KeyPoint0.jpg", out0);

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
		if (matches[i].distance <= max(2 * min_dist, 0.02))
		{
			good_matches.push_back(matches[i]);
		}
	}


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

	if (H.rows == 3 || H.cols == 3) {
		perspectiveTransform(obj_corners, scene_corners, H);

		std::cout << "Dimension von H korrekt";
	}


	//-- Draw lines between the corners (the mapped object in the scene - image_2 )
	line(img_matches, scene_corners[0], scene_corners[1], cv::Scalar(0, 255, 0), 4);
	line(img_matches, scene_corners[1], scene_corners[2], cv::Scalar(0, 255, 0), 4);
	line(img_matches, scene_corners[2], scene_corners[3], cv::Scalar(0, 255, 0), 4);
	line(img_matches, scene_corners[3], scene_corners[0], cv::Scalar(0, 255, 0), 4);



	//-- Show detected matches
	imshow("Good Matches & Object detection", img_matches);

};




void Process::getDepthandSize(cv::Mat3b& matRGB, cv::Mat& matHELP, cv::Mat& matIMG2, cv::Mat& matMATCHES, cv::Mat& img_matches, float* height, float* xMitte, float* yMitte, cv::Mat& matHelp2, cv::Mat4b& matDEPTH, int* xCheck, int* yCheck, NUI_LOCKED_RECT& lockedRectDEPTH, NUI_LOCKED_RECT& lockedRectRGB, USHORT* depthValue, INuiFrameTexture* textureRGB, INuiFrameTexture* textureDEPTH, Sensor& sensor, Process& process, float* gamma) {
	float a = 0;
	float b = 0;
	while (*xCheck > 20 || *yCheck > 20 || *xCheck < -20 || *yCheck < -20 || *gamma >100 || *gamma <80 || a < 30 || b < 30 || *depthValue < 800) {
		
		sensor.updateTextureRGB();
		textureRGB = sensor.getTextureRGB();
		lockedRectRGB = sensor.getLockedRectRGB();
		textureRGB->LockRect(0, &lockedRectRGB, NULL, 0);

		sensor.updateTextureDEPTH();
		textureDEPTH = sensor.getTextureDEPTH();
		lockedRectDEPTH = sensor.getLockedRectDEPTH();
		textureDEPTH->LockRect(0, &lockedRectDEPTH, NULL, 0);

		convertNuiToMatRGB(lockedRectRGB, matRGB);
		convertNuiToMatDEPTH(lockedRectDEPTH, matDEPTH);
	

		
		process.processImage(matRGB, matHELP, matIMG2, matMATCHES, img_matches, height, xMitte, yMitte, matHelp2, matDEPTH, xCheck, yCheck, gamma, &a, &b);

		float c = 100;
		float d = 640;
		
		/*line(matDEPTH, cv::Point2f(d-(*xMitte + c), *yMitte + c), cv::Point2f(d-*xMitte - c, *yMitte + c), cv::Scalar(255, 0, 0), 4);
		cv::imshow("Depth", matDEPTH);
		cv::moveWindow("Depth", 1500, 400);
		line(matRGB, cv::Point2f(*xMitte + c, *yMitte + c), cv::Point2f(*xMitte + c, *yMitte + c), cv::Scalar(255, 0, 0), 4);
		cv::imshow("RGB", matRGB);
		cv::moveWindow("RGB", 700, 300);*/

		

		convertNuiToMatDEPTH(lockedRectDEPTH, matDEPTH);

		getDepthValue(lockedRectDEPTH, xMitte, yMitte, depthValue);
		if (cv::waitKey(25) == 'q') {
			break;
		
			

		}

		

		

		textureRGB->UnlockRect(0);
		sensor.releaseFrameRGB();
		textureDEPTH->UnlockRect(0);
		sensor.releaseFrameDEPTH();
	}

	};

void Process::get2DMap(cv::Mat3b& mat2DMap, USHORT* depthValueObj1, USHORT* depthValueObj2, USHORT* depthValueObj3, float* xMitteObj1, float* xMitteObj2, float* xMitteObj3) {

	
	

	for (int i = 0; i < 480 ; i++)	
	{
		
		cv::Vec3b *pointerToRow = mat2DMap.ptr<cv::Vec3b>(i);

		for (int j = 0; j < 640; j++)
		{
			//print Kinect
			if (i > 465 && j > 277 && j < 363) {
				unsigned char r = 255;
				unsigned char g = 69;
				unsigned char b = 0;

				pointerToRow[j] = cv::Vec3b(r, g, b);
			}

			//print firts Object 



			else if (i < (480 - (15 + static_cast<int>(*depthValueObj1 * 30 / 100))) && i > ( 480 - (15 + static_cast<int>(*depthValueObj1 * 30 / 100) + 24)) && j < (static_cast<int>((*xMitteObj1 + 100) + 9 * 3)) && j >(static_cast<int>((*xMitteObj1 + 100) - 9 * 3))) {

				unsigned char r = 0;
				unsigned char g = 0;
				unsigned char b = 205;

				pointerToRow[j] = cv::Vec3b(r, g, b);


			}

			//print second Object 



			else if (i < (480 - (15 + static_cast<int>(*depthValueObj2 * 30 / 100))) && i >(480 - (15 + static_cast<int>(*depthValueObj2 * 30 / 100) + 24)) && j < (static_cast<int>((*xMitteObj2 + 100) + 9 * 3)) && j >(static_cast<int>((*xMitteObj2 + 100) - 9 * 3))) {

				unsigned char r = 0;
				unsigned char g = 205;
				unsigned char b = 0;

				pointerToRow[j] = cv::Vec3b(r, g, b);


			}
		
			//print third Object 



			else if (i < ( 480 - (15 + static_cast<int>(*depthValueObj3 * 30 / 100))) && i > ( 480 - (15 + static_cast<int>(*depthValueObj3 * 30 / 100) + 77)) && j < (static_cast<int>((*xMitteObj3 + 100) + 4 * 3)) && j > (static_cast<int>((*xMitteObj3 + 100) - 4 * 3))) {

				unsigned char r = 0;
				unsigned char g = 0;
				unsigned char b = 0;

				pointerToRow[j] = cv::Vec3b(r, g, b);


			}

			else {
				unsigned char r = 255;
				unsigned char g = 255;
				unsigned char b = 255;

				pointerToRow[j] = cv::Vec3b(r, g, b);
			}


		}

	}
	
	return;
	
};

