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

	void processImage(cv::Mat3b& mat, cv::Mat& mat1, cv::Mat& mat2, cv::Mat& mat3);

	void processNextImage(cv::Mat3b& mat, cv::Mat& mat1, cv::Mat& mat2, cv::Mat& mat3);

};