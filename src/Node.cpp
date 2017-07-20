#include <Node.h>
#include <math.h>

Nodes::Nodes()
{
	


};
Nodes::~Nodes()
{

}



void Nodes::initNode(int* startx, int* starty, int* endx, int* endy, int* startxobstacle, int* startyobstacle, int* endxobstacle, int* endyobstacle)
{
	
	
	for (int x = 0; x < 160; x++)
	{
		for (int y = 0; y < 120; y++)
		{
			if (x == *startx && y == *starty)
			{


				Knoten[x][y].walkable = true;
				Knoten[x][y].open = true;
				Knoten[x][y].closed = false;
				Knoten[x][y].parentx = x;
				Knoten[x][y].parenty = y;
				Knoten[x][y].g = 0;
				Knoten[x][y].h = sqrt((x - *endx) * (x - *endx) + (y - *endy) * (y - *endy));
				Knoten[x][y].f = Knoten[x][y].g + Knoten[x][y].h;

			}

			else if (x >= *startxobstacle && x <= *endxobstacle && y >= *startyobstacle && y <= *endyobstacle) {

				Knoten[x][y].walkable = false;
				Knoten[x][y].open = false;
				Knoten[x][y].closed = false;
				Knoten[x][y].parentx = x;
				Knoten[x][y].parenty = y;
				Knoten[x][y].g = 0;
				Knoten[x][y].h = 0;
				Knoten[x][y].f = 0;


			}

			else {
				Knoten[x][y].walkable = true;
				Knoten[x][y].open = false;
				Knoten[x][y].closed = false;
				Knoten[x][y].parentx = 0;
				Knoten[x][y].parenty = 0;
				Knoten[x][y].g = 0;
				Knoten[x][y].h = sqrt((x - *endx) * (x - *endx) + (y - *endy) * (y - *endy));
				Knoten[x][y].f = Knoten[x][y].g + Knoten[x][y].h;

				
			}

		}
	}
		
};

void Nodes::updateOpenList() {

	int t = 0;
	int i = 0;
	int j = 0;

	for (i = 0; i < 160; i++) {

		for (j = 0; j < 120; j++) {

			if (Knoten[i][j].open == true && Knoten[i][j].closed == false)
			{
				OpenList[t].f = Knoten[i][j].f;
				OpenList[t].x = i;
				OpenList[t].y = j;
				t++;
			}
		}
	}
};

void Nodes::bSortOpenList()
{
	int z = 0;
	int r = 0;
	int t = 0;
	int swap = 1;

	int i = 0;

	while (swap != 0)
	{
		swap = 0;
		i = 0;

		while (OpenList[i].f >= 0) {

			if (OpenList[i].f > OpenList[i + 1].f && OpenList[i + 1].f > 0)
			{
				z = OpenList[i + 1].f;
				OpenList[i + 1].f = OpenList[i].f;
				OpenList[i].f = z;

				t = OpenList[i + 1].x;
				OpenList[i + 1].x = OpenList[i].x;
				OpenList[i].x = t;

				r = OpenList[i + 1].y;
				OpenList[i + 1].y = OpenList[i].y;
				OpenList[i].y = r;
				swap++;
			}
			i++;
		}
		
	
	}

};

void Nodes::findpath(int* startx, int*  starty, int* endx, int* endy, cv::Mat3b& mat2DMap)
{
	int x = 0;
	int y = 0;
	int dx;
	int dy;
	int currentx = *startx;
	int currenty = *starty;
	int lowestf = 10000;

	while (Knoten[currentx][currenty].f != lowestf || currentx != *endx || currenty != *endy)
	{
		for (dx = -1; dx <= 1; dx++)
		{
			for (dy = -1; dy <= 1; dy++)
			{
				
				
					if (dx == 0 || dy == 0)
					{
						Knoten[currentx + dx][currenty + dy].g = Knoten[currentx][currenty].g + 10;
					}

					else
					{
						Knoten[currentx + dx][currenty + dy].g = Knoten[currentx][currenty].g + 14;
					}

					if (Knoten[currentx + dx][currenty + dy].walkable && !Knoten[currentx + dx][currenty + dy].open)
					{
						Knoten[currentx + dx][currenty + dy].open = true;
						Knoten[currentx + dx][currenty + dy].parentx = currentx;
						Knoten[currentx + dx][currenty + dy].parenty = currenty;
						Knoten[currentx + dx][currenty + dy].f = Knoten[currentx + dx][currenty + dy].g + Knoten[currentx + dx][currenty + dy].h;

					}

					else if (Knoten[currentx + dx][currenty + dy].open)
					{
						float help = 0;
						help = Knoten[currentx + dx][currenty + dy].f;
						float compare = Knoten[currentx + dx][currenty + dy].g + Knoten[currentx + dx][currenty + dy].h;

						if (compare < help && dx != 0 && dy != 0)
						{
							Knoten[currentx + dx][currenty + dy].f = compare;
							Knoten[currentx + dx][currenty + dy].parentx = currentx;
							Knoten[currentx + dx][currenty + dy].parenty = currenty;
						}


					}
				

				


			}
		}
		Knoten[currentx][currenty].closed = true;
		updateOpenList();
		bSortOpenList();
	
		currentx = OpenList[0].x;
		currenty = OpenList[0].y;
		lowestf = OpenList[0].f;

	}


	x = *endx;
	y = *endy;
	while (x != *startx && y != *starty) {


		line(mat2DMap, cv::Point2f(Knoten[x][y].parentx * 8, Knoten[x][y].parenty * 8), cv::Point2f(Knoten[x+1][y+1].parentx * 8, Knoten[x+1][y+1].parenty * 8), cv::Scalar(255, 20, 147), 4);

		x = Knoten[x][y].parentx;
		y = Knoten[x][y].parenty;

	}

	
}



