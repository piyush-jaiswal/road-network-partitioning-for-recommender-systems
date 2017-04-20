#include <iostream>
#include <math.h>
#include "partitions_and_POIs.hpp"
#include "partitioned_grids.hpp"

// for brute_force
#define TOP_LEFT_LAT 40.99
#define TOP_LEFT_LONG -74.276
#define LATITUDE_RANGE 0.442
#define LONGITUDE_RANGE 0.595

using namespace std;


// To get the whole original grid as a single partition for applying brute force.
Map get_brute_force_partition(vector<Data> dataRow)
{
    Map unPartitioned;
    Point topLeft, topRight, bottomLeft, bottomRight;

    topLeft.latitude = TOP_LEFT_LAT;
    topLeft.longitude = TOP_LEFT_LONG;
    topRight.latitude = TOP_LEFT_LAT;
    topRight.longitude = TOP_LEFT_LONG + LONGITUDE_RANGE;
    bottomLeft.latitude = TOP_LEFT_LAT - LATITUDE_RANGE;
    bottomLeft.longitude = TOP_LEFT_LONG;
    bottomRight.latitude = TOP_LEFT_LAT - LATITUDE_RANGE;
    bottomRight.longitude = TOP_LEFT_LONG + LONGITUDE_RANGE;

    unPartitioned.p_id.top_left = topLeft;
    unPartitioned.p_id.top_right = topRight;
    unPartitioned.p_id.bottom_left = bottomLeft;
    unPartitioned.p_id.bottom_right = bottomRight;

    unPartitioned.noOfPOIs = dataRow.size();
    copy(dataRow.begin(), dataRow.end(), unPartitioned.utility);

    return unPartitioned;
}


int main()
{
    int noOfSmallerPartitions, totalPartitionInRow, datarowSize;
    float factorLat, factorLong;
    string POICategory;
    vector<Data> dataRow;
    Map unpartitioned;

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
    vector<Map> initialPartition_vec = originalGrid(noOfSmallerPartitions, datarowSize);
    cout << "Success." << endl;
    // Load the POIs in the partitions created in the original grid
    cout << "Loading POIs for the original grid inside the created partitions" << endl;
    loadPoIs(dataRow, initialPartition_vec);
    cout << "Successfully loaded." << endl << endl;


    // Get factorLat and factorLong
    totalPartitionInRow = (int) sqrt(noOfSmallerPartitions);
    factorLat = getfactorLat(totalPartitionInRow);
    factorLong = getfactorLong(totalPartitionInRow);


    // Create top-left shifted grid
    cout << "Shifting partitions in the top left direction" << endl;
    vector<Map>topLeftShifted_vec = topLeftShift(noOfSmallerPartitions, datarowSize, factorLat, factorLong);
    cout << "Done." << endl;
    // Load the POIs in the partitions created in the top-left shifted grid
    cout << "Loading POIs for the top-left shifted grid inside the created partitions" << endl;
    loadPoIs(dataRow, topLeftShifted_vec);
    cout << "Successfully loaded." << endl << endl;


    // Create top-right shifted grid
    cout << "Shifting partitions in the top right direction" << endl;
    vector<Map>topRightShifted_vec = topRightShift(noOfSmallerPartitions, datarowSize, factorLat, factorLong);
    cout << "Done." << endl;
    // Load the POIs in the partitions created in the top-right shifted grid
    cout << "Loading POIs for the top-left shifted grid inside the created partitions" << endl;
    loadPoIs(dataRow, topRightShifted_vec);
    cout << "Successfully loaded." << endl << endl;


    // Create bottom-left shifted grid
    cout << "Shifting partitions in the bottom left direction" << endl;
    vector<Map>bottomLeftShifted_vec = bottomLeftShift(noOfSmallerPartitions, datarowSize, factorLat, factorLong);
    cout << "Done." << endl;
    // Load the POIs in the partitions created in the bottom-left shifted grid
    cout << "Loading POIs for the bottom-left shifted grid inside the created partitions" << endl;
    loadPoIs(dataRow, bottomLeftShifted_vec);
    cout << "Successfully loaded." << endl << endl;


    // Create bottom-right shifted grid
    cout << "Shifting partitions in the bottom right direction" << endl;
    vector<Map>bottomRightShifted_vec = bottomRightShift(noOfSmallerPartitions, datarowSize, factorLat, factorLong);
    cout << "Done." << endl;
    // Load the POIs in the partitions created in the bottom-right shifted grid
    cout << "Loading POIs for the bottom-right shifted grid inside the created partitions" << endl;
    loadPoIs(dataRow, bottomRightShifted_vec);
    cout << "Successfully loaded." << endl << endl;

    // Preprocessing for the brute_force
   unpartitioned =  get_brute_force_partition(dataRow);

    /*// Not completed. Call the k nearest for the whole map and then for the partitions created.
    // Enter the query
    cout << "Preprocessing complete. Waiting for query..." << endl;
    cin >> userLocation.latitude >> userLocation.longitude;
    cout << "Enter the POI category" << endl;
    cin >> POICategory;
    //find_K_NearestPOIs(userLocation, k, POICategory);*/
    
    return 0;
}