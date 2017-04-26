#include <iostream>
#include <math.h>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
#include <iomanip>
#include "partitions_and_POIs.hpp"
#include "partitioned_grids.hpp"
#include "data_structures.hpp"

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
    unPartitioned.utility = new Data[unPartitioned.noOfPOIs];
    copy(dataRow.begin(), dataRow.end(), &unPartitioned.utility[0]);
    sort(unPartitioned.utility, unPartitioned.utility + unPartitioned.noOfPOIs, comparePOI);
    
    /*for(int i = 0; i < unPartitioned.noOfPOIs; i++)
        cout << unPartitioned.utility[i].utilities << endl;*/
    //unPartitioned.utility = &dataRow[0];

    return unPartitioned;
}

//Function to return randomly generated user points
double getRandom(double min, double max)
{
    double before = rand() % (int)max + (int)min;
    double after = (double)rand() / RAND_MAX;
    double result = before + after;
    if (result < min || result > max) {
        result = getRandom(min, max);
    }
    return result;
}


int main()
{
    srand (time(NULL));
    int noOfSmallerPartitions, totalPartitionInRow, datarowSize, k;
    clock_t t1, t2, t3;
    double time_taken1, time_taken2, time_taken3;
    float factorLat, factorLong;
    string POICategory;
    Point userLocation;
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
    //cout << unpartitioned.utility[0].utilities << endl;

    // Enter the query
    cout << "Preprocessing complete. Waiting for query..." << endl;
    cin >> userLocation.latitude;
    cin >> userLocation.longitude;
    //userLocation.latitude = getRandom(TOP_LEFT_LAT - LATITUDE_RANGE, TOP_LEFT_LAT);
    //userLocation.longitude = getRandom(TOP_LEFT_LONG, TOP_LEFT_LONG + LONGITUDE_RANGE);
    cout << "User coordinates: "; 
    cout << setprecision(10) << userLocation.latitude << setprecision(10) << userLocation.longitude << endl;;

    cout << "Enter the POI category" << endl;
    cin.ignore();
    getline(cin, POICategory);
    cout << "Enter the no. of POIs wanted." << endl;
    cin >> k;

    t1 = clock();
    brute_force(userLocation, POICategory, unpartitioned, k);
    t1 = clock() - t1;
    time_taken1 = ((double)t1) / CLOCKS_PER_SEC;
    cout << "Time taken by brute force: " << time_taken1<< endl;

    t2 = clock();
    find_K_NearestPOIs(userLocation,initialPartition_vec, topLeftShifted_vec, topRightShifted_vec, bottomLeftShifted_vec, bottomRightShifted_vec, k, POICategory);
    t2 = clock() - t2;
    time_taken2 = ((double)t2) / CLOCKS_PER_SEC;
    cout << "Time taken by the entire recommendation algorithm: " << time_taken2 << endl;

    t3 = clock();
    originalGrid_recommendation(userLocation, POICategory, initialPartition_vec, k);
    t3 = clock() - t3;
    time_taken3 = ((double)t3) / CLOCKS_PER_SEC;
    cout << "Time taken by the initial partition:  " << time_taken3 << endl;

    return 0;
}
