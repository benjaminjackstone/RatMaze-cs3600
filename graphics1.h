#pragma once
enum RAT_MODE { TOP_VIEW, PERSPECTIVE_VIEW, RAT_VIEW };
void DrawLine(double x1, double y1, double x2, double y2, double z);
void DrawRectangle(double x1, double y1, double x2, double y2, double z);
void DrawTriangle(double x1, double y1, double x2, double y2, double x3, double y3, double z);
void DrawCircle(double x1, double y1, double z, double radius);
