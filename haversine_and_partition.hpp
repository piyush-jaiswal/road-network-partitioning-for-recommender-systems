#ifndef haversine_and_partition_H
#define haversine_and_partition_H

#include "data_structures.hpp"

extern double distanceEarth(Point, Point);
extern bool liesInPartition(Partition, Point);
extern float getfactorLat(int);
extern float getfactorLong(int);

#endif // !haversine_and_partition_H