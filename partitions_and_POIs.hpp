#ifndef partitions_and_POIs_H
#define partitions_and_POIs_H

#include <string>
#include <vector>
#include "data_structures.hpp"

extern void initialize_grid(Map*, int, float, float);
extern void loadPoIs(std::vector<Data>&, std::vector<Map>&);
extern int getNoOfPOIs();
extern std::vector<Data> loadPOI_inDataStructures();
extern float getfactorLat(int);
extern float getfactorLong(int);
extern void find_K_NearestPOIs(Point, std::vector<Map>&, std::vector<Map>&, std::vector<Map>&, std::vector<Map>&, std::vector<Map>&, int , std::string, int);
extern void brute_force(Point, std::string, Map, int);
extern void originalGrid_recommendation(Point, std::string, std::vector<Map>, int);
extern bool comparePOI(Data, Data);

#endif // !partitions_and_POIs_H
