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
	
	list = new list_t[w*h];
};

Nodes::~Nodes()
{

}



void Nodes::initNode2D(int* startx, int* starty, int* endx, int* endy, int* startxobstacle, int* startyobstacle, int* endxobstacle, int* endyobstacle, cv::Mat3b& mat2DMap)
{
	
	int sxobst = static_cast<int>(*startxobstacle - (6 * 3) / 2);
	int exobst = static_cast<int>(*endxobstacle + (6 * 3) / 2);
	int syobst = static_cast<int>(*startyobstacle - (6 * 3) / 2);
	int eyobst = static_cast<int>(*endyobstacle + (6 * 3) / 2);
	int color = 0;
	float k =20;
	float betrag = 0;
	float g_h = 0;
	float betrag_help = 0;

	for (int x = 0; x < 320; x++)
	{
		for (int y = 0; y < 240; y++)
		{

			 //******************************************************init start***********************************
			if (x == *endx && y == *endy)
			{


				pixel[x][y].walkable = true;
				pixel[x][y].open = true;
				pixel[x][y].closed = false;
				pixel[x][y].parentx = x;
				pixel[x][y].parenty = y;
				pixel[x][y].g = 0;
				pixel[x][y].h = 3 * sqrt((x - *startx) * (x - *startx) + (y - *starty) * (y - *starty));
				pixel[x][y].f = pixel[x][y].g + pixel[x][y].h;
				pixel[x][y].g_h = 0;
				pixel[x][y].init = true;
				

			}
			//********************************init config room obstacle********************************************
			
			else if (x == *startxobstacle && y == *startyobstacle) {
				for (int d_x = -(*startxobstacle - sxobst); d_x <= 0; d_x++)
				{

					for (int d_y = -9; d_y <= 0; d_y++)
					{
						betrag = sqrt((d_x*d_x + d_y*d_y));
						if (betrag <= 9.2) {
							pixel[x + d_x][y + d_y].walkable = false;
							pixel[x + d_x][y + d_y].open = false;
							pixel[x + d_x][y + d_y].closed = false;
							pixel[x + d_x][y + d_y].parentx = 0;
							pixel[x + d_x][y + d_y].parenty = 0;
							pixel[x + d_x][y + d_y].g = 0;
							pixel[x + d_x][y + d_y].h = sqrt((d_x + x - *startx) * (d_x + x - *startx) + (d_y + y - *starty) * (d_y + y - *starty));;
							pixel[x + d_x][y + d_y].f = 0;
							pixel[x + d_x][y + d_y].g_h = 0;
							pixel[x + d_x][y + d_y].init = true;


						}

					}
				}
				for (int d_x = -24; d_x <= 0; d_x++)
				{

					for (int d_y = -24; d_y <= 0 ; d_y++)
					{
						betrag = sqrt((d_x*d_x + d_y*d_y));
						if (betrag >= 9.2 && betrag <= 24.4) {
							betrag_help = (betrag - 9.2);
							g_h = 0.02 * k *((betrag_help - 15.2)  * (betrag_help - 15.2));
							color = 255 - (255 * (g_h / (0.02 * k*(15.2* 15.2))));
							pixel[x + d_x][y + d_y].walkable = true;
							pixel[x + d_x][y + d_y].open = false;
							pixel[x + d_x][y + d_y].closed = false;
							pixel[x + d_x][y + d_y].parentx = 0;
							pixel[x + d_x][y + d_y].parenty = 0;
							pixel[x + d_x][y + d_y].g_h = g_h;
							pixel[x + d_x][y + d_y].h = 3 * sqrt(((d_x + x - *startx) * (d_x + x - *startx) + (d_y + y - *starty) * (d_y + y - *starty)));
							pixel[x + d_x][y + d_y].f = h;
							
							pixel[x + d_x][y + d_y].init = true;
							cv::circle(mat2DMap, cv::Point2f((d_x + x) * 2, (d_y + y) * 2), 1, cv::Scalar(color,color,color), 1, 8);
							//cv::imshow("2DMap", mat2DMap);
							//cv::moveWindow("2DMap", 300, 100);
							//if (cv::waitKey(5) == 'q') {
							//	break;
							//}

						}
					}
				}
			}
			

			else if (x == *endxobstacle && y == *startyobstacle) {
				for (int d_x = 0; d_x <= 9; d_x++)
				{

					for (int d_y = -(*startyobstacle - syobst); d_y <= 0; d_y++)
					{
						betrag = sqrt((d_x*d_x + d_y*d_y));
						if (betrag <= 9.2) {
							pixel[x + d_x][y + d_y].walkable = false;
							pixel[x + d_x][y + d_y].open = false;
							pixel[x + d_x][y + d_y].closed = false;
							pixel[x + d_x][y + d_y].parentx = 0;
							pixel[x + d_x][y + d_y].parenty = 0;
							pixel[x + d_x][y + d_y].g = 0;
							pixel[x + d_x][y + d_y].h = ((d_x + x - *startx) * (d_x + x - *startx) + (d_y + y - *starty) * (d_y + y - *starty));
							pixel[x + d_x][y + d_y].f = 0;
							pixel[x + d_x][y + d_y].g_h = 0;
							pixel[x + d_x][y + d_y].init = true;


						}

					}
				}

				for (int d_x = 0; d_x <= 24; d_x++)
				{

					for (int d_y = -24; d_y <= 0; d_y++)
					{
						betrag = sqrt((d_x*d_x + d_y*d_y));
						if (betrag >= 9.2 && betrag <= 24.4) {
							betrag_help = (betrag - 9.2);
							g_h = 0.02 * k *((betrag_help - 15.2)  * (betrag_help - 15.2));
							color = 255 - (255 * (g_h / (0.02 * k*(15.2* 15.2))));
							pixel[x + d_x][y + d_y].walkable = true;
							pixel[x + d_x][y + d_y].open = false;
							pixel[x + d_x][y + d_y].closed = false;
							pixel[x + d_x][y + d_y].parentx = 0;
							pixel[x + d_x][y + d_y].parenty = 0;
							pixel[x + d_x][y + d_y].g_h = g_h;
							pixel[x + d_x][y + d_y].h = 3 * sqrt(((d_x + x - *startx) * (d_x + x - *startx) + (d_y + y - *starty) * (d_y + y - *starty)));
							pixel[x + d_x][y + d_y].f = h;
							
							pixel[x + d_x][y + d_y].init = true;
							cv::circle(mat2DMap, cv::Point2f((d_x + x) * 2, (d_y + y) * 2), 1, cv::Scalar(color, color, color), 1, 8);
							//cv::imshow("2DMap", mat2DMap);
							//cv::moveWindow("2DMap", 300, 100);
							//if (cv::waitKey(5) == 'q') {
							//	break;
							//}


						}
					}
				}
			}

			else if (x == *startxobstacle && y == *endyobstacle) {
				for (int d_x = -(*startxobstacle - sxobst); d_x <= 0; d_x++)
				{

					for (int d_y = 0; d_y <= 9; d_y++)
					{
						betrag = sqrt((d_x*d_x + d_y*d_y));
						if (betrag <= 9.2) {
							pixel[x + d_x][y + d_y].walkable = false;
							pixel[x + d_x][y + d_y].open = false;
							pixel[x + d_x][y + d_y].closed = false;
							pixel[x + d_x][y + d_y].parentx = 0;
							pixel[x + d_x][y + d_y].parenty = 0;
							pixel[x + d_x][y + d_y].g = 0;
							pixel[x + d_x][y + d_y].h = 3 * ((d_x + x - *startx) * (d_x + x - *startx) + (d_y + y - *starty) * (d_y + y - *starty));
							pixel[x + d_x][y + d_y].f = 0;
							pixel[x + d_x][y + d_y].g_h = 0;
							pixel[x + d_x][y + d_y].init = true;



						}

					}
				}
				for (int d_x = -24; d_x <= 0; d_x++)
				{

					for (int d_y = 0; d_y <= 24; d_y++)
					{
						betrag = sqrt((d_x*d_x + d_y*d_y));
						if (betrag >= 9.2 && betrag <= 24.4) {
							betrag_help = (betrag - 9.2);
							g_h = 0.02 * k *((betrag_help - 15.2)  * (betrag_help - 15.2));
							color = 255 - (255 * (g_h / (0.02 * k*(15.2* 15.2))));
							pixel[x + d_x][y + d_y].walkable = true;
							pixel[x + d_x][y + d_y].open = false;
							pixel[x + d_x][y + d_y].closed = false;
							pixel[x + d_x][y + d_y].parentx = 0;
							pixel[x + d_x][y + d_y].parenty = 0;
							pixel[x + d_x][y + d_y].g_h = g_h;
							pixel[x + d_x][y + d_y].h = 3 * sqrt(((d_x + x - *startx) * (d_x + x - *startx) + (d_y + y - *starty) * (d_y + y - *starty)));
							pixel[x + d_x][y + d_y].f = h;
							
							pixel[x + d_x][y + d_y].init = true;
							cv::circle(mat2DMap, cv::Point2f((d_x + x) * 2, (d_y + y) * 2), 1, cv::Scalar(color, color, color), 1, 8);
							//cv::imshow("2DMap", mat2DMap);
							//cv::moveWindow("2DMap", 300, 100);
							//if (cv::waitKey(5) == 'q') {
							//	break;
							//}

						}
					}
				}
			}

			else if (x == *endxobstacle && y == *endyobstacle) {
				for (int d_x = 0; d_x <= 9; d_x++)
				{

					for (int d_y = 0; d_y <= 9; d_y++)
					{
						betrag = sqrt((d_x*d_x + d_y*d_y));
						if (betrag <= 9.2) {
							pixel[x + d_x][y + d_y].walkable = false;
							pixel[x + d_x][y + d_y].open = false;
							pixel[x + d_x][y + d_y].closed = false;
							pixel[x + d_x][y + d_y].parentx = 0;
							pixel[x + d_x][y + d_y].parenty = 0;
							pixel[x + d_x][y + d_y].g = 0;
							pixel[x + d_x][y + d_y].h = 3 * ((d_x + x - *startx) * (d_x + x - *startx) + (d_y + y - *starty) * (d_y + y - *starty));
							pixel[x + d_x][y + d_y].f = 0;
							pixel[x + d_x][y + d_y].g_h = 0;
							pixel[x + d_x][y + d_y].init = true;

						}

					}
				}
				for (int d_x = 0; d_x <= 24; d_x++)
				{

					for (int d_y = 0; d_y <= 24; d_y++)
					{
						betrag = sqrt((d_x*d_x + d_y*d_y));
						if (betrag >= 9.2 && betrag <= 24.4) {
							betrag_help = (betrag - 9.2);
							g_h = 0.02 * k *((betrag_help - 15.2)  * (betrag_help - 15.2));
							color = 255 - (255 * (g_h / (0.02 * k*(15.2* 15.2))));
							pixel[x + d_x][y + d_y].walkable = true;
							pixel[x + d_x][y + d_y].open = false;
							pixel[x + d_x][y + d_y].closed = false;
							pixel[x + d_x][y + d_y].parentx = 0;
							pixel[x + d_x][y + d_y].parenty = 0;
							pixel[x + d_x][y + d_y].g_h = g_h;
							pixel[x + d_x][y + d_y].h = 3 * sqrt(((d_x + x - *startx) * (d_x + x - *startx) + (d_y + y - *starty) * (d_y + y - *starty)));
							pixel[x + d_x][y + d_y].f = h;
							
							pixel[x + d_x][y + d_y].init = true;
							cv::circle(mat2DMap, cv::Point2f((d_x + x) * 2, (d_y + y) * 2), 1, cv::Scalar(color, color, color), 1, 8);
							//cv::imshow("2DMap", mat2DMap);
							//cv::moveWindow("2DMap", 300, 100);
							//if (cv::waitKey(5) == 'q') {
							//	break;
							//}


						}

					}
				}


			}

			else if (x >= sxobst && x <= exobst && y >= *startyobstacle && y <= *endyobstacle) {

				pixel[x][y].walkable = false;
				pixel[x][y].open = false;
				pixel[x][y].closed = false;
				pixel[x][y].parentx = 0;
				pixel[x][y].parenty = 0;
				pixel[x][y].g = 0;
				pixel[x][y].h = 0;
				pixel[x][y].f = 0;
				pixel[x][y].g_h = 0;
				pixel[x][y].init = true;

				

			}


			else if (x >= *startxobstacle  && x <= *endxobstacle  && y >= syobst && y <= eyobst) {

				pixel[x][y].walkable = false;
				pixel[x][y].open = false;
				pixel[x][y].closed = false;
				pixel[x][y].parentx = 0;
				pixel[x][y].parenty = 0;
				pixel[x][y].g = 0;
				pixel[x][y].h = 0;
				pixel[x][y].f = 0;
				pixel[x][y].g_h = 0;
				pixel[x][y].init = true;


			}

			//**************************************init potentialfield method obstacle*****************

			else if (x >= sxobst - 15 && x <= sxobst  && y >= *startyobstacle && y <= *endyobstacle) {

				g_h = 0.02*k*(((sxobst - 15) - x)*((sxobst - 15) - x));
				color = 255 - (255 * (g_h / (0.02 * k*(15* 15))));
				pixel[x][y].open = false;
				pixel[x][y].closed = false;
				pixel[x][y].parentx = 0;
				pixel[x][y].parenty = 0;
				pixel[x][y].g = 0;
				pixel[x][y].walkable = true;
				pixel[x][y].g_h = g_h;
				pixel[x][y].h = 3 * sqrt(((x - *startx) * (x - *startx) + (y - *starty) * (y - *starty)));
				pixel[x][y].f = pixel[x][y].h;
				cv::circle(mat2DMap, cv::Point2f(x * 2, y * 2), 1,cv::Scalar(color,color,color), 1, 8);
				//cv::imshow("2DMap", mat2DMap);
				//cv::moveWindow("2DMap", 300, 100);
				//if (cv::waitKey(5) == 'q') {
				//	break;
				//}

			}

			else if (x >= exobst && x <= exobst + 15 && y >= *startyobstacle && y <= *endyobstacle) {

				g_h = 0.02*k*(((exobst + 15) - x)*((exobst + 15) - x));
				color = 255 - (255 * (g_h / (0.02 * k*(15 * 15))));
				pixel[x][y].open = false;
				pixel[x][y].closed = false;
				pixel[x][y].parentx = 0;
				pixel[x][y].parenty = 0;
				pixel[x][y].g = 0;
				pixel[x][y].walkable = true;
				pixel[x][y].g_h = g_h;
				pixel[x][y].h = 3 * sqrt(((x - *startx) * (x - *startx) + (y - *starty) * (y - *starty)));
				pixel[x][y].f = pixel[x][y].h;
				cv::circle(mat2DMap, cv::Point2f(x * 2, y * 2), 1, cv::Scalar(color, color, color), 1, 8);
				//cv::imshow("2DMap", mat2DMap);
				//cv::moveWindow("2DMap", 300, 100);
				//if (cv::waitKey(5) == 'q') {
				//	break;
				//}

			}

			else if (x >= *startxobstacle && x <= *endxobstacle  && y >= syobst - 15 && y <= syobst) {

				g_h = 0.02*k*(((syobst - 15) - y)*((syobst - 15) - y));
				color = 255 - (255 * (g_h / (0.02 * k*(15 * 15))));
				pixel[x][y].open = false;
				pixel[x][y].closed = false;
				pixel[x][y].parentx = 0;
				pixel[x][y].parenty = 0;
				pixel[x][y].g = 0;
				pixel[x][y].walkable = true;
				pixel[x][y].g_h = g_h;
				pixel[x][y].h = 3 * sqrt(((x - *startx) * (x - *startx) + (y - *starty) * (y - *starty)));
				pixel[x][y].f = pixel[x][y].h;
				cv::circle(mat2DMap, cv::Point2f(x * 2, y * 2), 1, cv::Scalar(color, color, color), 1, 8);
				//cv::imshow("2DMap", mat2DMap);
				//cv::moveWindow("2DMap", 300, 100);
				//if (cv::waitKey(5) == 'q') {
				//	break;
				//}

			}

			else if (x >= *startxobstacle && x <= *endxobstacle  && y >= eyobst && y <= eyobst + 15) {

				g_h = 0.02 *k*(((eyobst + 15) - y)*((eyobst + 15) - y));
				color = 255 - (255 * (g_h / (0.02 * k*(15 * 15))));
				pixel[x][y].open = false;
				pixel[x][y].closed = false;
				pixel[x][y].parentx = 0;
				pixel[x][y].parenty = 0;
				pixel[x][y].g = 0;
				pixel[x][y].walkable = true;
				pixel[x][y].g_h = g_h;
				pixel[x][y].h = 3 * sqrt(((x - *startx) * (x - *startx) + (y - *starty) * (y - *starty)));
				pixel[x][y].f = pixel[x][y].h;
				cv::circle(mat2DMap, cv::Point2f(x * 2, y * 2), 1, cv::Scalar(color, color, color), 1, 8);
				//cv::imshow("2DMap", mat2DMap);
				//cv::moveWindow("2DMap", 300, 100);
				//if (cv::waitKey(5) == 'q') {
				//	break;
				//}


			}


			

			//**************************************init edge ******************************************

			
			else if (x == 0 || y == 0 || x == 319 || y == 239) {

				pixel[x][y].walkable = false;
				pixel[x][y].open = false;
				pixel[x][y].closed = false;
				pixel[x][y].parentx = 0;
				pixel[x][y].parenty = 0;
				pixel[x][y].g = 0;
				pixel[x][y].h = 0;
				pixel[x][y].f = 0;
				pixel[x][y].g_h = 0;
				pixel[x][y].init = true;

			}

			//*******************************init any other pixel****************************************

			else {

				if (pixel[x][y].init != true){

					pixel[x][y].walkable = true;
					pixel[x][y].open = false;
					pixel[x][y].closed = false;
					pixel[x][y].parentx = 0;
					pixel[x][y].parenty = 0;
					pixel[x][y].g = 0;
					pixel[x][y].h = 3 * sqrt((x - *startx) * (x - *startx) + (y - *starty) * (y - *starty));
					pixel[x][y].f = pixel[x][y].g + pixel[x][y].h;
					pixel[x][y].g_h = 0;
					pixel[x][y].init = true;
				}
				
			}




		}
		cv::imshow("2DMap", mat2DMap);
		cv::moveWindow("2DMap", 300, 100);
		if (cv::waitKey(5) == 'q') {
			break;
		}

	}

	//for (int d_x = -15; d_x <= 15; d_x++)
	//{

	//	for (int d_y = -15; d_y <= 15; d_y++)
	//	{

	//		betrag = sqrt((d_x*d_x + d_y*d_y));
	//		g_h = 0.5 * k * (15.7 - betrag)*(15.7 - betrag);
	//		if (betrag <= 15.7) {

	//			int x = sxobst + d_x;
	//			int y = syobst + d_y;

	//			pixel[x][y].g_h = g_h;
	//			mat2DMap.at<char>(y * 2, x * 6) = (0, 0, 0);
	//			x = sxobst + d_x;
	//			y = eyobst + d_y;

	//			pixel[x][y].g_h = g_h;
	//			mat2DMap.at<char>(y * 2, x * 6) = (0, 0, 0);
	//			x = exobst + d_x;
	//			y = syobst + d_y;
	//		
	//			pixel[x][y].g_h = g_h;
	//			mat2DMap.at<char>(y * 2, x * 6) = (0, 0, 0);
	//			x = exobst + d_x;
	//			y = eyobst + d_y;

	//			pixel[x][y].g_h = g_h;
	//			mat2DMap.at<char>(y * 2, x * 6) = (0, 0, 0);
	//			x = (sxobst + exobst) / 2 + d_x;
	//			y = eyobst + d_y;
	//			
	//			pixel[x][y].g_h += g_h/4;

	//			mat2DMap.at<char>(y * 2, x * 6) = (0,0,0);

	//			cv::imshow("2Dmap", mat2DMap);
	//			cv::moveWindow("2Dmap", 300, 100);
	//			if (cv::waitKey(25) == 'q') {
	//				break;



	//			}
	//		}
	//	}
	//}
		
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

void Nodes::findpath2D(int* startx, int*  starty, int* endx, int* endy, cv::Mat3b& mat2DMap)
{
	int s_x = *startx;
	int s_y = *starty;
	int e_x = *endx;
	int e_y = *endy;
	int x = 0;
	int y = 0;
	int dx = -1;
	int dy;
	float e = 0;
	int currentx = e_x;
	int currenty = e_y;
	int lowestf = 10000;
	int x_alt = 0;
	int y_alt = 0;
	int counter = 0;


	while (!pixel[s_x][s_y].closed)
	{

		//mat2DMap.at<char>(currenty * 2, currentx * 2) = 100;
		//mat2DMap.at<char>(currenty * 2, currentx * 4) = 100;
		//mat2DMap.at<char>(currenty * 2, currentx * 6) = 40;
		//if (counter < 11) {
		//	counter++;
		//}
		//if (counter > 10) {
		//	counter = 0;
		//}
		//if (counter > 9) {
		//	x = currentx;
		//	y = currenty;
		//	x_alt = currentx;
		//	y_alt = currenty;

		//	while (x != *startx || y != *starty) {


		//		line(mat2DMap, cv::Point2f(x_alt*2 , y_alt*2 ), cv::Point2f(pixel[x][y].parentx *2 , pixel[x][y].parenty * 2), cv::Scalar(255, 20, 147), 4);
		//		/*	cv::circle(mat2DMap, cv::Point2f(pixel[x][y].parentx * 2, pixel[x][y].parenty * 2), 1, 1, 8);*/

		//		x_alt = x;
		//		y_alt = y;
		//		x = pixel[x][y].parentx;

		//		y = pixel[x_alt][y].parenty;

				//cv::imshow("2DMap", mat2DMap);
				//cv::moveWindow("2DMap", 300, 100);
				//if (cv::waitKey(15) == 'q') {
				//	break;



				//}
		//	}
		//}


		for (dx = -1; dx <= 1; dx++)	
		{
			
			for (dy = -1; dy <= 1; dy++)
			{
				
			
				if ((dx == 0) || (dy == 0))
				{
					if ((dx*dx == 1) || ( dy*dy == 1)) {
						pixel[currentx + dx][currenty + dy].g = pixel[currentx][currenty].g + 2;
					}
				}
				
					else if ( (dx != 0) && (dy != 0) )
					{
						pixel[currentx + dx][currenty + dy].g = pixel[currentx][currenty].g + 2*sqrt(2);
					}
					
					if ( (pixel[currentx + dx][currenty + dy].walkable == true) && (pixel[currentx + dx][currenty + dy].open == false) )
					{
						pixel[currentx + dx][currenty + dy].open = true;
						pixel[currentx + dx][currenty + dy].parentx = currentx;
						pixel[currentx + dx][currenty + dy].parenty = currenty;
						pixel[currentx + dx][currenty + dy].f = pixel[currentx + dx][currenty + dy].g + pixel[currentx + dx][currenty + dy].h + pixel[currentx + dx][currenty +dy].g_h;

					}

					else if ( (pixel[currentx + dx][currenty + dy].walkable == true) && (pixel[currentx + dx][currenty + dy].open == true) && dx != 0 && dy != 0)
					{
						//pixel[currentx][currenty].counter += 1;
						float help = 0;
						help = pixel[currentx + dx][currenty + dy].f;
						float compare = pixel[currentx + dx][currenty + dy].g + pixel[currentx + dx][currenty + dy].h + pixel[currentx + dx][currenty + dy].g_h;

						if (compare < help )
						{
							pixel[currentx + dx][currenty + dy].f = compare;
							pixel[currentx + dx][currenty + dy].parentx = currentx;
							pixel[currentx + dx][currenty + dy].parenty = currenty;
						}
					

					}


					}
				}

			
			
		

	
		pixel[currentx][currenty].closed = true;
		
		
		updateOpenList(&currentx, &currenty);
		bSortOpenList();
	
		currentx = openList[0].x;
		currenty = openList[0].y;
		lowestf = openList[0].f;
	
	}


	x = s_x;
	y = s_y;
	x_alt = s_x;
	y_alt = s_y;

	while (x != e_x || y != e_y ) {


		//line(mat2DMap,  cv::Point2f(x_alt * 8, y_alt * 8), cv::Point2f(pixel[x][y].parentx * 8, pixel[x][y].parenty * 8), cv::Scalar(255, 20, 147), 4);
		cv::circle(mat2DMap, cv::Point2f(pixel[x][y].parentx * 2, pixel[x][y].parenty * 2), 1, cv::Scalar(255,0,0), 1, 8);

		x_alt = x;
		y_alt = y;
		x = pixel[x][y].parentx;

		y = pixel[x_alt][y].parenty;
		
			cv::imshow("2DMap", mat2DMap);
			cv::moveWindow("2DMap", 300, 100);
			if (cv::waitKey(100) == 'q') {
				break;



			}

			//for (int q = 0; q < 480; q += 2)
			//{

			//	line(mat2DMap, cv::Point2f(0, q), cv::Point2f(1280, q), cv::Scalar(210, 210, 210), 1);
			//	
			//}

			//for (int p = 0; p < 640; p += 2) {

			//	line(mat2DMap, cv::Point2f(p, 0), cv::Point2f(p, 960), cv::Scalar(210, 210, 210), 1);

			//}
		
	}

	
};

void Nodes::bSortOpenListPFM(int* counter)
{
	float z = 0;
	int r = 0;
	int t = 0;
	int swap = 1;
	int i = 0;

	while (swap != 0)
	{

		swap = 0;
		i = 0;
		while (i < (*counter - 1)) {

			if (list[i].f > list[i + 1].f)
			{
				z =list[i + 1].f;
				list[i + 1].f =list[i].f;
				list[i].f = z;

				t =list[i + 1].x;
				list[i + 1].x =list[i].x;
				list[i].x = t;

				r =list[i + 1].y;
				list[i + 1].y =list[i].y;
				list[i].y = r;
				swap++;


			}
			i++;
		}

	}

};

void Nodes::findpathPFM(int* startx, int*  starty, int* endx, int* endy, cv::Mat3b& mat2DMap) {

	int currentx = 190;
	int currenty = 10;
	int x_alt = 0;
	int y_alt = 0;
	int help_x = 0;
	float help_y = 0;
	int x = 0;
	int y = 0;
	int counter = 0;

	while (currentx != *startx || currenty != *starty) {

		for (int dx = -1; dx <= 1; dx++)
		{

			for (int dy = -1; dy <= 1; dy++)
			{


				if (((dx == 0) || (dy == 0) )&& pixel[currentx + dx][currenty + dy].walkable == true )
				{
					if ((dx*dx == 1) || (dy*dy == 1)) {
						pixel[currentx + dx][currenty + dy].g = pixel[currentx][currenty].g + 0.5;
						pixel[currentx + dx][currenty + dy].f = pixel[currentx + dx][currenty + dy].g + pixel[currentx + dx][currenty + dy].h + pixel[currentx + dx][currenty + dy].g_h;
						list[counter].x = currentx + dx;
						list[counter].y = currenty + dy;
						list[counter].f = pixel[currentx + dx][currenty + dy].f;
						counter++;

					}
				}

				else if (((dx != 0) && (dy != 0)) && pixel[currentx + dx][currenty + dy].walkable == true)
				{
					pixel[currentx + dx][currenty + dy].g = pixel[currentx][currenty].g + 0.5 * sqrt(2);
					pixel[currentx + dx][currenty + dy].f = pixel[currentx + dx][currenty + dy].g + pixel[currentx + dx][currenty + dy].h + pixel[currentx + dx][currenty + dy].g_h;
					list[counter].x = currentx + dx;
					list[counter].y = currenty + dy;
					list[counter].f = pixel[currentx + dx][currenty + dy].f;
					counter++;
				}
				//if ((dx != 0) && (dy != 0)) {
				//	list[counter].x = currentx + dx;
				//	list[counter].y = currenty + dy;
				//	list[counter].f = pixel[currentx + dx][currenty + dy].f;
				//	counter++;

				//}


			}
		}
		bSortOpenListPFM(&counter);
		std::cout << currentx << std::endl;
		std::cout << currenty << std::endl;
		std::cout << std::endl;
		while (help_x < 8) {
			help_y = list[help_x].f;
			x = list[help_x].x;
			y = list[help_x].y;
			std::cout << help_y << std::endl;
			std::cout << x << std::endl;
			std::cout << y << std::endl;

			help_x++;
		}
		help_x = 0;
		std::cout << std::endl;
		cv::circle(mat2DMap, cv::Point2f(currentx * 2, currenty * 2), 1, cv::Scalar(255, 0, 0), 1, 8);
		cv::imshow("2DMap", mat2DMap);
		cv::moveWindow("2DMap", 300, 100);
		if (cv::waitKey(5) == 'q') {
			break;
		}
		if (list[0].x == x_alt && list[0].y == y_alt) {
			x_alt = currentx;
			y_alt = currenty;
			currentx = list[1].x;
			currenty = list[1].y;
			counter = 0;
		}
		
			x_alt = currentx;
			y_alt = currenty;
			currentx = list[0].x;
			currenty = list[0].y;
			counter = 0;
		

	}

};
