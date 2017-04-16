#include "data_structures.hpp"
#include "partitions_and_POIs.hpp"
#include <vector>
#define TOP_LEFT_LAT 40.99
#define TOP_LEFT_LONG -74.276
#define BOUNDARY_RANGE 0.05

using namespace std;


// Returns the data structure for the bigger grid
Map* getGrid(int noOfSmallerPartitions, int datarowSize)
{
    int i;
    Map* grid = new Map[noOfSmallerPartitions];

    for(i = 0; i < noOfSmallerPartitions; i++)
        grid[i].utility = new Data[datarowSize];
    
    return grid;
}


// Returns the original partitioned grid
vector<Map> originalGrid(int noOfSmallerPartitions, int datarowSize)
{
    Map *original;

    original = getGrid(noOfSmallerPartitions, datarowSize);
    initialize_grid(original, noOfSmallerPartitions, TOP_LEFT_LAT, TOP_LEFT_LONG);
    vector<Map> initialPartition_vec (original, original + noOfSmallerPartitions);

    return initialPartition_vec;
}


// Top left shifted partitions
vector<Map> topLeftShift(int noOfSmallerPartitions, int datarowSize, float factorLat, float factorLong)
{
    float topLeftPartition_lat, topLeftPartition_long;
    Map *topLeftGrid;

    topLeftPartition_lat = TOP_LEFT_LAT + BOUNDARY_RANGE * factorLat;
    topLeftPartition_long = TOP_LEFT_LONG - BOUNDARY_RANGE * factorLong;
    topLeftGrid = getGrid(noOfSmallerPartitions, datarowSize);
    initialize_grid(topLeftGrid, noOfSmallerPartitions, topLeftPartition_lat, topLeftPartition_long);
    vector<Map> topLeftShifted_vec (topLeftGrid, topLeftGrid + noOfSmallerPartitions);

    return topLeftShifted_vec;
}


// Top right shifted partitions
vector<Map> topRightShift(int noOfSmallerPartitions, int datarowSize, float factorLat, float factorLong)
{
    float topRightPartition_lat, topRightPartition_long;
    Map *topRightGrid;

    topRightPartition_lat = TOP_LEFT_LAT + BOUNDARY_RANGE * factorLat;
    topRightPartition_long = TOP_LEFT_LONG + BOUNDARY_RANGE * factorLong;
    topRightGrid = getGrid(noOfSmallerPartitions, datarowSize);
    initialize_grid(topRightGrid, noOfSmallerPartitions, topRightPartition_lat, topRightPartition_long);
    vector<Map> topRightShifted_vec (topRightGrid, topRightGrid + noOfSmallerPartitions);

    return topRightShifted_vec;
}


// Bottom left shifted partitions
vector<Map> bottomLeftShift(int noOfSmallerPartitions, int datarowSize, float factorLat, float factorLong)
{
    float bottomLeftPartition_lat, bottomLeftPartition_long;
    Map *bottomLeftGrid;

    bottomLeftPartition_lat = TOP_LEFT_LAT - BOUNDARY_RANGE * factorLat;
    bottomLeftPartition_long = TOP_LEFT_LONG - BOUNDARY_RANGE * factorLong;
    bottomLeftGrid = getGrid(noOfSmallerPartitions, datarowSize);
    initialize_grid(bottomLeftGrid, noOfSmallerPartitions, bottomLeftPartition_lat, bottomLeftPartition_long);
    vector<Map> bottomLeftShifted_vec (bottomLeftGrid, bottomLeftGrid + noOfSmallerPartitions);

    return bottomLeftShifted_vec;
}


// Bottom right shifted partitions
vector<Map> bottomRightShift(int noOfSmallerPartitions, int datarowSize, float factorLat, float factorLong)
{
    float bottomRightPartition_lat, bottomRightPartition_long;
    Map *bottomRightGrid;

    bottomRightPartition_lat = TOP_LEFT_LAT - BOUNDARY_RANGE * factorLat;
    bottomRightPartition_long = TOP_LEFT_LONG + BOUNDARY_RANGE * factorLong;
    bottomRightGrid = getGrid(noOfSmallerPartitions, datarowSize);
    initialize_grid(bottomRightGrid, noOfSmallerPartitions, bottomRightPartition_lat, bottomRightPartition_long);
    vector<Map> bottomRightShifted_vec (bottomRightGrid, bottomRightGrid + noOfSmallerPartitions);

    return bottomRightShifted_vec;
}