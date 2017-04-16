#include <iostream>
#include <math.h>
#include "partitions_and_POIs.hpp"
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


int main()
{
    int noOfSmallerPartitions, totalPartitionInRow, datarowSize, i, k;
    float factorLat, factorLong, topLeftPartition_lat, topLeftPartition_long, topRightPartition_lat, topRightPartition_long, 
          bottomLeftPartition_lat, bottomLeftPartition_long, bottomRightPartition_lat, bottomRightPartition_long;
    string POICategory;
    vector<Data> dataRow;
    Point userLocation;
    Map *originalGrid, *topLeftGrid, *topRightGrid, *bottomLeftGrid, *bottomRightGrid;

    //Taking the number of boxes in initial partition by the user
    cout << "Enter the number of partitions :" << endl;
    cin >> noOfSmallerPartitions;

    //A function to take number of rows in the dataset and memory allocation to the object array.
    cout << endl << "Calculating total no. of POIs" << endl;
    datarowSize = getNoOfPOIs();
    cout << "Total POIs = " << datarowSize << endl << endl;

    // Store the POIs in our data structures
    cout << "Loading the POIs in the data structures..." << endl;
    dataRow = loadPOI_inDataStructures();
    cout << "Successfully loaded." << endl << endl;


    // Make partitions in the original grid
    cout << "Making partitions in the original grid..." << endl;
    originalGrid = getGrid(noOfSmallerPartitions, datarowSize);
    initialize_grid(originalGrid, noOfSmallerPartitions, TOP_LEFT_LAT, TOP_LEFT_LONG);
    cout << "Success." << endl;

    // Load the POIs in the partitions created in the original grid
    cout << "Loading POIs for the original grid inside the created partitions" << endl;
    vector<Map> initialPartition_vec (originalGrid, originalGrid + noOfSmallerPartitions);
    loadPoIs(dataRow, initialPartition_vec);
    cout << "Successfully loaded." << endl << endl;


    // Get factorLat and factorLong
    totalPartitionInRow = (int) sqrt(noOfSmallerPartitions);
    factorLat = getfactorLat(totalPartitionInRow);
    factorLong = getfactorLong(totalPartitionInRow);


    // Create top-left shifted grid
    cout << "Shifting partitions in the top left direction" << endl;
    topLeftPartition_lat = TOP_LEFT_LAT + BOUNDARY_RANGE * factorLat;
    topLeftPartition_long = TOP_LEFT_LONG - BOUNDARY_RANGE * factorLong;
    topLeftGrid = getGrid(noOfSmallerPartitions, datarowSize);
    initialize_grid(topLeftGrid, noOfSmallerPartitions, topLeftPartition_lat, topLeftPartition_long);
    cout << "Done." << endl;

    // Load the POIs in the partitions created in the top-left shifted grid
    cout << "Loading POIs for the top-left shifted grid inside the created partitions" << endl;
    vector<Map> topLeftShifted_vec (topLeftGrid, topLeftGrid + noOfSmallerPartitions);
    loadPoIs(dataRow, topLeftShifted_vec);
    cout << "Successfully loaded." << endl << endl;


    // Create top-right shifted grid
    cout << "Shifting partitions in the top right direction" << endl;
    topRightPartition_lat = TOP_LEFT_LAT + BOUNDARY_RANGE * factorLat;
    topRightPartition_long = TOP_LEFT_LONG + BOUNDARY_RANGE * factorLong;
    topRightGrid = getGrid(noOfSmallerPartitions, datarowSize);
    initialize_grid(topRightGrid, noOfSmallerPartitions, topRightPartition_lat, topRightPartition_long);
    cout << "Done." << endl;

    // Load the POIs in the partitions created in the top-right shifted grid
    cout << "Loading POIs for the top-left shifted grid inside the created partitions" << endl;
    vector<Map> topRightShifted_vec (topRightGrid, topRightGrid + noOfSmallerPartitions);
    loadPoIs(dataRow, topRightShifted_vec);
    cout << "Successfully loaded." << endl << endl;


    // Create bottom-left shifted grid
    cout << "Shifting partitions in the bottom left direction" << endl;
    bottomLeftPartition_lat = TOP_LEFT_LAT - BOUNDARY_RANGE * factorLat;
    bottomLeftPartition_long = TOP_LEFT_LONG - BOUNDARY_RANGE * factorLong;
    bottomLeftGrid = getGrid(noOfSmallerPartitions, datarowSize);
    initialize_grid(bottomLeftGrid, noOfSmallerPartitions, bottomLeftPartition_lat, bottomLeftPartition_long);
    cout << "Done." << endl;

    // Load the POIs in the partitions created in the bottom-left shifted grid
    cout << "Loading POIs for the bottom-left shifted grid inside the created partitions" << endl;
    vector<Map> bottomLeftShifted_vec (bottomLeftGrid, bottomLeftGrid + noOfSmallerPartitions);
    loadPoIs(dataRow, bottomLeftShifted_vec);
    cout << "Successfully loaded." << endl << endl;


    // Create bottom-right shifted grid
    cout << "Shifting partitions in the bottom right direction" << endl;
    bottomRightPartition_lat = TOP_LEFT_LAT - BOUNDARY_RANGE * factorLat;
    bottomRightPartition_long = TOP_LEFT_LONG + BOUNDARY_RANGE * factorLong;
    bottomRightGrid = getGrid(noOfSmallerPartitions, datarowSize);
    initialize_grid(bottomRightGrid, noOfSmallerPartitions, bottomRightPartition_lat, bottomRightPartition_long);
    cout << "Done." << endl;

    // Load the POIs in the partitions created in the bottom-right shifted grid
    cout << "Loading POIs for the bottom-right shifted grid inside the created partitions" << endl;
    vector<Map> bottomRightShifted_vec (bottomRightGrid, bottomRightGrid + noOfSmallerPartitions);
    loadPoIs(dataRow, bottomRightShifted_vec);
    cout << "Successfully loaded." << endl << endl;



    /*// Not completed. Call the k nearest for the whole map and then for the partitions created.
    // Enter the query
    cout << "Preprocessing complete. Waiting for query..." << endl;
    cin >> userLocation.latitude >> userLocation.longitude;
    cout << "Enter the POI category" << endl;
    cin >> POICategory;
    //find_K_NearestPOIs(userLocation, k, POICategory);*/
    
    return 0;
}