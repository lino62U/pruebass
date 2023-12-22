#ifndef BRUTEFORCE_H
#define BRUTEFORCE_H

#include <vector>
#include <cmath>
#include "Point.h"
#include <algorithm>  // Asegúrate de agregar esta línea
class BruteForce {
private:
    std::vector<Point> points;

public:
    BruteForce(std::vector<Point>& p);
    std::vector<Point> bruteForceNearestNeighbors(const Point& query, int numNeighbors);


    static double euclideanDistance(const Point& p1, const Point& p2);
    

};
#endif // BRUTEFORCE_H
