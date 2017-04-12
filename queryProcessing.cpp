#include <iostream>
#include <vector>
#include <cmath>
//#include "common.cpp"
#include "haversine_distance.cpp"
#define MAX_PARTITIONS 5
#define BOUNDARY_RANGE 0.05

using namespace std;


typedef struct POIforUser
{
    Data utility;
    double distance;
}selectedPOI;


// TO-DO, reduce this complexity
// Locates the smaller partition in which the user lies
Map locateUserPartition(Point userLocation, vector<Map>& initialPartition)
{
    int i;

    for(i = 0; i < initialPartition.size(); i++)
    {
        if(liesInPartition(initialPartition[i].p_id, userLocation))
            break;
    }
    return initialPartition[i];
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
vector<Data>& find_K_NearestPOIs(Point userLocation, vector<Map>& initialPartition, int k)
{
    int i, j;
    Map userPartitions[MAX_PARTITIONS];
    vector<Map> acceptedPartitions, rejectedPartitions;
    vector<selectedPOI> POIs;
    selectedPOI tempSelectedPOI;
    Map tempPartition;
    
    userPartitions[0] = locateUserPartition(userLocation, initialPartition);
    /*userPartitions[1] = topleftshift
    userPartitions[2] = toprightshift
    userPartitions[3] = bottomleftshift
    userPartitions[4] = bottomrightshift*/

    for(i = 0; i < MAX_PARTITIONS; i++)
    {
        if(satisfiesBoundaryCase(userLocation, userPartitions[i]))
            rejectedPartitions.push_back(userPartitions[i]);
        else
            acceptedPartitions.push_back(userPartitions[i]);
    }

    // Find POIs
    for(i = 0; i < acceptedPartitions.size(); i++)
    {
        tempPartition = acceptedPartitions[i];

        for(j = 0; j < tempPartition.noOfPOIs; i++)
        {
            tempSelectedPOI.utility = tempPartition.utility[j];
            tempSelectedPOI.distance = get_distance(userLocation, tempSelectedPOI.utility.coordinate);
            POIs.push_back(tempSelectedPOI);
        }
    }

    if(POIs.size() < k)
    {
        for(i = 0; i < rejectedPartitions.size(); i++)
        {
            tempPartition = rejectedPartitions[i];

            for(j = 0; j < tempPartition.noOfPOIs; i++)
            {
                tempSelectedPOI.utility = tempPartition.utility[j];
                tempSelectedPOI.distance = get_distance(userLocation, tempSelectedPOI.utility.coordinate);
                POIs.push_back(tempSelectedPOI);
            }

            if(POIs.size() >= k)
                break;
        }
    }

    // TO-DO: implement applyRecommendationAlgo for finding the nearest distance;
    applyRecommendationAlgo(POIs, k);
}


int main()
{
    int k;
    Point userLocation;

    cout << "Enter the query location." << endl;
    cin >> userLocation.latitude >> userLocation.longitude;
    cout << "Enter the no. of POIs wanted." << endl;
    cin >> k;

    //find_K_NearestPOIs(userLocation, k);
    return 0;
}