#include <cstdlib>
#include <vector>
#include <iostream>
#include <string>
using namespace std;
#include "glut.h"
#include "maze1.h"
#include "graphics1.h"
extern RAT_MODE gCurrentView;

Cell::Cell()
{
	mLeft = mBottom = mRight = mTop = true;
	mVisited = false;
}

void Cell::SetBottom(bool state)
{
	mBottom = state;
}

void Cell::SetLeft(bool state)
{
	mLeft = state;
}
void Cell::SetTop(bool state)
{
	mTop = state;
}

void Cell::SetRight(bool state)
{
	mRight = state;
}
bool Cell::GetTop() {
	return mTop;
}
bool Cell::GetRight() {
	return mRight;
}
bool Cell::GetBottom() {
	return mBottom;
}
bool Cell::GetLeft() {
	return mLeft;
}
bool Cell::GetVisited() {
	return mVisited;
}
void Cell::SetVisited(bool state)
{
	mVisited = state;
}
void Cell::Draw(int i, int j, GLuint * texName)
{
	int x = i;
	int y = j;
	if (gCurrentView == RAT_VIEW) {
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texName[3]);
		static double h = 0.0;
		glBegin(GL_QUADS);
		glTexCoord2d(0, h); glVertex3d(-M, -N, 2);
		glTexCoord2d(M / (M - 2), h); glVertex3d(M*1.5, -N, 2);
		glTexCoord2d(M / (M - 2), h + N / (N - 2)); glVertex3d(M*1.5, N*1.5, 2);
		glTexCoord2d(0, h + N / (N - 2)); glVertex3d(-M, N*1.5, 2);
		glEnd();
		h += .00000007;
		glDisable(GL_TEXTURE_2D);

	}
	if (gCurrentView != TOP_VIEW) {
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texName[0]);
		glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex3d(0, 0, 0);
		glTexCoord2f(M, 0); glVertex3d(0, N, 0);
		glTexCoord2f(M, N); glVertex3d(M, N, 0);
		glTexCoord2f(0, N); glVertex3d(M, 0, 0);
		glEnd();
		glDisable(GL_TEXTURE_2D);
	}
	if (mBottom)
	{

		if (gCurrentView == TOP_VIEW) {
			DrawLine(i, j, i + 1, j, 0);
		}
		else {
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, texName[1]);
			glBegin(GL_QUADS);
			glTexCoord2d(0, 0); glVertex3f(x, y, 0);
			glTexCoord2d(1, 0); glVertex3f(x + 1, y, 0);
			glTexCoord2d(1, 1); glVertex3f(x + 1, y, 1);
			glTexCoord2d(0, 1); glVertex3f(x, y, 1);
			glEnd();
			glDisable(GL_TEXTURE_2D);
		}
	}
	if (mTop)
	{
		if (gCurrentView == TOP_VIEW) {
			DrawLine(i, j + 1, i + 1, j + 1, 0);
		}
		else{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, texName[1]);
			glBegin(GL_QUADS);
			glTexCoord2d(0, 0); glVertex3f(x, y + 1, 0);
			glTexCoord2d(1, 0); glVertex3f(x + 1, y + 1, 0);
			glTexCoord2d(1, 1); glVertex3f(x + 1, y + 1, 1);
			glTexCoord2d(0, 1); glVertex3f(x, y + 1, 1);
			glEnd();
			glDisable(GL_TEXTURE_2D);
		}
	}
	if (mLeft)
	{
		if (gCurrentView == TOP_VIEW) {
			DrawLine(i, j, i, j + 1, 0);
		}
		else {
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, texName[2]);
			glBegin(GL_QUADS);
			glTexCoord2d(0, 0); glVertex3f(x, y, 0);
			glTexCoord2d(1,0); glVertex3f(x, y + 1, 0);
			glTexCoord2d(1,1); glVertex3f(x, y + 1, 1);
			glTexCoord2d(0,1); glVertex3f(x, y, 1);
			glEnd();
			glDisable(GL_TEXTURE_2D);
		}

	}
	if (mRight)
	{
		if (gCurrentView == TOP_VIEW) {
			DrawLine(i + 1, j, i + 1, j + 1, 0);
		}
		else{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, texName[2]);
			glBegin(GL_QUADS);
			glTexCoord2d(0, 0); glVertex3f(x + 1, y, 0);
			glTexCoord2d(1, 0); glVertex3f(x + 1, y + 1, 0);
			glTexCoord2d(1, 1); glVertex3f(x + 1, y + 1, 1);
			glTexCoord2d(0, 1); glVertex3f(x + 1, y, 1);
			glEnd();
			glDisable(GL_TEXTURE_2D);

		}
	}
}

Maze::Maze()
{
}

