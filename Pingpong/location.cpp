#define _USE_MATH_DEFINES
#include <math.h>

#include "location.h"

location::location()
{
	x = 0;
	y = 0;
}

location::location(double x0, double y0)
{
	x = x0;
	y = y0;
}

double location::GetDistance(location loc2)
{
	return sqrt(pow(x - loc2.x, 2) + pow(y - loc2.y, 2));
}

double location::GetFacingAngle(location loc2)
{
	if (GetDistance(loc2) == 0) return 0;
	return atan2(loc2.y - y, loc2.x - x);
}

bool location::InRect(double left, double top, double right, double bottom)
{
	return (x >= left && x < right &&y >= top && y < bottom);
}

