#include <Node.h>
#include <math.h>
#include <windows.h>

Nodes::Nodes()
{
	pixel = new pixel_t*[w];
	for (int i = 0; i < w; i++) {
		pixel[i] = new pixel_t[h];
	}

	openList = new openList_t [w*h];
};

Nodes::~Nodes()
{

}



void Nodes::initNode(int* startx, int* starty, int* endx, int* endy, int* startxobstacle, int* startyobstacle, int* endxobstacle, int* endyobstacle, cv::Mat3b& mat2DMap)
{
	
	int sxobst = static_cast<int>(*startxobstacle - (6 * 3) / 2);
	int exobst = static_cast<int>(*endxobstacle + (6 * 3) / 2);
	int syobst = static_cast<int>(*startyobstacle - (6 * 3) / 2);
	int eyobst = static_cast<int>(*endyobstacle + (6 * 3) / 2);
	float k = 1000;
	float betrag = 0;
	float g_h = 0;

	for (int x = 0; x < 320; x++)
	{
		for (int y = 0; y < 240; y++)
		{
			if (x == *startx && y == *starty)
			{


				pixel[x][y].walkable = true;
				pixel[x][y].open = true;
				pixel[x][y].closed = false;
				pixel[x][y].parentx = x;
				pixel[x][y].parenty = y;
				pixel[x][y].g = 0;
				pixel[x][y].h = sqrt((x - *endx) * (x - *endx) + (y - *endy) * (y - *endy));
				pixel[x][y].f = pixel[x][y].g + pixel[x][y].h;
				pixel[x][y].counter = 0;

				

			}

			else if (x >= sxobst  && x <= exobst && y >= syobst && y <= eyobst) {

				pixel[x][y].walkable = false;
				pixel[x][y].open = false;
				pixel[x][y].closed = false;
				pixel[x][y].parentx = 0;
				pixel[x][y].parenty = 0;
				pixel[x][y].g = 0;
				pixel[x][y].h = 0;
				pixel[x][y].f = 0;
				pixel[x][y].counter = 0;

			}
			else if (x == 0 || y == 0) {

				pixel[x][y].walkable = false;
				pixel[x][y].open = false;
				pixel[x][y].closed = false;
				pixel[x][y].parentx = 0;
				pixel[x][y].parenty = 0;
				pixel[x][y].g = 0;
				pixel[x][y].h = 0;
				pixel[x][y].f = 0;
				pixel[x][y].counter = 0;

			}

			else {
				pixel[x][y].walkable = true;
				pixel[x][y].open = false;
				pixel[x][y].closed = false;
				pixel[x][y].parentx = 0;
				pixel[x][y].parenty = 0;
				pixel[x][y].g = 0;
				pixel[x][y].h = sqrt((x - *endx) * (x - *endx) + (y - *endy) * (y - *endy));
				pixel[x][y].f = pixel[x][y].g + pixel[x][y].h;
				pixel[x][y].counter = 0;
				
				
			}

		}
	}

	for (int d_x = -10; d_x <= 10; d_x++)
	{

		for (int d_y = -10; d_y <= 10; d_y++)
		{

			betrag = sqrt((d_x*d_x + d_y*d_y));
			g_h = 0.5 * k * (10.5 - betrag)*(10.5 - betrag);
			if (betrag <= 10.5) {

				int x = sxobst + d_x;
				int y = syobst + d_y;

				pixel[x][y].g = g_h;
				mat2DMap.at<char>(y * 2, x * 6) = ((1-betrag/360)*255, (1 - betrag / 360) * 255, (1 - betrag / 360) * 255);
				x = sxobst + d_x;
				y = eyobst + d_y;

				pixel[x][y].g = g_h;
				mat2DMap.at<char>(y * 2, x * 6) = ((1 - betrag / 360) * 255, (1 - betrag / 360) * 255, (1 - betrag / 360) * 255);
				x = exobst + d_x;
				y = syobst + d_y;
			
				pixel[x][y].g = g_h;
				mat2DMap.at<char>(y * 2, x * 6) = ((1 - betrag / 360) * 255, (1 - betrag / 360) * 255, (1 - betrag / 360) * 255);
				x = exobst + d_x;
				y = eyobst + d_y;

				pixel[x][y].g = g_h;
				mat2DMap.at<char>(y * 2, x * 6) = ((1 - betrag / 360) * 255, (1 - betrag / 360) * 255, (1 - betrag / 360) * 255);
				x = (sxobst + exobst) / 2 + d_x;
				y = syobst + d_y;
				
				pixel[x][y].g = pixel[x][y].g + g_h;

				mat2DMap.at<char>(y * 2, x * 6) = ((1 - betrag / 360) * 255, (1 - betrag / 360) * 255, (1 - betrag / 360) * 255);

				cv::imshow("2dmap", mat2DMap);
				cv::moveWindow("2dmap", 300, 100);
				if (cv::waitKey(25) == 'q') {
					break;



				}
			}
		}
	}
		
};

