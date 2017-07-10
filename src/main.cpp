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
	
	INuiFrameTexture* textureRGB;
	INuiFrameTexture* textureDEPTH;
	NUI_LOCKED_RECT lockedRectRGB;
	NUI_LOCKED_RECT lockedRectDEPTH;

	Mat3b matRGB = Mat::zeros(480, 640, CV_8UC3);

	Mat4b matDEPTH = Mat::zeros(480, 640, CV_8UC4);
	Mat4b matDEPTH2 = Mat::zeros(480, 640, CV_8UC1);
	Mat matHELP = Mat::zeros(480, 640, CV_8UC1);
	Mat matHelp2 = Mat::zeros(280, 440, CV_8UC1);
	Mat matIMG1;
	Mat matIMG2;
	Mat Descriptor1;
	Mat Descriptor2;
	Mat img_matches;
	uchar xValue;
	USHORT depthvalue;
	float xMitte = 0;
	float yMitte = 0;
	int xCheck = 100;
	int yCheck = 100;

	
	Mat matMATCHES = Mat::zeros(480, 640, CV_8UC1);
	int i = 0;
	float height = 0;

	/*xMitte == 0 && yMitte == 0*/

	/*xCheck > 20 || yCheck > 20*/

	while (xCheck > 20 || yCheck > 20) {

		sensor.updateTextureRGB();
		textureRGB = sensor.getTextureRGB();
		lockedRectRGB = sensor.getLockedRectRGB();
		textureRGB->LockRect(0, &lockedRectRGB, NULL, 0);

		sensor.updateTextureDEPTH();
		textureDEPTH= sensor.getTextureDEPTH();
		lockedRectDEPTH = sensor.getLockedRectDEPTH();
		textureDEPTH->LockRect(0, &lockedRectDEPTH, NULL, 0);

		convertNuiToMatRGB(lockedRectRGB, matRGB);
		convertNuiToMatDEPTH(lockedRectDEPTH, matDEPTH);
		

		matIMG2 = imread("GO.jpg", IMREAD_GRAYSCALE);
		process.processImage(matRGB, matHELP, matIMG2, matMATCHES, Descriptor1, Descriptor2, img_matches, &height, &xMitte, &yMitte, matHelp2, matDEPTH, &xCheck, &yCheck);

		getDepthValue(lockedRectDEPTH, &xMitte, &yMitte, &depthvalue);
		

	
		if (waitKey(25) == 'q') {
			break;


		}
		textureRGB->UnlockRect(0);
		sensor.releaseFrameRGB();
		textureDEPTH->UnlockRect(0);
		sensor.releaseFrameDEPTH();
		/*cout << xMitte, yMitte;
		
		cout << xValue;*/
	}

	cout << "Hello Wolrd";

		
//		while (true) {
//
//		sensor.updateTextureRGB();
//		textureRGB = sensor.getTextureRGB();
//		lockedRectRGB = sensor.getLockedRectRGB();
//		textureRGB->LockRect(0, &lockedRectRGB, NULL, 0);
//		sensor.updateTextureDEPTH();
//		textureDEPTH= sensor.getTextureDEPTH();
//		lockedRectDEPTH = sensor.getLockedRectDEPTH();
//		textureDEPTH->LockRect(0, &lockedRectDEPTH, NULL, 0);
//		
//		
//		convertNuiToMatRGB(lockedRectRGB, matRGB);
//		
//
//		matIMG1 = imread("StartGroﬂ_1280x920.jpg", IMREAD_GRAYSCALE);
//		matIMG2 = imread("GO.jpg", IMREAD_GRAYSCALE);
//
//		if (matIMG2.empty())                      
//		{
//			cout << "Could not open or find the image" << std::endl;
//			return -1;
//		}
//
//
//		
//		convertNuiToMatDEPTH(lockedRectDEPTH, matDEPTH);
//		
//		//process.SiftAlgorithm(matIMG1, matIMG2);
//				
//		/*process.SurfAlgorithm(matIMG1, matIMG2);
//*/
//		process.processImage(matRGB, matHELP, matIMG2, matMATCHES, Descriptor1, Descriptor2, img_matches, height, xMitte, yMitte, matHelp2);
//
//		
//
//		
//
//			/*imshow("RGB", matRGB);
//			cvMoveWindow("RGB", 1500, 0);*/
//
//	/*		if (waitKey(25) == 'f') {
//				break;
//			}
//		}*/
//		/*imshow("depth", matDEPTH);*/
//		//cvmovewindow("depth", 2150, 0);
//		textureRGB->UnlockRect(0);
//		sensor.releaseFrameRGB(); 
//
//		textureDEPTH->UnlockRect(0);
//		sensor.releaseFrameDEPTH();
//
//		if (waitKey(25) == 'q') {
//			break;
//		}
//	}
//
	return 0;
}