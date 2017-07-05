#include <Windows.h>
#include <iostream>
#include <Sensor.h>
#include <Sift.h>
#include <util.h>
#include <Process.h>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\core\mat.hpp>




using namespace std;
using namespace cv;

#define width 640
#define height 480

int main() {
	Sensor sensor;
	Process process;
	Sift sift;
	try {
		sensor.initialize();
		//sensor.setCameraAngle(10);
	}
	catch (const std::exception& e) {
		std::cout << e.what();
		return 0;
	}
	
	INuiFrameTexture* textureRGB;
	INuiFrameTexture* textureDEPTH;
	NUI_LOCKED_RECT lockedRectRGB;
	NUI_LOCKED_RECT lockedRectDEPTH;

	Mat3b matRGB = Mat::zeros(480, 640, CV_8UC3);

	Mat4b matDEPTH = Mat::zeros(480, 640, CV_8UC4);
	Mat matHELP = Mat::zeros(480, 640, CV_8UC1);
	Mat matIMG1;
	Mat matIMG2;
	Mat matMATCHES = Mat::zeros(480, 640, CV_8UC1);

	while (true) {
		sensor.updateTextureRGB();
		textureRGB = sensor.getTextureRGB();
		lockedRectRGB = sensor.getLockedRectRGB();
		textureRGB->LockRect(0, &lockedRectRGB, NULL, 0);

		convertNuiToMatRGB(lockedRectRGB, matRGB);
		
		sensor.updateTextureDEPTH();
		textureDEPTH= sensor.getTextureDEPTH();
		lockedRectDEPTH = sensor.getLockedRectDEPTH();
		textureDEPTH->LockRect(0, &lockedRectDEPTH, NULL, 0);
		
		
		
		

		matIMG1 = imread("ziel2.jpg", IMREAD_GRAYSCALE);
		matIMG2 = imread("start3.jpg", IMREAD_GRAYSCALE);

		if (matIMG2.empty())                      
		{
			cout << "Could not open or find the image" << std::endl;
			return -1;
		}


		
		convertNuiToMatDEPTH(lockedRectDEPTH, matDEPTH);
				
	/*	while (TRUE) {*/
		

			//process.processImage(matRGB, matHELP, matIMG1, matMATCHES);
			
			

			/*imshow("RGB", matRGB);
			cvMoveWindow("RGB", 1500, 0);*/

	/*		if (waitKey(25) == 'f') {
				break;
			}
		}*/
		imshow("DEPTH", matDEPTH);
		cvMoveWindow("DEPTH", 2150, 0);
		textureRGB->UnlockRect(0);
		sensor.releaseFrameRGB(); 

		textureDEPTH->UnlockRect(0);
		sensor.releaseFrameDEPTH();

		if (waitKey(25) == 'q') {
			break;
		}
	}

	return 0;
}