void Nodes::updateOpenList(int* currentx, int* currenty){

	int t = 0;
	int i = 0;
	int j = 0;

	for (i = 0; i < 320; i++) {

		for (j = 0; j < 240; j++) {

			if ( (pixel[i][j].open == true) && (pixel[i][j].closed == false) )
			{
				openList[t].f = pixel[i][j].f;
				openList[t].x = i;
				openList[t].y = j;
				t++;

		

					
			}
		}
	}
};

void Nodes::bSortOpenList()
{
	float z = 0;
	int r = 0;
	int t = 0;
	int swap = 1;

	int i = 0;
	//float e = pixel[100][93].f;

	//std::cout << e << std::endl;
	while (swap != 0)
	{

		swap = 0;
		i = 0;
		while (openList[i].f >= 0) {

			if (openList[i].f > openList[i + 1].f && openList[i + 1].f > 0)
			{
				z = openList[i + 1].f;
				openList[i + 1].f = openList[i].f;
				openList[i].f = z;

				t = openList[i + 1].x;
				openList[i + 1].x = openList[i].x;
				openList[i].x = t;

				r = openList[i + 1].y;
				openList[i + 1].y = openList[i].y;
				openList[i].y = r;
				swap++;


			}
			//std::cout << openList[i].f << std::endl;
			//std::cout << openList[i].x << std::endl;
			//std::cout << openList[i].y << std::endl;
			i++;
		}
		//if (swap == 0) {
		//	for (t = 0; t < i; t++) {
		//		std::cout << openList[t].f << std::endl;
		//		std::cout << openList[t].x << std::endl;
		//		std::cout << openList[t].y << std::endl;
		//	}
		//}
	
	}

};

