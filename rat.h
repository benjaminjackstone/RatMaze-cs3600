#ifndef _RAT_H_
#define _RAT_H_
#include "maze1.h"
class Rat {
public:
	Rat();
	//Rat(double x, double y, double radius, double rotation);
	void Draw(double z);
	void TurnLeft();
	void TurnRight();
	void Move(double dT, Maze &maze, bool left, bool right, double z);
	double GetRotation();
	double GetX() { return mX; };
	double GetY() { return mY; };
	double GetRadius() { return mRadius; };
	void SetX(double x) { mX = x; };
	void SetY(double y) { mY = y; };
	double GetDX() { return mDx; };
	double GetDY() { return mDy; };
	double GetSpeed() { return mSpeed; };
	void SetDX(double Dx) { mDx = Dx; };
	void SetSpeed(double s) { mSpeed = s; };
	void SetDY(double Dy) { mDy = Dy; };
private:
	double mX, mY, mDx, mDy, mR, mG, mB, mA, mFx, mFy, mRotation, mRadius, mSpeed;
};
#endif