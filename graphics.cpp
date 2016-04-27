// OpenGL/GLUT starter kit for Windows 7 and Visual Studio 2010
// Created spring, 2011
//
// This is a starting point for OpenGl applications.
// Add code to the "display" function below, or otherwise
// modify this file to get your desired results.
//
// For the first assignment, add this file to an empty Windows Console project
//		and then compile and run it as is.
// NOTE: You should also have glut.h,
// glut32.dll, and glut32.lib in the directory of your project.
// OR, see GlutDirectories.txt for a better place to put them.
#define _CRT_SECURE_NO_WARNINGS
#include <cstdlib>
#include <ctype.h>
#include <conio.h>
#include <random>
#include <cmath>
#include <cstring>
#include <iostream>
#include <ctime>
using namespace std;
#include "glut.h"
#include "maze1.h"
#include "rat.h"
#include "graphics1.h"

// Global Variables (Only what you need!)
RAT_MODE gCurrentView = TOP_VIEW;
double screen_x = 700;
double screen_y = 500;
double gWorldBuffer = .5;
bool gLeftButtonDown = false;
bool gMiddleButtonDown = false;
bool gRightButtonDown, gLeftMove, gRightMove, gJumpU,gJumpD = false;
double gJumpHeight = 0;
double DT = 0.0001;
Maze gMaze;
Rat gRat;
// Functions that draw basic primitives
const int num_textures = 4;
static GLuint texName[num_textures];

// tga image loader code.
#include "tga.h"
gliGenericImage *readTgaImage(char *filename)
{
	FILE *file;
	gliGenericImage *image;
	file = fopen(filename, "rb");
	if (file == NULL) {
		printf("Error: could not open \"%s\"\n", filename);
		return NULL;
	}
	image = gliReadTGA(file, filename);
	fclose(file);
	if (image == NULL) {
		printf("Error: could not decode file format of \"%s\"\n", filename);
		return NULL;
	}
	return image;
}


// Generic image loader code.
gliGenericImage *readImage(char *filename)
{
	size_t size = strlen(filename);
	if (toupper(filename[size - 3]) == 'T' && toupper(filename[size - 2]) == 'G' && toupper(filename[size - 1]) == 'A')
	{
		gliGenericImage * result = readTgaImage(filename);
		if (!result)
		{
			cerr << "Error opening " << filename << endl;
			_getch();
			exit(1);
		}
		return result;
	}
	else
	{
		cerr << "Unknown Filetype!\n";
		_getch();
		exit(1);
	}
}

// This resets the edges of the texture image to a given "border color".
// You must call this for clamped images that do not take up the whole polygon.
// Otherwise, the texture edges will smear outward across the rest
// of the polygon.
void SetBorder(gliGenericImage * image)
{
	// set a border color.
	unsigned int border_r = 50;
	unsigned int border_g = 50;
	unsigned int border_b = 255;
	int x, y;
	y = 0;
	for (x = 0; x<image->width; x++)
	{
		image->pixels[3 * (y*image->width + x) + 0] = border_r;
		image->pixels[3 * (y*image->width + x) + 1] = border_g;
		image->pixels[3 * (y*image->width + x) + 2] = border_b;
	}
	y = 1;
	for (x = 0; x<image->width; x++)
	{
		image->pixels[3 * (y*image->width + x) + 0] = border_r;
		image->pixels[3 * (y*image->width + x) + 1] = border_g;
		image->pixels[3 * (y*image->width + x) + 2] = border_b;
	}
	y = image->height - 1;
	for (x = 0; x<image->width; x++)
	{
		image->pixels[3 * (y*image->width + x) + 0] = border_r;
		image->pixels[3 * (y*image->width + x) + 1] = border_g;
		image->pixels[3 * (y*image->width + x) + 2] = border_b;
	}
	y = image->height - 2;
	for (x = 0; x<image->width; x++)
	{
		image->pixels[3 * (y*image->width + x) + 0] = border_r;
		image->pixels[3 * (y*image->width + x) + 1] = border_g;
		image->pixels[3 * (y*image->width + x) + 2] = border_b;
	}

	x = 0;
	for (y = 0; y<image->height; y++)
	{
		image->pixels[3 * (y*image->width + x) + 0] = border_r;
		image->pixels[3 * (y*image->width + x) + 1] = border_g;
		image->pixels[3 * (y*image->width + x) + 2] = border_b;
	}
	x = 1;
	for (y = 0; y<image->height; y++)
	{
		image->pixels[3 * (y*image->width + x) + 0] = border_r;
		image->pixels[3 * (y*image->width + x) + 1] = border_g;
		image->pixels[3 * (y*image->width + x) + 2] = border_b;
	}
	x = image->width - 1;
	for (y = 0; y<image->height; y++)
	{
		int index = 3 * (y*image->width + x);
		image->pixels[index + 0] = border_r;
		image->pixels[index + 1] = border_g;
		image->pixels[index + 2] = border_b;
	}
	x = image->width - 2;
	for (y = 0; y<image->height; y++)
	{
		int index = 3 * (y*image->width + x);
		image->pixels[index + 0] = border_r;
		image->pixels[index + 1] = border_g;
		image->pixels[index + 2] = border_b;
	}
}

