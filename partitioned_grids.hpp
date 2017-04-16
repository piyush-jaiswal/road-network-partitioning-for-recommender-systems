#ifndef partitioned_grids_H
#define partitioned_grids_H

#include <vector>
#include "data_structures.hpp"

extern std::vector<Map> originalGrid(int, int);
extern std::vector<Map> topLeftShift(int, int, float, float);
extern std::vector<Map> topRightShift(int, int, float, float);
extern std::vector<Map> bottomLeftShift(int, int, float, float);
extern std::vector<Map> bottomRightShift(int, int, float, float);

#endif // !partitioned_grids_H