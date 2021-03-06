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

	void processImage(cv::Mat3b& mat, cv::Mat& mat1, cv::Mat& mat2, cv::Mat& mat3, cv::Mat& img_matches, float* height, float* xMitte, float* yMitte, cv::Mat& matHelp2, cv::Mat4b& matDEPTH, int* xCheck, int* yCheck, float* gamma, float* a, float* b);

	void getDepthandSize(cv::Mat3b& mat, cv::Mat& mat1, cv::Mat& mat2, cv::Mat& mat3, cv::Mat& img_matches, float* height, float* xMitte, float* yMitte, cv::Mat& matHelp2, cv::Mat4b& matDEPTH, int* xCheck, int* yCheck, NUI_LOCKED_RECT& lockedRectDepth, NUI_LOCKED_RECT& lockedRectRGB, USHORT* depthValue, INuiFrameTexture* textureRGB, INuiFrameTexture* textureDEPTH, Sensor& sensor, Process& process, float* gamma);

	void SiftAlgorithm(cv::Mat& mat, cv::Mat& mat1);
	void SurfAlgorithm(cv::Mat& mat, cv::Mat& mat1);

	void get2DMap(cv::Mat3b& mat2DMap, USHORT* depthValueObj1, USHORT* depthValueObj2, USHORT* depthValueObj3, float* xMitteObj1, float* xMitteObj2, float* xMitteObj3);




};