// Return true if h is a perfect power of 2 (up to 4096)
bool PowerOf2(int h)
{
	if (h != 2 && h != 4 && h != 8 && h != 16 && h != 32 && h != 64 && h != 128 &&
		h != 256 && h != 512 && h != 1024 && h != 2048 && h != 4096)
		return false;
	else
		return true;
}
void DrawCircle(double x1, double y1, double z, double radius)
{
	glBegin(GL_POLYGON);
	for (int i = 0; i<32; i++)
	{
		double theta = (double)i / 32.0 * 2.0 * 3.1415926;
		double x = x1 + radius * cos(theta);
		double y = y1 + radius * sin(theta);
		glVertex3d(x, y, z);
	}
	glEnd();
}
void DrawRectangle(double x1, double y1, double x2, double y2, double z)
{
	glBegin(GL_QUADS);
	glVertex3d(x1, y1,z);
	glVertex3d(x2, y1,z);
	glVertex3d(x2, y2,z);
	glVertex3d(x1, y2,z);
	glEnd();
}

void DrawLine(double x1, double y1, double x2, double y2, double z)
{
	glBegin(GL_LINES);
	glVertex3d(x1, y1,z);
	glVertex3d(x2, y2,z);
	glEnd();
}

void DrawTriangle(double x1, double y1, double x2, double y2, double x3, double y3, double z)
{
	glBegin(GL_TRIANGLES);
	glVertex3d(x1, y1,z);
	glVertex3d(x2, y2,z);
	glVertex3d(x3, y3,z);
	glEnd();
}

// Outputs a string of text at the specified location.
void DrawText(double x, double y, char *string)
{
	void *font = GLUT_BITMAP_9_BY_15;

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	int len, i;
	glRasterPos2d(x, y);
	len = (int)strlen(string);
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(font, string[i]);
	}

	glDisable(GL_BLEND);
}


//
// GLUT callback functions
//

