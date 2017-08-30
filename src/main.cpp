#include <Windows.h>
#include <iostream>
#include <Sensor.h>
#include <util.h>
#include <Process.h>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\core\mat.hpp>
#include <Node.h>




using namespace std;

using namespace cv;



int main() {
	Sensor sensor;
	Process process;
	
	Nodes nodes;

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

	Mat3b mat2DMap = Mat::zeros(480, 640, CV_8UC3);

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
	float height1 = 0;
	float height2 = 0;
	float height3 = 0;
	float gamma1 = 0;
	float gamma2 = 0;
	float gamma3 = 0;

	//matIMG1 = imread("SK_30.jpg", cv::IMREAD_GRAYSCALE);
	//matIMG2 = imread("SG_30.jpg", cv::IMREAD_GRAYSCALE);
	//while (TRUE) {
	//	process.SurfAlgorithm(matIMG1, matIMG2);
	//	if (cv::waitKey(25) == 'q') {
	//		break;

	//		

	//	}
	//}

	/*matIMG1 = imread("END_Logo.jpg", cv::IMREAD_GRAYSCALE);
	matIMG2 = imread("GO_Logo.jpg", cv::IMREAD_GRAYSCALE);
	matIMG3 = imread("OBSTACLE_Logo.jpg", cv::IMREAD_GRAYSCALE);

	process.getDepthandSize(matRGB, matHELP, matIMG1, matMATCHES, img_matches, &height1, &xMitteObj1, &yMitteObj1, matHelp2, matDEPTH, &xCheckObj1, &yCheckObj1, lockedRectDEPTH, lockedRectRGB, &depthValueObj1, textureRGB, textureDEPTH, sensor, process, &gamma1);

	process.getDepthandSize(matRGB, matHELP, matIMG2, matMATCHES, img_matches, &height2, &xMitteObj2, &yMitteObj2, matHelp2, matDEPTH, &xCheckObj2, &yCheckObj2, lockedRectDEPTH, lockedRectRGB, &depthValueObj2, textureRGB, textureDEPTH, sensor, process, &gamma2);

	process.getDepthandSize(matRGB, matHELP, matIMG3, matMATCHES, img_matches, &height3, &xMitteObj3, &yMitteObj3, matHelp2, matDEPTH, &xCheckObj3, &yCheckObj3, lockedRectDEPTH, lockedRectRGB, &depthValueObj3, textureRGB, textureDEPTH, sensor, process, &gamma3);
	*/
	depthValueObj1 = 900;
	depthValueObj2 = 900;
	depthValueObj3 = 900;
	xMitteObj1 = 305;
	xMitteObj2 = 99;
	xMitteObj3 = 200;
	
	process.get2DMap(mat2DMap, &depthValueObj1, &depthValueObj2, &depthValueObj3, &xMitteObj1, &xMitteObj2, &xMitteObj3);
		
	

	int startx = ((static_cast<int>((xMitteObj2 + 100) + 9 * 3)) + (static_cast<int>((xMitteObj2 + 100)- 9 * 3))) / 4;
	int starty = ((480 - (15 + static_cast<int>(depthValueObj2 * 30 / 100))) + (480 - (15 + static_cast<int>(depthValueObj2 * 30 / 100) + 24))) / 4;
	int endx = ((static_cast<int>((xMitteObj1 + 100)+ 9 * 3)) + (static_cast<int>((xMitteObj1 + 100) - 9 * 3))) / 4;
	int endy = ((480 - (15 + static_cast<int>(depthValueObj1 * 30 / 100))) + (480 - (15 + static_cast<int>(depthValueObj1 * 30 / 100) + 24))) / 4;
	int endxobstacle = (static_cast<int>((xMitteObj3 + 100) + 4 * 3)) / 2;
	int startxobstacle = (static_cast<int>((xMitteObj3 + 100) - 4 * 3)) / 2;
	int startyobstacle = (480 - (15 + static_cast<int>(depthValueObj3 * 30 / 100) + 77)) / 2;
	int endyobstacle = (480 - (15 + static_cast<int>(depthValueObj3 * 30 / 100))) / 2;

	//startx = 99 ;
	//starty = 93;
	//endx = 200;
	//endy = 89;
	//endxobstacle = 158;
	//startxobstacle = 146;
	//startyobstacle = 62;
	//endyobstacle = 100;

	nodes.initNode2D(&startx, &starty, &endx, &endy, &startxobstacle, &startyobstacle, &endxobstacle, &endyobstacle, mat2DMap);
	nodes.findpath2D(&startx, &starty, &endx, &endy, mat2DMap);

	

	while (TRUE) {
		cv::imshow("2DMap", mat2DMap);
		cv::moveWindow("2DMap", 300, 100);
		if (cv::waitKey(25) == 'q') {
			break;



		}
	}
	cout << "Hello World";

	return 0;
}