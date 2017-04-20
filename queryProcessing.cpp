#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <algorithm>
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


void applyRecommendationAlgo(vector<selectedPOI>& POIs, int k)
{
    int i;
    sort(POIs.begin(), POIs.end(), compareSelectedPOI);

    for(i = 0; i < k; i++)
    {
        cout << POIs[i].utility.utilities << endl;
        cout << "Latitude: " << POIs[i].utility.coordinate.latitude << "\tLongitude: " << POIs[i].utility.coordinate.longitude << endl << endl;
    }
}


// TO-DO, reduce this complexity
// Locates the smaller partition in which the user lies
Map locateUserPartition(Point userLocation, vector<Map>& grid)
{
    int i;

    for(i = 0; i < grid.size(); i++)
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
    int low, high, comparision, tempPos, mid;
    selectedPOI temp;
    vector<selectedPOI> foundPOIs;

    low = 0;
    high = partition.noOfPOIs;

    while(low + 1 != high)
    {
        mid = (low + high) / 2;
        comparision = partition.utility[mid].utilities.compare(POICategory);

        if (!comparision)
        {
            tempPos = mid;

            // for the POIs lying in the current and above positions
            while(tempPos >= 0 && partition.utility[tempPos].utilities.compare(POICategory))
            {
                temp.utility = partition.utility[tempPos];
                temp.distance = -1;
                foundPOIs.push_back(temp);
                tempPos--;
            }

            // For dataRow lying below the mid position
            tempPos = mid + 1;
            while(tempPos < partition.noOfPOIs && partition.utility[tempPos].utilities.compare(POICategory))
            {
                temp.utility = partition.utility[tempPos];
                temp.distance = -1;
                foundPOIs.push_back(temp);
                tempPos++;
            }
            return foundPOIs;
        }

        else if (comparision < 0)
            low = mid;
        else
            high = mid;
    }
}


// Adds the POIs in the 'partition' to the total list of relelevant 'POIs'
void addPOIs(Map partition, vector<selectedPOI>& POIs, string POICategory, Point userLocation)
{
    int prev, j = 0, l, tempPos;

    vector<selectedPOI> userPOIs = findPOIs(partition, POICategory);
        prev = j;
        j += DISTANCE_QUERY_LIMIT;

        while(j < userPOIs.size())
        {
            vector<selectedPOI> temp (userPOIs.begin() + prev, userPOIs.begin() + j);
            vector<double> distance = get_distance(temp, userLocation);
            tempPos = 0;

            for(l = prev; l <j; l++)
                userPOIs[l].distance = distance[tempPos++];

            prev = j;
            j += DISTANCE_QUERY_LIMIT;
        }

        vector<selectedPOI> temp (userPOIs.begin() + prev, userPOIs.end());
        vector<double> distance = get_distance(temp, userLocation);
        tempPos = 0;
        for(l = prev; l < userPOIs.size(); l++)
            userPOIs[l].distance = distance[tempPos++];
        
        POIs.insert(POIs.end(), userPOIs.begin(), userPOIs.end());
}


// This uses haversine distance
bool satisfiesBoundaryCase(Point userLocation, Map initialPartition)
{
    double averageDimension, latBoundaryRange, longBoundaryRange;
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


// TO-DO: Change distance measure to actual road distance
void find_K_NearestPOIs(Point userLocation, vector<Map> originalGrid,  vector<Map> topLeftGrid,  vector<Map> topRightGrid,  
                                                                        vector<Map> bottomLeftGrid,  vector<Map> bottomRightGrid, int k, string POICategory)
{
    int i, j = 0, l, prev, tempPos;
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
    for(i = 0; i < acceptedPartitions.size(); i++)
        addPOIs(acceptedPartitions[i], POIs, POICategory, userLocation);

    // If we do not have enough k then go in rejectedPartitions
    if(POIs.size() < k)
    {
        for(i = 0; i < rejectedPartitions.size(); i++)
        {
            addPOIs(rejectedPartitions[i], POIs, POICategory, userLocation);

            // If k is exceeded, no need to look at other rejectedPartitions
            if(POIs.size() >= k)
                break;
        }
    }

    // TO-DO: implement applyRecommendationAlgo for finding the nearest distance;
    applyRecommendationAlgo(POIs, k);
}


// Finding the POIs using brute force without creating any partitions
void brute_force(Point userLocation, string POICategory, Map unPartitioned, int k)
{
    vector<selectedPOI> POIs;

    addPOIs(unPartitioned, POIs, POICategory, userLocation);
    applyRecommendationAlgo(POIs, k);
}


int main()
{
    int k;
    string POICategory;
    Point userLocation;

    cout << "Enter the query location." << endl;
    cin >> userLocation.latitude >> userLocation.longitude;
    cout << "Enter the no. of POIs wanted." << endl;
    cin >> k;
    cout << "Enter the POI category" << endl;
    cin >> POICategory;

    //find_K_NearestPOIs(userLocation, k, POICategory);
    return 0;
}