void Maze::Init()
{
	// Find a start cell:
	mStartI = rand() % M;
	mEndI = rand() % M;
	mCells[mStartI][0].SetBottom(false);
	mCells[mEndI][N - 1].SetTop(false);
	mCells[mStartI][0].SetVisited(true);
	RemoveWallsR(mStartI, 0);
}
bool Maze::IsLegal(double newx, double newy, double radius, double z) {
	double h = 1;
	//if (z > h) return true;
	int cellx = int(newx);
	int celly = int(newy);
	double offsetx = newx - cellx;
	double offsety = newy - celly;
	if (mCells[cellx][celly].GetLeft() && (offsetx - radius <= 0)) {
		return false;
	}
	if (mCells[cellx][celly].GetRight() && (offsetx + radius >= 1.0)) {
		return false;
	}
	if (mCells[cellx][celly].GetBottom() && (offsety - radius <= 0.0)) {
		return false;
	}
	if (mCells[cellx][celly].GetTop() && (offsety + radius >= 1.0)) {
		return false;
	}
	if (offsetx + radius >= 1 && offsety - radius <= 0) {
		return false;
	}
	if (offsetx - radius <= 0 && offsety - radius <= 0) {
		return false;
	}
	if (offsetx + radius >= 1 && offsety + radius >= 1.0) {
		return false;
	}
	if (offsetx - radius <= 0 && offsety + radius >= 1.0) {
		return false;
	}
	return true;
}
void Maze::RemoveWallsR(int i, int j)
{
	while (true) {
		std::vector<string> choices;
		if (i == 0) {
			if (j == 0) {
				if (mCells[i + 1][j].GetLeft() && !mCells[i + 1][j].GetVisited()) {
					choices.push_back("right");
				}
				if (mCells[i][j + 1].GetBottom() && !mCells[i][j + 1].GetVisited()) {
					choices.push_back("top");
				}
			}
			else if (j == N - 1) {
				if (mCells[i + 1][j].GetLeft() && !mCells[i + 1][j].GetVisited()) {
					choices.push_back("right");
				}
				if (mCells[i][j - 1].GetTop() && !mCells[i][j - 1].GetVisited()) {
					choices.push_back("bottom");
				}
			}
			else {
				if (mCells[i + 1][j].GetLeft() && !mCells[i + 1][j].GetVisited()) {
					choices.push_back("right");
				}
				if (mCells[i][j - 1].GetTop() && !mCells[i][j - 1].GetVisited()) {
					choices.push_back("bottom");
				}
				if (mCells[i][j + 1].GetBottom() && !mCells[i][j - 1].GetVisited()) {
					choices.push_back("top");
				}
			}
		}
		if (j == 0 && i != 0) {
			if (i == M - 1) {
				if (mCells[i - 1][j].GetRight() && !mCells[i - 1][j].GetVisited()) {
					choices.push_back("left");
				}
				if (mCells[i][j + 1].GetBottom() && !mCells[i][j + 1].GetVisited()) {
					choices.push_back("top");
				}
			}
			else {
				if (mCells[i - 1][j].GetRight() && !mCells[i - 1][j].GetVisited()) {
					choices.push_back("left");
				}
				if (mCells[i + 1][j].GetLeft() && !mCells[i + 1][j].GetVisited()) {
					choices.push_back("right");
				}
				if (mCells[i][j + 1].GetBottom() && !mCells[i][j - 1].GetVisited()) {
					choices.push_back("top");
				}
			}
		}
		if (i == M - 1 && j != 0) {
			if (j == N - 1) {
				if (mCells[i][j - 1].GetTop() && !mCells[i][j - 1].GetVisited()) {
					choices.push_back("bottom");
				}
				if (mCells[i - 1][j].GetRight() && !mCells[i - 1][j].GetVisited()) {
					choices.push_back("left");
				}
			}
			else {
				if (mCells[i - 1][j].GetRight() && !mCells[i - 1][j].GetVisited()) {
					choices.push_back("left");
				}
				if (mCells[i][j + 1].GetBottom() && !mCells[i + 1][j].GetVisited()) {
					choices.push_back("top");
				}
				if (mCells[i][j - 1].GetTop() && !mCells[i][j - 1].GetVisited()) {
					choices.push_back("bottom");
				}
			}
		}
		if (j == N - 1 && i != M - 1) {
			if (mCells[i - 1][j].GetRight() && !mCells[i - 1][j].GetVisited()) {
				choices.push_back("left");
			}
			if (mCells[i + 1][j].GetLeft() && !mCells[i + 1][j].GetVisited()) {
				choices.push_back("right");
			}
			if (mCells[i][j - 1].GetTop() && !mCells[i][j - 1].GetVisited()) {
				choices.push_back("bottom");
			}
		}
		if (i > 0 && i < M - 1 && j > 0 && j < N - 1) {
			if (mCells[i - 1][j].GetRight() && !mCells[i - 1][j].GetVisited()) {
				choices.push_back("left");
			}
			//std::cout << "all other cases" << endl;
			if (mCells[i + 1][j].GetLeft() && !mCells[i + 1][j].GetVisited()) {
				choices.push_back("right");
			}
			if (mCells[i][j - 1].GetTop() && !mCells[i][j - 1].GetVisited()) {
				choices.push_back("bottom");
			}
			if (mCells[i][j + 1].GetBottom() && !mCells[i][j + 1].GetVisited()) {
				choices.push_back("top");
			}
		}
		int size = choices.size();
		if (size < 1) {
			return;
		}
		int dir = rand() % size;
		string choice = choices[dir];
		if (choice == "top") {
			mCells[i][j].SetTop(false);
			i = i;
			j = j + 1;
			mCells[i][j].SetBottom(false);
		}
		if (choice == "bottom") {
			mCells[i][j].SetBottom(false);
			i = i;
			j = j - 1;
			mCells[i][j].SetTop(false);
		}
		if (choice == "left") {
			mCells[i][j].SetLeft(false);
			i = i - 1;
			j = j;
			mCells[i][j].SetRight(false);
		}
		if (choice == "right") {
			mCells[i][j].SetRight(false);
			i = i + 1;
			j = j;
			mCells[i][j].SetLeft(false);
		}
		mCells[i][j].SetVisited(true);
		//std::cout << size << " " << mCells[i][j].GetVisited() << " i = " << i << " j = " << j << std::endl;
		RemoveWallsR(i, j);
	}
}

void Maze::Draw(GLuint * texName)
{
	
	for (int i = 0; i < M; i++)
		for (int j = 0; j < N; j++)
		{
			mCells[i][j].Draw(i, j, texName);
		}
}
