#include <math.h>
#include <vector>
#include <algorithm>
#include <fstream>
#include "data_structures.hpp"
#define LATITUDE_RANGE 0.442
#define LONGITUDE_RANGE 0.595

using namespace std;


// The difference between the top and bottom latitudes of the smaller partition.
float getfactorLat(int partitionInRow)
{
    return (LATITUDE_RANGE/partitionInRow);
}


// The difference between the top and bottom latitudes of the smaller partition
float getfactorLong(int partitionInColumn)
{
    return (LONGITUDE_RANGE/partitionInColumn);
}


// initialization for initial partition
void initial_partition(struct Map* box, float factorLat, float factorLong, float topLeftLat, float topLeftLong)
{
    // for 16 smaller partitions in the first partition;
    //latitude range = 0.442;
    //longitude range = 0.595;
    
    box[0].p_id.top_left.latitude = topLeftLat;
    box[0].p_id.top_left.longitude = topLeftLong;

    box[0].p_id.top_right.latitude = box[0].p_id.top_left.latitude;
    box[0].p_id.top_right.longitude = box[0].p_id.top_left.longitude + factorLong;

    box[0].p_id.bottom_left.latitude = box[0].p_id.top_left.latitude - factorLat;
    box[0].p_id.bottom_left.longitude = box[0].p_id.top_left.longitude;

    box[0].p_id.bottom_right.latitude = box[0].p_id.bottom_left.latitude;
    box[0].p_id.bottom_right.longitude = box[0].p_id.bottom_left.longitude + factorLong;
}


// Set the coordinates of the smaller partitions.
void initialize_grid(Map *box, int boxSize, float topLeftLat, float topLeftLong)
{
    int totalPartitionInRow = (int) sqrt(boxSize);
    float factorLat = getfactorLat(totalPartitionInRow);
    float factorLong = getfactorLong(totalPartitionInRow); 

    initial_partition(box, factorLat, factorLong, topLeftLat, topLeftLong);
    for(int i = 1; i < boxSize; i++)
    {
        // For first partition in a new row
        if(i % totalPartitionInRow == 0)
        {
            //cout << "first:" << i << endl;
            int boxAbove = i - totalPartitionInRow;
            box[i].p_id.top_left = box[boxAbove].p_id.bottom_left;
            box[i].p_id.top_right = box[boxAbove].p_id.bottom_right;

            box[i].p_id.bottom_left.latitude = box[i].p_id.top_left.latitude - factorLat;
            box[i].p_id.bottom_left.longitude = box[i].p_id.top_left.longitude;

            box[i].p_id.bottom_right.latitude = box[i].p_id.top_right.latitude - factorLat;
            box[i].p_id.bottom_right.longitude = box[i].p_id.top_right.longitude;
        }

        // for all paritions except first one in a row
        else
        {
            //cout << "second:" << i << endl;
            // top_left and bottom_left
            box[i].p_id.top_left = box[i-1].p_id.top_right;
            box[i].p_id.bottom_left = box[i-1].p_id.bottom_right;

            // top_right
            box[i].p_id.top_right.latitude = box[i].p_id.top_left.latitude;
            box[i].p_id.top_right.longitude = box[i].p_id.top_left.longitude + factorLong;

            // bottom_right
            box[i].p_id.bottom_right.latitude = box[i].p_id.bottom_left.latitude;
            box[i].p_id.bottom_right.longitude = box[i].p_id.bottom_left.longitude + factorLong;
        }
        
    }
}


// Checks if the dataRow lies within the longitude range of the partition
bool liesWithinLongitudeRange(Partition smallerPartition, Point toBeLocated)
{
    return toBeLocated.longitude >= smallerPartition.top_left.longitude && toBeLocated.longitude <= smallerPartition.top_right.longitude;
}


// Checks if the dataRow lies within the latitude range of the partition
bool liesWithinLatitudeRange(Partition smallerPartition, Point toBeLocated)
{
    return toBeLocated.latitude <= smallerPartition.top_left.latitude && toBeLocated.latitude >= smallerPartition.bottom_left.latitude;
}

