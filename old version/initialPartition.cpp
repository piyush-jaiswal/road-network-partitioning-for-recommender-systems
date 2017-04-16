#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <math.h>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <set>
#include "common.cpp"
#define TOP_LEFT_LAT 40.99
#define TOP_LEFT_LONG -74.276

using namespace std;


// initialization for initial partition
void initial_partition(struct Map* box, float factorLat, float factorLong)
{
    // for 16 smaller partitions in the first partition;
    //latitude range = 0.442;
    //longitude range = 0.595;
    
    box[0].p_id.top_left.latitude = TOP_LEFT_LAT;
    box[0].p_id.top_left.longitude = TOP_LEFT_LONG;

    box[0].p_id.top_right.latitude = box[0].p_id.top_left.latitude;
    box[0].p_id.top_right.longitude = box[0].p_id.top_left.longitude + factorLong;

    box[0].p_id.bottom_left.latitude = box[0].p_id.top_left.latitude - factorLat;
    box[0].p_id.bottom_left.longitude = box[0].p_id.top_left.longitude;

    box[0].p_id.bottom_right.latitude = box[0].p_id.bottom_left.latitude;
    box[0].p_id.bottom_right.longitude = box[0].p_id.bottom_left.longitude + factorLong;
}


// Set the coordinates of the smaller partitions.
void initialize_box(Map *box, int boxSize)
{
    int totalPartitionInRow = (int) sqrt(boxSize);
    float factorLat = getfactorLat(latRange, totalPartitionInRow);
    float factorLong = getfactorLong(longRange, totalPartitionInRow); 

    initial_partition(box, factorLat, factorLong);
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


/*void loadPoIs(vector<Data>& dataRow, vector<Map>& smallerPartitions)
{
    int i, j, total = 0, pos;

    for(i = 0; i < smallerPartitions.size(); i++)
    {
        pos = 0;
        for(j = 0; j < dataRow.size(); j++)
        {
            if(liesInPartition(smallerPartitions[i].p_id, dataRow[j].coordinate))
                smallerPartitions[i].utility[pos++] = dataRow[j];
        }
        cout << pos << " ";
        total += pos;
    }

    cout << endl << "total = " << total << endl;
}*/


// Sort the array of POIs in a parititon according to the 'utility' string
bool comparePOI(Data a, Data b)
{   
    if(a.utilities.compare(b.utilities) < 0)
        return true;
    return false;
}


// Assigns the POIs to their respective smaller partitions.
void loadPoIs2(vector<Data>& dataRow, vector<Map>& smallerPartitions)
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
int getdatarowSize()
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


// The difference between the top and bottom latitudes of the smaller partition.
float getfactorLat(float distance, int partitionInRow)
{
    return (distance/partitionInRow);
}


// The difference between the top and bottom latitudes of the smaller partition
float getfactorLong(float distance, int partitionInColumn)
{
    return (distance/partitionInColumn);
}


// comparision function for struct 'Data'.
bool compareData(Data a, Data b)
{
    // if both have same longitude
    if(a.coordinate.longitude == b.coordinate.longitude)
        return a.coordinate.latitude < b.coordinate.latitude;
    
    return a.coordinate.longitude < b.coordinate.longitude;
}


// Comparision function to convert vector into set in order to remove duplicates.
bool operator<(const Data& a, const Data& b)
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


int main ()
{
    int noOfSmallerPartitions, datarowSize, i, j, rootOfSmallerPartitions;
    float factorLat, factorLong, latRange, longRange;
    string temp;
    Data dataTemp;

    //Taking the number of boxes in initial partition by the user
    cout << "Enter the number of partitions :" << endl;
    cin >> noOfSmallerPartitions;

    cout << "Enter the latitude range : " << endl;
    cin >> latRange;

    cout << "Enter the longitude range : " << endl;
    cin >> longRange;

    rootOfSmallerPartitions = (int) sqrt(noOfSmallerPartitions);

    // Data loading starts
    ifstream file("final_dataset.csv");
    ifstream fin("final_dataset.csv");

    if(!file.is_open())
        cout << "Error in opening file" << endl;

    //A function to take number of rows in the dataset and memory allocation to the object array.
    datarowSize = getdatarowSize(fin);
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
    // Data loading ends
    
    factorLat = getfactorLat(latRange, rootOfSmallerPartitions);
    factorLong = getfactorLong(longRange, rootOfSmallerPartitions);

    Map* box = new Map[noOfSmallerPartitions];
    
    for(i = 0; i < noOfSmallerPartitions; i++)
        box[i].utility = new Data[datarowSize];

    initialize_box(box, factorLat, factorLong, noOfSmallerPartitions, rootOfSmallerPartitions);
    vector<Map> smallerPartitons (box, box + noOfSmallerPartitions);
    sort(dataRow.begin(), dataRow.end(), compareData);

    set<Data> unique(dataRow.begin(), dataRow.end());
    //cout << "\nOriginal size = " << dataRow.size();
    //cout << "\nUnique size = " << unique.size() << endl;
    vector<Data> unique2(unique.begin(), unique.end());
    //loadPoIs(unique2, smallerPartitons);
    loadPoIs2(unique2, smallerPartitons);




    /*ofstream out("output.csv");  // to check the loaded data
    for(i = 0; i < datarowSize; i++)
    {
        out << dataRow[i].poi.venue_id << ",";
        out << dataRow[i].poi.venueCategory_id << ",";
        out << dataRow[i].poi.utilities << ",";
        out << dataRow[i].coordinate.latitude << ",";
        out << dataRow[i].coordinate.longitude << "\n";
    }
    out.close();*/

    /*ofstream out2("left.csv");  // to check the loaded data
    for(i = 0; i < unique2.size(); i++)
    {
        if(!unique2[i].selected)
        {
            out2 << unique2[i].poi.venue_id << ",";
            out2 << unique2[i].poi.venueCategory_id << ",";
            out2 << unique2[i].poi.utilities << ",";
            out2 << unique2[i].coordinate.latitude << ",";
            out2 << unique2[i].coordinate.longitude << "\n";
        }
    }
    out2.close();*/

    //Print the data for testing purposes
    /*for(i = 0; i < noOfSmallerPartitions; i++)
    {
        cout << endl << endl;
        cout << "Top_Left coordinates : " << smallerPartitons[i].p_id.top_left.latitude << "," << smallerPartitons[i].p_id.top_left.longitude << endl;
        cout << "Top_Right coordinates : " << smallerPartitons[i].p_id.top_right.latitude << "," << smallerPartitons[i].p_id.top_right.longitude << endl;
        cout << "Bottom_Right coordinates : " << smallerPartitons[i].p_id.bottom_right.latitude << "," << smallerPartitons[i].p_id.bottom_right.longitude << endl;
        cout << "Bottom_Left coordinates : " << smallerPartitons[i].p_id.bottom_left.latitude << "," << smallerPartitons[i].p_id.bottom_left.longitude << endl;
        //cout << "POIs in this partition are : " << endl;
        /*for(int j = 0; j < box[i].noOfPOIs; j++)
            cout << box[i].utility[j].utilities  << endl;*/
    //}

    /*cout << "Printing the first Partition : " << endl;
    cout << "Top_Left coordinates : " << box[0].p_id.top_left.latitude << "," << box[0].p_id.top_left.longitude << endl;
    cout << "Top_Right coordinates : " << box[0].p_id.top_right.latitude << "," << box[0].p_id.top_right.longitude << endl;
    cout << "Bottom_Right coordinates : " << box[0].p_id.bottom_right.latitude << "," << box[0].p_id.bottom_right.longitude << endl;
    cout << "Bottom_Left coordinates : " << box[0].p_id.bottom_left.latitude << "," << box[0].p_id.bottom_left.longitude << endl;
    cout << "POIs in this partition are : " << endl;
    cout << box[6].noOfPOIs;
    for(i = 0; i < box[4].noOfPOIs; i++)
        cout << box[4].utility[i].utilities  << endl;
    */

    return 0;
}
