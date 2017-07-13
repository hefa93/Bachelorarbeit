#include <Windows.h>
#include <iostream>
#include <Sensor.h>
#include <util.h>
#include <Process.h>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\core\mat.hpp>




using namespace std;

using namespace cv;



int main() {
	Sensor sensor;
	Process process;
	
	try {
		sensor.initialize();
		//sensor.setCameraAngle(10);
	}
	catch (const std::exception& e) {
		std::cout << e.what();
		return 0;
	}
	
	INuiFrameTexture* textureRGB = 0;
	INuiFrameTexture* textureDEPTH = 0;
	NUI_LOCKED_RECT lockedRectRGB;
	NUI_LOCKED_RECT lockedRectDEPTH;

	Mat3b matRGB = Mat::zeros(480, 640, CV_8UC3);

	Mat4b matDEPTH = Mat::zeros(480, 640, CV_8UC4);
	Mat4b matDEPTH2 = Mat::zeros(480, 640, CV_8UC1);
	Mat matHELP = Mat::zeros(480, 640, CV_8UC1);
	Mat matHelp2 = Mat::zeros(280, 440, CV_8UC1);
	Mat matIMG1;
	Mat matIMG2;
	Mat matIMG3;
	Mat Descriptor1;
	Mat Descriptor2;
	Mat img_matches;

	USHORT depthValueObj1;
	USHORT depthValueObj2;
	USHORT depthValueObj3;

	float xMitteObj1 = 0;
	float yMitteObj1 = 0;
	float xMitteObj2 = 0;
	float yMitteObj2 = 0;
	float xMitteObj3 = 0;
	float yMitteObj3 = 0;
	int xCheckObj1 = 100;
	int yCheckObj1 = 100;
	int xCheckObj2 = 100;
	int yCheckObj2 = 100;
	int xCheckObj3 = 100;
	int yCheckObj3 = 100;
	


	Mat matMATCHES = Mat::zeros(480, 640, CV_8UC1);
	int i = 0;
	float height = 0;
	float gamma = 0;

	matIMG1 = imread("END_Logo.jpg", cv::IMREAD_GRAYSCALE);
	matIMG2 = imread("GO.jpg", cv::IMREAD_GRAYSCALE);
	matIMG3 = imread("GO.jpg", cv::IMREAD_GRAYSCALE);

	process.getDepthandSize(matRGB, matHELP, matIMG1, matMATCHES, Descriptor1, Descriptor2, img_matches, &height, &xMitteObj1, &yMitteObj1, matHelp2, matDEPTH, &xCheckObj1, &yCheckObj1, lockedRectDEPTH, lockedRectRGB, &depthValueObj1, textureRGB, textureDEPTH, sensor, process, &gamma);

	


	cout << "Hello Wolrd";

		

	return 0;
}