// Checks if the POI lies within the given partition
bool liesInPartition(Partition smallerPartition, Point toBeLocated)
{
    if(liesWithinLongitudeRange(smallerPartition, toBeLocated) && liesWithinLatitudeRange(smallerPartition, toBeLocated))
        return true;
    else
        return false;
}


// Sort the array of POIs in a parititon according to the 'utility' string
bool comparePOI(Data a, Data b)
{   
    if(a.utilities.compare(b.utilities) < 0)
        return true;
    return false;
}


// Assigns the POIs to their respective smaller partitions.
void loadPoIs(vector<Data>& dataRow, vector<Map>& smallerPartitions)
{
    int i, mid, low, high, tempPos, total = 0, stopBinarySearchFlag, posOfUtility;
    Map tempPartition;

    //cout << "\nInside new loadPOI" << endl;

    for(i = 0; i < smallerPartitions.size(); i++)
    {
        low = 0;
        high = dataRow.size();
        tempPartition = smallerPartitions[i];
        stopBinarySearchFlag = 0;

        while(low + 1 != high)
        {
            if(stopBinarySearchFlag)
                break;

            mid = (low + high) / 2;
            
            if(liesWithinLongitudeRange(tempPartition.p_id, dataRow[mid].coordinate))
            {
                tempPos = mid;
                posOfUtility = 0;

                while(tempPos >= 0 && dataRow[tempPos].coordinate.longitude >= tempPartition.p_id.top_left.longitude)
                {
                    if(liesInPartition(tempPartition.p_id, dataRow[tempPos].coordinate))
                        smallerPartitions[i].utility[posOfUtility++] = dataRow[tempPos];
                    tempPos--;
                }

                // For dataRow lying below the mid position
                tempPos = mid + 1;
                
                while(tempPos < dataRow.size() && dataRow[tempPos].coordinate.longitude <= tempPartition.p_id.top_right.longitude)
                {
                    if(liesInPartition(tempPartition.p_id, dataRow[tempPos].coordinate))
                        smallerPartitions[i].utility[posOfUtility++] = dataRow[tempPos];
                    tempPos++;
                }

                smallerPartitions[i].noOfPOIs = posOfUtility;
                
                // sort the POIs in partition
                sort(smallerPartitions[i].utility, smallerPartitions[i].utility + smallerPartitions[i].noOfPOIs, comparePOI);
                
                stopBinarySearchFlag = 1;
                //cout << posOfUtility << " ";
                total += posOfUtility;
            }

            else if(dataRow[mid].coordinate.longitude > tempPartition.p_id.top_right.longitude)
                 high = mid;
            else
                low = mid;
            }
        }

    //cout << "\ntotal = " << total << endl;
}


// For calculating the no of POIs in the dataset
int getNoOfPOIs()
{
    int count = 0;
    string str;
    ifstream fin("unique.csv");

    while(!fin.eof())
    {
        getline(fin,str);
        count++;
    }

    fin.close();
    return count;
}


// comparision function for struct 'Data'.
bool compareData(Data a, Data b)
{
    // if both have same longitude
    if(a.coordinate.longitude == b.coordinate.longitude)
        return a.coordinate.latitude < b.coordinate.latitude;
    
    return a.coordinate.longitude < b.coordinate.longitude;
}


// Load the POIs in the data structures
vector<Data> loadPOI_inDataStructures()
{
    string temp;
    Data dataTemp;
    ifstream file("unique.csv");
    vector<Data> dataRow;

    while(file.good())
    {
        getline(file, temp, ',');
        if(temp.empty())
            break;
        dataTemp.venue_id = temp;
        getline(file, dataTemp.venueCategory_id, ',');
        getline(file, dataTemp.utilities, ',');
        getline(file, temp, ',');
        dataTemp.coordinate.latitude = atof(temp.c_str());
        getline(file, temp);
        dataTemp.coordinate.longitude = atof(temp.c_str());
        dataRow.push_back(dataTemp);
    }
    file.close();
    
    // Sort dataRow
    sort(dataRow.begin(), dataRow.end(), compareData);
    return dataRow;
}