// This callback function gets called by the Glut
// system whenever it decides things need to be redrawn.
void display(void)
{
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	if (gCurrentView == PERSPECTIVE_VIEW) {
		if (gJumpU && gJumpHeight < 1.5) {
			gJumpHeight += .005;
		}
		else gJumpU = false;
		if (!gJumpU && gJumpHeight > 0) {
			gJumpHeight -= .005;
		}
		//xyz of perspective, xyz of rat maze, xyz of camera
		gluLookAt(M/3, -N/3, M,   M/2, N/2, 0,   0,0,1);
	}
	if (gCurrentView == RAT_VIEW) {
		double radians = gRat.GetRotation() / 180 * 3.141592654;
		double dx = cos(radians);
		double dy = sin(radians);
		if (gJumpU && gJumpHeight < 1.5) {
			gJumpHeight += .005;
		}
		else gJumpU = false;
		if (!gJumpU && gJumpHeight > 0) {
			gJumpHeight -= .005;
		}
		//xyz of perspective,					xyz of rat maze,               xyz of camera
		gluLookAt(gRat.GetX(), gRat.GetY(), .1 + gJumpHeight, (gRat.GetX()+dx), (gRat.GetY()+dy), 0,         0, 0, 1);
	}
	glColor3d(0, .5, 1);
	gMaze.Draw(texName);
	while (!gMaze.IsLegal(gRat.GetX()+ gRat.GetDX(), gRat.GetY()+ gRat.GetDY(), gRat.GetRadius(), gJumpHeight) && gJumpHeight > 1) {
		gRat.SetX(gRat.GetX() + gRat.GetDX());
		gRat.SetY(gRat.GetY() + gRat.GetDY());
		glutPostRedisplay();
	}
	if (gLeftButtonDown == true) {
		gRat.TurnLeft();
	}
	if (gRightButtonDown == true) {
		gRat.TurnRight();
	}
	if (gMiddleButtonDown == true) {
		gRat.Move(DT, gMaze, gRightMove, gLeftMove, gJumpHeight);
	}
	if (gRightMove == true) {
		gRat.Move(DT, gMaze, gRightMove, gLeftMove, gJumpHeight);
		gRightMove = false;
	}
	if (gLeftMove == true) {
		gRat.Move(DT, gMaze, gRightMove, gLeftMove, gJumpHeight);
		gLeftMove = false;
	}
	glPushMatrix();
	glTranslated(gRat.GetX(), gRat.GetY(), 0);
	glRotated(gRat.GetRotation(), 0, 0, 1);
	glScaled(.7, .7, 1.0);
	if(gCurrentView != RAT_VIEW) gRat.Draw(gJumpHeight);
	glPopMatrix();
	glutSwapBuffers();
	glutPostRedisplay();
}


// This callback function gets called by the Glut
// system whenever a key is pressed.
void setOrthoProjection() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-gWorldBuffer, M + gWorldBuffer, -gWorldBuffer, N + gWorldBuffer);
	glMatrixMode(GL_MODELVIEW);
}
void setPerspectiveProjection() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//field of view(angle in degress), aspect ratio, near, far
	double fov = 90;
	double aspect = (float)screen_y / screen_x;
	double far = M + (N * 5.0);
	double near = 0.1;
	gluPerspective(fov, aspect, near, far);
	glMatrixMode(GL_MODELVIEW);
}
void keyboard(unsigned char c, int x, int y)
{
	switch (c)
	{
	case 27: // escape character means to quit the program
		exit(0);
		break;
	case 'b':
		// do something when 'b' character is hit.
		break;
	case 'w':
			gMiddleButtonDown =!gMiddleButtonDown;
			break;
	case 'a':
		gLeftButtonDown = true;
		break;
	case 'd':
		gRightButtonDown = true;
		break;
	case 't':
		setOrthoProjection();
		gCurrentView = TOP_VIEW;
		break;
	case 'p':
		setPerspectiveProjection();
		gCurrentView = PERSPECTIVE_VIEW;
		break;
	case 'r':
		setPerspectiveProjection();
		gCurrentView = RAT_VIEW;
		break;
	case 'z':
		gLeftMove = true;
		
		break;
	case 'x':
		gRightMove = true;
		
		break;
	case 'j':
		gJumpU = true;

		break;
	default:
		return; // if we don't care, return without glutPostRedisplay()
	}

	glutPostRedisplay();
}

