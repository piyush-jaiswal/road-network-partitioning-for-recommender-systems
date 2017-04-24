#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <fstream>
#include <algorithm>
#include "data_structures.hpp"
#include "haversine_and_partition.hpp"
#define MAX_PARTITIONS 5
#define BOUNDARY_RANGE 0.05
#define DISTANCE_QUERY_LIMIT 24

using namespace std;


typedef struct POIforUser
{
    Data utility;
    double distance;
}selectedPOI;



bool compareSelectedPOI(selectedPOI a, selectedPOI b)
{
    return a.distance < b.distance;
}


void applyRecommendationAlgo(vector<selectedPOI> POIs, int k)
{
    int i, resultSize;
    std::sort(POIs.begin(), POIs.end(), compareSelectedPOI);
    resultSize = min(k, (int) (POIs.size()));

    cout << "You have in total : " << resultSize << " " << POIs[0].utility.utilities << endl;
    cout << "In increasing order of distance, they are at a distance of :";
    for(i = 0; i < resultSize; i++)
      cout << POIs[i].distance << "km" << endl;
}

//writing the poi coordinates to file
void writeCoordinatesToFile(vector <selectedPOI> &wantedPOI)
{
    int i, wantedPOISize;
    ofstream fout;
    fout.open("coordinates.txt", ios::trunc);

    wantedPOISize = wantedPOI.size();
    for(i = 0; i < wantedPOISize; i++)
    {
        fout << wantedPOI[i].utility.coordinate.latitude << "," << wantedPOI[i].utility.coordinate.longitude << endl;
        //cout << wantedPOI[i].utility.coordinate.latitude << "," << wantedPOI[i].utility.coordinate.longitude << endl;
    }

    fout.close();
    //This is awesome, it creates a pipe for seeking command line terminal from external program.
    system("node googleDistanceAPI.js gnome-terminal -x sh -c 'cd /tmp ; ls -la'");
}

//writing userLocation to file
void writeUserCoordinatesToFile(Point userLocation)
{
    ofstream fo;
    fo.open("userpoint.txt", ios::trunc);
    fo << userLocation.latitude << "," << userLocation.longitude << endl;
    fo.close();
}

//Getting the distance vector from js file
vector <double> get_distance(vector <selectedPOI> &temp, Point userLocation)
{
    vector<double> x;
    writeCoordinatesToFile(temp);
    writeUserCoordinatesToFile(userLocation);
    vector <double> distanceVector;
    ifstream fin;
    string str;
    fin.open("distances.txt");
    while(!fin.eof())
    {
        getline(fin,str);
        if(str != "")
            distanceVector.push_back(stod(str));
    }
    return distanceVector;
    return x;
}

// TO-DO, reduce this complexity
// Locates the smaller partition in which the user lies
Map locateUserPartition(Point userLocation, vector<Map>& grid)
{
    int gridSize = grid.size(), i;
    for(i = 0; i < gridSize; i++)
    {
        if(liesInPartition(grid[i].p_id, userLocation))
            return grid[i];
    }

    // If user does not lie in any partition in the grid
    Map notFound;
    notFound.noOfPOIs = -1;
    return notFound;
}


// Finds the relevant POIs for the user in the partition
vector<selectedPOI> findPOIs(Map partition, string POICategory)
{
    vector<selectedPOI> foundPOIs;
    int comparision, mid, low, high, tempPos, stopBinarySearchFlag;
    selectedPOI temp;
    low = 0;
    high = partition.noOfPOIs;
    stopBinarySearchFlag = 0;
    
    while(low <= high)
    {
        if(stopBinarySearchFlag)
            break;
            
        mid = (low + high) / 2;
        comparision = partition.utility[mid].utilities.compare(POICategory);
    
        // If both the strings are equal
        if (comparision == 0)
        {
            tempPos = mid;
    
            // for the POIs lying in the current and below positions
            while(tempPos >= 0 && !partition.utility[tempPos].utilities.compare(POICategory))
            {
                temp.utility = partition.utility[tempPos];
                temp.distance = -1;
                foundPOIs.push_back(temp);
                tempPos--;
            }
    
            // For dataRow lying above the mid position
            tempPos = mid + 1;
            while(tempPos < partition.noOfPOIs && !partition.utility[tempPos].utilities.compare(POICategory))
            {
                temp.utility = partition.utility[tempPos];
                temp.distance = -1;
                foundPOIs.push_back(temp);
                tempPos++;
            }

            stopBinarySearchFlag = 1;
        }
    
        else if (comparision < 0)
            low = mid + 1;
        else
            high = mid - 1;
     }
    //Added since error was occuring "non-return type error".
    //cout << "reached end" << endl;
    return foundPOIs;
}


