#ifndef LOCATION_H
#define LOCATION_H


struct location {
	double x, y;
	location();
	location(double x0, double y0);
	double GetDistance(location loc2);
	double GetFacingAngle(location loc2);
	bool InRect(double left, double top, double right, double bottom);
};

#endif