void Nodes::findpath(int* startx, int*  starty, int* endx, int* endy, cv::Mat3b& mat2DMap)
{
	int x = 0;
	int y = 0;
	int dx = -1;
	int dy;
	int currentx = *startx;
	int currenty = *starty;
	int lowestf = 10000;
	int x_alt = 0;
	int y_alt = 0;
	//float z = pixel[100][93].h;

	//std::cout << z << std::endl;

	while ((pixel[currentx][currenty].counter < 1) || (currentx != *endx) || (currenty != *endy) )
	{
		mat2DMap.at<char>(currenty * 2, currentx * 6)= (100, 0, 0);

	/*	x = currentx;
		y = currenty;
		int x_alt = 0;
		int y_alt = 0;*/

		//while (x != *startx || y != *starty) {


			//line(mat2DMap,  cv::Point2f(x_alt * 8, y_alt * 8), cv::Point2f(pixel[x][y].parentx * 8, pixel[x][y].parenty * 8), cv::Scalar(255, 20, 147), 4);
		/*	cv::circle(mat2DMap, cv::Point2f(pixel[x][y].parentx * 2, pixel[x][y].parenty * 2), 1, 1, 8);*/

			//x_alt = x;
			//y_alt = y;
			//x = pixel[x][y].parentx;

			//y = pixel[x_alt][y].parenty;
		
			cv::imshow("2DMap", mat2DMap);
			cv::moveWindow("2DMap", 300, 100);
			if (cv::waitKey(15) == 'q') {
				break;



			}
		//}
		
		for (dx = -1; dx <= 1; dx++)	
		{
			
			for (dy = -1; dy <= 1; dy++)
			{
				
			
				if ((dx == 0) || (dy == 0))
				{
					if ((dx*dx == 1) || ( dy*dy == 1)) {
						pixel[currentx + dx][currenty + dy].g = pixel[currentx][currenty].g + 1;
					}
				}
				
					else if ( (dx != 0) && (dy != 0) )
					{
						pixel[currentx + dx][currenty + dy].g = pixel[currentx][currenty].g + 1.4;
					}
					
					if ( (pixel[currentx + dx][currenty + dy].walkable == true) && (pixel[currentx + dx][currenty + dy].open == false) )
					{
						pixel[currentx + dx][currenty + dy].open = true;
						pixel[currentx + dx][currenty + dy].parentx = currentx;
						pixel[currentx + dx][currenty + dy].parenty = currenty;
						pixel[currentx + dx][currenty + dy].f = pixel[currentx + dx][currenty + dy].g + pixel[currentx + dx][currenty + dy].h;

					}

					else if ( (pixel[currentx + dx][currenty + dy].walkable == true) && (pixel[currentx + dx][currenty + dy].open == true))
					{
						//pixel[currentx][currenty].counter += 1;
						float help = 0;
						help = pixel[currentx + dx][currenty + dy].f;
						float compare = pixel[currentx + dx][currenty + dy].g + pixel[currentx + dx][currenty + dy].h;

						if (compare < help )
						{
							pixel[currentx + dx][currenty + dy].f = compare;
							pixel[currentx + dx][currenty + dy].parentx = currentx;
							pixel[currentx + dx][currenty + dy].parenty = currenty;
						}
					

					}
					//float e = pixel[currentx + dx ][currenty + dy].f;
					//
					//std::cout << e << std::endl;
					//std::cout << currentx + dx << std::endl;
					//std::cout << currenty + dy << std::endl;

			}
			
		}

	
		pixel[currentx][currenty].closed = true;
		
		
		updateOpenList(&currentx, &currenty);
		bSortOpenList();
	
		currentx = openList[0].x;
		currenty = openList[0].y;
		lowestf = openList[0].f;
		if (currentx == *endx && currenty == *endy) {
			pixel[currentx][currenty].counter++;
		}
	}


	x = *endx;
	y = *endy;
	x_alt = *endx;
	y_alt = *endy;

	while (x != *startx || y != *starty ) {


		//line(mat2DMap,  cv::Point2f(x_alt * 8, y_alt * 8), cv::Point2f(pixel[x][y].parentx * 8, pixel[x][y].parenty * 8), cv::Scalar(255, 20, 147), 4);
		cv::circle(mat2DMap, cv::Point2f(pixel[x][y].parentx * 2, pixel[x][y].parenty * 2), 1, 1, 8);

		x_alt = x;
		y_alt = y;
		x = pixel[x][y].parentx;

		y = pixel[x_alt][y].parenty;
		
			cv::imshow("2DMap", mat2DMap);
			cv::moveWindow("2DMap", 300, 100);
			if (cv::waitKey(200) == 'q') {
				break;



			}

			for (int q = 0; q < 480; q += 2)
			{

				line(mat2DMap, cv::Point2f(0, q), cv::Point2f(1280, q), cv::Scalar(210, 210, 210), 1);
				
			}

			for (int p = 0; p < 640; p += 2) {

				line(mat2DMap, cv::Point2f(p, 0), cv::Point2f(p, 960), cv::Scalar(210, 210, 210), 1);

			}
		
	}

	
}