// Adds the POIs in the 'partition' to the total list of relelevant 'POIs'
void addPOIs(Map partition, vector<selectedPOI>& POIs, string POICategory, Point userLocation)
{
    int prev, j = 0, l, tempPos, userPOIsSize, i;
    vector<selectedPOI> userPOIs = findPOIs(partition, POICategory);
    userPOIsSize = userPOIs.size();

    prev = j;
    j += DISTANCE_QUERY_LIMIT;

        while(j < userPOIsSize)
        {
            vector<selectedPOI> temp (userPOIs.begin() + prev, userPOIs.begin() + j);
            vector<double> distance = get_distance(temp, userLocation);
            tempPos = 0;
             for(l = prev; l < j; l++)
                userPOIs[l].distance = distance[tempPos++];

            prev = j;
            j += DISTANCE_QUERY_LIMIT;
        }

        vector<selectedPOI> temp (userPOIs.begin() + prev, userPOIs.end());
        vector<double> distance = get_distance(temp, userLocation);
        tempPos = 0;
        for(l = prev; l < userPOIsSize; l++)
            userPOIs[l].distance = distance[tempPos++];

         for(i = 0; i < userPOIsSize; i++)
             cout << userPOIs[i].distance << endl;
        POIs.insert(POIs.end(), userPOIs.begin(), userPOIs.end());
}


// This uses haversine distance
bool satisfiesBoundaryCase(Point userLocation, Map initialPartition)
{
    double latBoundaryRange, longBoundaryRange;
    bool hasRightBoundaryCase, hasLeftBoundaryCase, hasTopBoundaryCase, hasBottomBoundaryCase;
    Point temp;

    // The range within which the user lies within the boundary.
    // Separate for latitude and longitude because they are not exactly equal.
    latBoundaryRange = distanceEarth(initialPartition.p_id.top_left, initialPartition.p_id.bottom_left) * BOUNDARY_RANGE;
    longBoundaryRange = distanceEarth(initialPartition.p_id.top_right, initialPartition.p_id.top_left) * BOUNDARY_RANGE;

    // Calculate the distances from the four boundaries using haversine_distance
    temp.latitude = userLocation.latitude;
    temp.longitude = initialPartition.p_id.top_right.longitude;
    hasRightBoundaryCase = distanceEarth(userLocation, temp) < longBoundaryRange;

    temp.longitude = initialPartition.p_id.top_left.longitude;
    hasLeftBoundaryCase = distanceEarth(userLocation, temp) < longBoundaryRange;

    temp.latitude = initialPartition.p_id.top_left.latitude;
    temp.longitude = userLocation.longitude;
    hasTopBoundaryCase = distanceEarth(userLocation, temp) < latBoundaryRange;

    temp.latitude = initialPartition.p_id.bottom_left.latitude;
    hasBottomBoundaryCase = distanceEarth(userLocation, temp) < latBoundaryRange;

    return hasRightBoundaryCase | hasLeftBoundaryCase | hasTopBoundaryCase | hasBottomBoundaryCase;
}

//Function of our project
void find_K_NearestPOIs(Point userLocation, vector<Map> &originalGrid,  vector<Map> &topLeftGrid,  vector<Map> &topRightGrid,
                                                                        vector<Map> &bottomLeftGrid,  vector<Map> &bottomRightGrid, int k, string POICategory)
{
    int  i, acceptedPartitionSize, rejectedPartitionSize, poiSize;
    Map userPartitions[MAX_PARTITIONS];
    vector<Map> acceptedPartitions, rejectedPartitions;
    vector<selectedPOI> POIs;
    selectedPOI tempSelectedPOI;

    userPartitions[0] = locateUserPartition(userLocation, originalGrid);
    userPartitions[1] = locateUserPartition(userLocation, topLeftGrid);
    userPartitions[2] = locateUserPartition(userLocation, topRightGrid);
    userPartitions[3] = locateUserPartition(userLocation, bottomLeftGrid);
    userPartitions[4] = locateUserPartition(userLocation, bottomRightGrid);

    for(i = 0; i < MAX_PARTITIONS; i++)
    {
        if(userPartitions[i].noOfPOIs > 0)
        {
            if(satisfiesBoundaryCase(userLocation, userPartitions[i]))
                rejectedPartitions.push_back(userPartitions[i]);
            else
                acceptedPartitions.push_back(userPartitions[i]);
        }
    }

    // Find POIs in accepted partitions
    acceptedPartitionSize = acceptedPartitions.size();
    rejectedPartitionSize = rejectedPartitions.size();

    for(i = 0; i < acceptedPartitionSize; i++)
        addPOIs(acceptedPartitions[i], POIs, POICategory, userLocation);

    // If we do not have enough k then go in rejectedPartitions
    poiSize = POIs.size();
    if(poiSize < k)
    {
        for(i = 0; i < rejectedPartitionSize; i++)
        {
            addPOIs(rejectedPartitions[i], POIs, POICategory, userLocation);
            poiSize = POIs.size();

            // If k is exceeded, no need to look at other rejectedPartitions
            if(poiSize >= k)
              break;
        }
    }

    applyRecommendationAlgo(POIs, k);
}


// Finding the POIs using brute force without creating any partitions
void brute_force(Point userLocation, string POICategory, Map unPartitioned, int k)
{
    vector<selectedPOI> POIs;

    addPOIs(unPartitioned, POIs, POICategory, userLocation);
    applyRecommendationAlgo(POIs, k);
}