void handleSpecialKeypress(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT:
		gLeftButtonDown = true;
		break;
	case GLUT_KEY_RIGHT:
		gRightButtonDown = true;
		break;
	case GLUT_KEY_UP:
		gMiddleButtonDown = true;
		break;
	}
}
void handleSpecialKeyReleased(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT:
		gLeftButtonDown = false;
		break;
	case GLUT_KEY_RIGHT:
		gRightButtonDown = false;
		break;
	case GLUT_KEY_UP:
		gMiddleButtonDown = false;
		break;
	}
}
// This callback function gets called by the Glut
// system whenever the window is resized by the user.
void reshape(int w, int h)
{
	// Reset our global variables to the new width and height.
	screen_x = w;
	screen_y = h;

	// Set the pixel resolution of the final picture (Screen coordinates).
	glViewport(0, 0, w, h);

	// Set the projection mode to 2D orthographic, and set the world coordinates:
	if (gCurrentView == TOP_VIEW) {
		setOrthoProjection();
	}
	else { setPerspectiveProjection(); }

}
using namespace std;
// This callback function gets called by the Glut
// system whenever any mouse button goes up or down.
void mouse(int mouse_button, int state, int x, int y)
{
	if (mouse_button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		gLeftButtonDown = true;
	}
	if (mouse_button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		gLeftButtonDown = false;
	}
	if (mouse_button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN)
	{
		gMiddleButtonDown = true;
	}
	if (mouse_button == GLUT_MIDDLE_BUTTON && state == GLUT_UP)
	{
		gMiddleButtonDown = false;
	}
	if (mouse_button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		gRightButtonDown = true;
	}
	if (mouse_button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
	{
		gRightButtonDown = false;
	}
	glutPostRedisplay();
}

// Your initialization code goes here.
void InitializeMyStuff()
{
	gliGenericImage *image[num_textures];
	int n = 0;
	image[n++] = readImage("concrete.tga");
	image[n++] = readImage("fence.tga");
	image[n++] = readImage("brick_wall.tga");
	image[n++] = readImage("sky.tga");
	//image[n++] = readImage("Seattle.tga");
	if (n != num_textures)
	{
		printf("Error: Wrong number of textures\n");
		_getch();
		exit(1);;
	}

	glGenTextures(num_textures, texName);

	for (int i = 0; i<num_textures; i++)
	{
		glBindTexture(GL_TEXTURE_2D, texName[i]);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		int repeats = false;
		int needs_border = false; // Needed if clamping and not filling the whole polygon.
		if (i == 0 || i == 1) repeats = true;
		if (i == 2) needs_border = true;
		if (repeats)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		}
		if(needs_border){
			// set a border.
			SetBorder(image[i]);
		}

		bool mipmaps = false;
		if (!PowerOf2(image[i]->height) || !PowerOf2(image[i]->width))
		{
			// WARNING: Images that do not have width and height as 
			// powers of 2 MUST use mipmaps.
			mipmaps = true;
		}

		if (mipmaps)
		{
			gluBuild2DMipmaps(GL_TEXTURE_2D, image[i]->components,
				image[i]->width, image[i]->height,
				image[i]->format, GL_UNSIGNED_BYTE, image[i]->pixels);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
				//GL_LINEAR_MIPMAP_LINEAR);
				GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
				//GL_LINEAR);
				GL_NEAREST);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, image[i]->components,
				image[i]->width, image[i]->height, 0,
				image[i]->format, GL_UNSIGNED_BYTE, image[i]->pixels);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
	}
	gMaze.Init();
	double gRatX = double(gMaze.GetStartI()) + gWorldBuffer;
	double gRatY = gWorldBuffer;
	gRat.SetX(gRatX);
	gRat.SetY(gRatY);
}

void motion(int x, int y) {
	/*
	y = screen_y - y;
	double ratio = (double)x / screen_x;
	double wx = -.5 + ratio * (M+1.0);
	ratio = (double)y / screen_y;
	double wy = -.5 + ratio * (N+1.0);
	*/
	
	glutPostRedisplay();
	
}
int main(int argc, char **argv)
{
	srand(time(0));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(screen_x, screen_y);
	glutInitWindowPosition(50, 50);

	int fullscreen = 0;
	if (fullscreen)
	{
		glutGameModeString("800x600:32");
		glutEnterGameMode();
	}
	else
	{
		glutCreateWindow("This appears in the title bar");
	}

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutSpecialFunc(handleSpecialKeypress);
	glutSpecialUpFunc(handleSpecialKeyReleased);
	glColor3d(0, 0, 0); // forground color
	glClearColor(1, 1, 1, 0); // background color
	InitializeMyStuff();

	glutMainLoop();

	return 0;
}
