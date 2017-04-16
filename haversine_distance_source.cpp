#include <cmath>
#include "data_structures.hpp"
#define PI 3.14159265358979323846
#define EARTH_RADIUS_IN_METERS 6372797.560856
//#define earthRadiusKm 6371.0


// This function converts decimal degrees to radians
double deg2rad(double deg) {
  return (deg * PI / 180);
}


// Calculate haversine distance between two points in metres
double distanceEarth(Point a, Point b) 
{
  double u, v;
  Point a_r, b_r;
  a_r.latitude = deg2rad(a.latitude);
  a_r.longitude = deg2rad(a.longitude);
  b_r.latitude = deg2rad(b.latitude);
  b_r.longitude = deg2rad(b.longitude);
  u = sin((b_r.latitude- a_r.latitude)/2);
  v = sin((b_r.longitude - a_r.longitude)/2);
  return 2.0 * EARTH_RADIUS_IN_METERS * asin(sqrt(u * u + cos(a_r.latitude) * cos(b_r.latitude) * v * v));
}

/*int main()
{
    Point a, b;
    a.latitude = 38.898556;
    a.longitude = -77.037852;
    b.latitude = 38.897147;
    b.longitude = -77.043934;
    cout << distanceEarth(a, b) << endl;

    return 0;
}*/