using namespace std;

struct Point
{
    float latitude;
    float longitude;
};

// structure containing the data for the a POI
struct Data 
{
    string venue_id;
    string venueCategory_id;
    string utilities;
    Point coordinate;
};

// structure containing the coordinates of the smaller partitions
struct Partition
{
    Point top_left;
    Point top_right;
    Point bottom_left;
    Point bottom_right;
};

// structure for the smaller partitions
struct Map
{
    Partition p_id;
    Data *utility;
    int noOfPOIs;
};


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