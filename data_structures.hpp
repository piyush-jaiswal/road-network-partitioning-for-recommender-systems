#ifndef data_structures_H
#define data_structures_H

#include <string>

struct Point
{
    float latitude;
    float longitude;
};

// structure containing the data for the a POI
struct Data 
{
    std::string venue_id;
    std::string venueCategory_id;
    std::string utilities;
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

#endif // !data_structures_H
