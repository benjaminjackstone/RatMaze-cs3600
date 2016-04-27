#include "rat.h"
#include <math.h>
#include "glut.h"
#include "maze1.h"
#include "graphics1.h"

Rat::Rat() 
{
	mSpeed = .001;
}
/*
Rat::Rat(double x, double y, double radius, double rotation)
	:mX(x), mY(y), mRadius(radius), mRotation(rotation)
{

}
*/
void Rat::Draw(double z) {
	glColor3d(.9, .9, .5);
	//body
	DrawCircle(0, 0, z,.15);
	//head
	DrawTriangle(.08, .1, .08, -.1, .25, 0, z);
	glColor3d(0, 0, 0);
	//nose
	DrawCircle(.25, 0, z,.02);
	//tail
	DrawLine(-.15, 0, -.25, 0, z);
}
void Rat::TurnLeft() {
	mRotation += .1;

}

void Rat::TurnRight() {
	mRotation -= .1;
}

void Rat::Move(double dT, Maze &maze, bool right, bool left, double z) {
	mRadius = .2;
	double speed = mSpeed;
	//if (z > 0) speed += .001;
	double radians = mRotation / 180 * 3.141592654;
	mDx = cos(radians) * speed;
	mDy = sin(radians) * speed;
	if (right) {
		double tmp = mDx * 10;
		mDx = mDy * 10;
		mDy = -tmp;
	}
	if (left) {
		double tmp = mDy * 10;
		mDy = mDx * 10;
		mDx = -tmp;
	}
	if (maze.IsLegal(mX+ mDx, mY + mDy, mRadius, z)) {
		mX += mDx;
		mY += mDy;
	}
	else if (maze.IsLegal(mX, mY+ mDy, mRadius, z)){
		mY += mDy;
	}
	else if (maze.IsLegal(mX+ mDx, mY, mRadius, z)) {
		mX += mDx;
	}
}
double Rat::GetRotation() {
	return mRotation;
}