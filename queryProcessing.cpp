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
    int i;
    sort(POIs.begin(), POIs.end(), compareSelectedPOI);

    //cout << POIs.size() << endl;

    for(i = 0; i < min(k, (int) POIs.size()); i++)
    {
        // cout << POIs[i].utility.utilities << endl;
        // cout << "Latitude: " << POIs[i].utility.coordinate.latitude << "\tLongitude: " << POIs[i].utility.coordinate.longitude << endl;
        // cout << "Distance from user = " << POIs[i].distance << endl << endl; 
    }
}

//writing the poi coordinates to file
void writeCoordinatesToFile(vector <selectedPOI> &wantedPOI)
{
    int i, wantedPOISize;
    ofstream fout;
    fout.open("coordinates.txt", ios::trunc);
    //cout << wantedPOI[i].utility.coordinate.latitude << "," << wantedPOI[i].utility.coordinate.longitude

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
//TODO : Improve this writing
void writeUserCoordinatesToFile(Point userLocation)
{
    ofstream fo;
    fo.open("userpoint.txt", ios::trunc);
    //cout << "hihihi";
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
    //return x;
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
    //Doing linear search
    int i;
     vector<selectedPOI> foundPOIs;
     selectedPOI temp;
     
    int n = partition.noOfPOIs;
    //cout << "here: " << n << endl;
    int count = 0;
    for(i = 0; i < n; i++)
    {
        if(partition.utility[i].utilities == "Bar")
        {
            temp.utility = partition.utility[i];
            count++;
            temp.distance = -1;
            foundPOIs.push_back(temp);
        }
    }
    //cout << endl << "COunt: " << count << endl;
    return foundPOIs;
   /* int low, high, comparision, tempPos, mid;
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
    //Added since error was occuring "non-return type error".
    return foundPOIs;*/
}


// Adds the POIs in the 'partition' to the total list of relelevant 'POIs'
void addPOIs(Map partition, vector<selectedPOI>& POIs, string POICategory, Point userLocation)
{
    int prev, j = 0, l, tempPos, userPOIsSize, i;
    vector<selectedPOI> userPOIs = findPOIs(partition, POICategory);
    userPOIsSize = userPOIs.size();
    
    prev = j;
    //vector <double> :: iterator it;
    j += DISTANCE_QUERY_LIMIT;
    
    cout << endl << "size hai: " << userPOIsSize << endl;


        while(j < userPOIsSize)
        {
            cout << "mohit is awesome" << endl;
            vector<selectedPOI> temp (userPOIs.begin() + prev, userPOIs.begin() + j);
            // cout << endl << "yha hai" << endl;
            // cout << "size of temp " << temp.size() << endl;
            vector<double> distance = get_distance(temp, userLocation);
            //cout << distance.size() << endl;
            cout << endl << "yha hai" << endl; 
            tempPos = 0;
             for(l = prev; l < j; l++)
            {
                //cout << "check" << endl;
                userPOIs[l].distance = distance[tempPos++];
            }
                
            prev = j;
            j += DISTANCE_QUERY_LIMIT;
        }
         
        vector<selectedPOI> temp (userPOIs.begin() + prev, userPOIs.end());
        vector<double> distance = get_distance(temp, userLocation);
        tempPos = 0;
        for(l = prev; l < userPOIsSize; l++)
        {
            userPOIs[l].distance = distance[tempPos++];
            //cout << "should not come here " << endl;
        }
        //cout << "printing distance size" << distance.size();
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


// TO-DO: Change distance measure to actual road distance
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
    //cout << "anshul: " << acceptedPartitionSize << "    " << rejectedPartitionSize << endl;
    addPOIs(acceptedPartitions[0], POIs, POICategory, userLocation);
//     for(i = 0; i < acceptedPartitionSize; i++)
//     {
//         addPOIs(acceptedPartitions[i], POIs, POICategory, userLocation);
//         //cout << "ye to hoga";
//     }
// //        addPOIs(acceptedPartitions[i], POIs, POICategory, userLocation);
//     // If we do not have enough k then go in rejectedPartitions
//     poiSize = POIs.size();
//     //cout << "pos anshul : " << poiSize << endl;
//     if(poiSize < k)
//     {
//         for(i = 0; i < rejectedPartitionSize; i++)
//         {
//             addPOIs(rejectedPartitions[i], POIs, POICategory, userLocation);
//             poiSize = POIs.size();
//             // cout << "anshul is awesome" << POIs.size() <<  endl;
//             // If k is exceeded, no need to look at other rejectedPartitions
//             if(poiSize >= k){
//                 cout << "break is working" << endl;
//                 break;
//             }
//             // else if(poiSize < k)
//             // {
//             //     break;
//             // }
//         }
//         //cout << "anshul is awesome" << endl;

//     }

    // TO-DO: implement applyRecommendationAlgo for finding the nearest distance;
   // applyRecommendationAlgo(POIs, k);
}


// Finding the POIs using brute force without creating any partitions
void brute_force(Point userLocation, string POICategory, Map unPartitioned, int k)
{
    vector<selectedPOI> POIs;

    addPOIs(unPartitioned, POIs, POICategory, userLocation);
    applyRecommendationAlgo(POIs, k);
}


// int main()
// {
//     int k;
//     string POICategory;
//     Point userLocation;

//     cout << "Enter the query location." << endl;
//     cin >> userLocation.latitude >> userLocation.longitude;
//     cout << "Enter the no. of POIs wanted." << endl;
//     cin >> k;
//     cout << "Enter the POI category" << endl;
//     cin >> POICategory;

//     find_K_NearestPOIs(userLocation, initialPartition_vec,topL k, POICategory);
//     return 0;
// }