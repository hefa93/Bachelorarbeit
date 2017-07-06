#pragma once
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\core\mat.hpp>
#include <opencv\highgui.h>
#include "opencv2/imgproc/imgproc.hpp"


class Process {
public:
	Process();
	~Process();

	void processImage(cv::Mat3b& mat, cv::Mat& mat1, cv::Mat& mat2, cv::Mat& mat3, cv::Mat& Descriptor1, cv::Mat& Descriptor2, cv::Mat& img_matches, float height, float xMitte, float yMitte);

	

	void SiftAlgorithm(cv::Mat& mat, cv::Mat& mat1);
	void SurfAlgorithm(cv::Mat& mat, cv::Mat& mat1);
};