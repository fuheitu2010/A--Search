#include <iostream>
#include <iomanip>
#include <queue>
#include <string>
#include <math.h>
#include <ctime>
#include<fstream>
#include<time.h>
using namespace std;


int m = 101;
int n = 101;
int map[101][101] = { 0 };
int close[101][101] = { 0 };
int open[101][101] = { 0 };
int dmap[101][101] = { 0 };

int dx[4] = { 1, 0, -1, 0 };
int dy[4] = { 0, 1, 0, -1 };

int goalx, goaly, startx, starty = 0;
clock_t start, finish;

double duration;

class node
{
public:
	int xPos;
	int yPos;
	int gvalue;   //g
	int fvalue;  //f
	node(int xp, int yp, int g, int f)
	{
		xPos = xp;
		yPos = yp;
		gvalue = g;
		fvalue = f;
	}

	void calculate_fvalue(int gx, int gy)
	{
		int xd, yd, hvalue;

		xd = gx - xPos;
		yd = gy - yPos;
		hvalue = abs(xd) + abs(yd);

		fvalue = gvalue + hvalue;     //Calculat fvalue
	}
};

bool operator<(const node & a, const node & b)
{
	return a.fvalue > b.fvalue;
}

bool computepath(int xFinish, int yFinish, int xStart, int yStart)
{
	priority_queue<node> openlist; // list of open (not-yet-tried) nodes
	node* current;
	node* sub;
	int i, x, y, xdx, ydy;


	current = new node(xStart, yStart, 0, 0);
	current->calculate_fvalue(xFinish, yFinish);
	openlist.push(*current);
	open[xStart][yStart] = current->fvalue; // mark it on the open nodes map


	while (openlist.empty() == false)         //A*search
	{
		current = new node(openlist.top().xPos, openlist.top().yPos, openlist.top().gvalue, openlist.top().fvalue);

		x = current->xPos;
		y = current->yPos;

		openlist.pop(); // remove the node from the open list

		if (openlist.empty() == false)
		{
			if (current->fvalue == openlist.top().fvalue)
			{
				if (current->gvalue >= openlist.top().gvalue)   //compare the gvalue 
				{
					x = current->xPos;
					y = current->yPos;
				}
				else
				{
					x = openlist.top().xPos;
					y = openlist.top().yPos;
					openlist.pop();
					openlist.push(*current);
				}
			}
		}
		open[x][y] = 0;
		close[x][y] = 1;

		if (x == xFinish && y == yFinish)     //Find route, return true
		{
			delete current;
			while (!openlist.empty())
				openlist.pop();
			return true;
		}

		for (i = 0; i<4; i++)             //Generate subnoe of current node
		{
			xdx = x + dx[i]; ydy = y + dy[i];

			if ((xdx>0) & (xdx<n - 1) & (ydy>0) & (ydy<m - 1) &(map[xdx][ydy] != 1)& (close[xdx][ydy] != 1))
			{
				sub = new node(xdx, ydy, current->gvalue, current->fvalue);
				sub->gvalue = sub->gvalue + 1;
				sub->calculate_fvalue(xFinish, yFinish);

				if (open[xdx][ydy] == 0)      // if not in the open list then add into that
				{
					open[xdx][ydy] = sub->fvalue;
					openlist.push(*sub);
					dmap[xdx][ydy] = (i + 2) % 4;
				}
				else if (open[xdx][ydy]>sub->fvalue)
				{
					open[xdx][ydy] = sub->fvalue;   //update fvalue
					dmap[xdx][ydy] = (i + 2) % 4;
					openlist.push(*sub);
				}
				else
					delete sub;
			}
		}
		delete current;
	}
	return false; // no route found
}

void set_map()         //function to set map
{
	ifstream file;
	file.open("a.txt", ios::in);
	for (int i = 0; i < 101; i++)
	{
		for (int j = 0; j < 101; j++)
		{
			file >> map[i][j];
		}
	}
	file.close();
}

void set_start_goal()    //Randomly choose start point and end point
{
	int x, y;
	srand(time(0));
	x = rand() % 101;
	y = rand() % 101;
	while (map[x][y] == 1)
	{
		x = rand() % 101;
		y = rand() % 101;
	}
	startx = x;
	starty = y;
	x = rand() % 101;
	y = rand() % 101;
	while ((x == startx)&(y == starty) | (map[x][y] == 1) | (abs(y - starty)>40))
	{
		x = rand() % 101;
		y = rand() % 101;
	}
	goalx = x;
	goaly = y;
}

void input_start_goal()
{
	cout << "Please input the x of start point:";
	cin >> startx;
	cout << "Please input the y of start point:";
	cin >> starty;
	while (map[startx][starty] == 1)
	{
		cout << "This is a wall, please input again!" << endl;
		cout << "Please input the x of start point:";
		cin >> startx;
		cout << "Please input the y of start point:";
		cin >> starty;
	}

	cout << "Please input the x of goal point:";
	cin >> goalx;
	cout << "Please input the y of goal point:";
	cin >> goaly;
	while ((goalx == startx)&(goaly == starty) | (map[goalx][goaly] == 1))
	{
		cout << "This is a wall, please input again!" << endl;
		cout << "Please input the x of goal point:";
		cin >> goalx;
		cout << "Please input the y of goal point:";
		cin >> goaly;
	}
}

int main()
{
	int p1, p2;
	int j = 0;
	bool find;
	set_map();
	//set_start_goal();
	input_start_goal();

	cout << "Start point: " << startx << "," << starty << endl;
	cout << "Goal point: " << goalx << "," << goaly << endl;
	// get the route

	start = clock();
	find = computepath(startx, starty, goalx, goaly);    // compute path
	finish = clock();

	duration = (double)(finish - 1.1*start) / CLOCKS_PER_SEC;

	cout << "The compute run time is:" << duration << "seconds" << endl;
	if (find == false)
	{
		cout << "The destination cannot be reached" << endl;
	}
	else
	{
		p1 = startx;
		p2 = starty;
		while (!(p1 == goalx && p2 == goaly))
		{
			map[p1][p2] = 3;
			j = dmap[p1][p2];
			p1 += dx[j];
			p2 += dy[j];
		}
		map[goalx][goaly] = 4;
		map[startx][starty] = 2;

		for (int i = 0; i < 101; i++)
		{
			for (int j = 0; j < 101; j++)
			{
				if (map[i][j] == 2)
					cout << "S";              //mark the start point
				else if (map[i][j] == 3)
					cout << "*";                //mark the route
				else if (map[i][j] == 4)
					cout << "G";           //mark the finish point
				else
					cout << map[i][j];
			}
			cout << endl;
		}
	}
	return(0);
}
