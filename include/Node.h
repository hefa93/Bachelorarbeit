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
#include <conio.h>
class Nodes {
public:

	typedef struct pixel {
		bool walkable;
		bool open;
		bool closed;
		int parentx;
		int parenty;
		float g;
		float h;
		float f;
		int counter;
	} pixel_t;

	typedef struct openList {

		int x;
		int y;
		float f;

	} openList_t;



	Nodes();
	~Nodes();

	void initNode(int* startx, int* starty, int* endx, int* endy, int* startxobstacle, int* startyobstacle, int* endxobstacle, int* endyobstacle, cv::Mat3b& mat2DMap);
	void updateOpenList(int* currentx, int* currenty);
	void bSortOpenList();
	void findpath(int* startx, int*  starty, int* endx, int* endy, cv::Mat3b& mat2DMap);

	pixel_t **pixel = nullptr;
	openList_t *openList = nullptr;
	uint16_t h = 240;
	uint16_t w = 320;
private:

	

};