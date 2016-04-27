#pragma once
const int M = 6;
const int N = 5;
#include <cstdlib>
#include "Tga.h"
#include <random>
#include "tga.h"
#include "glut.h"
using namespace std;

class Cell
{
public:
	Cell();
	void Draw(int i, int j, GLuint * texname);
	void SetBottom(bool state);
	void SetTop(bool state);
	void SetVisited(bool state);
	void SetRight(bool state);
	void SetLeft(bool state);
	bool GetTop();
	bool GetLeft();
	bool GetRight();
	bool GetVisited();
	bool GetBottom();

private:
	bool mLeft, mTop, mRight, mBottom, mVisited;
};
class Maze
{
public:
	Maze();
	void Init();
	void Draw(GLuint * texName);
	Cell mCells[M][N];
	void RemoveWallsR(int i, int j);
	int GetStartI() { return mStartI; }
	bool IsLegal(double newx, double newy, double radius, double z);

private:
	//Cell mCells[M][N];
	int mStartI;
	int mEndI;
};

