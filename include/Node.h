#pragma once
#include <opencv2\flann.hpp>
#include <opencv2\calib3d.hpp>
#include <opencv2\imgproc.hpp>
#include <opencv2\imgcodecs.hpp>
#include <opencv2\xfeatures2d.hpp>
#include <opencv2\features2d.hpp>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\core\mat.hpp>
class Nodes {
public:

	struct Pixel {
		bool walkable;
		bool open;
		bool closed;
		int parentx;
		int parenty;
		float g;
		float h;
		float f;

	} Knoten[160][120];

	struct OpenList {

		int x;
		int y;
		float f;

	} OpenList[160*120];

	Nodes();
	~Nodes();

	void initNode(int* startx, int* starty, int* endx, int* endy, int* startxobstacle, int* startyobstacle, int* endxobstacle, int* endyobstacle);
	void updateOpenList();
	void bSortOpenList();
	void findpath(int* startx, int*  starty, int* endx, int* endy, cv::Mat3b& mat2DMap);
private:

	

};