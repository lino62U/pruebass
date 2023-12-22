#include "BruteForce.h"

BruteForce::BruteForce(std::vector<Point>& p) : points(p) {}

std::vector<Point> BruteForce::bruteForceNearestNeighbors(const Point& query, int numNeighbors) {
    std::vector<Point> nearestNeighbors;
    nearestNeighbors.reserve(numNeighbors);

    auto compareDistances = [&query](const Point& a, const Point& b) -> bool {
        return (euclideanDistance(query, a) < euclideanDistance(query, b));
    };

    std::partial_sort(points.begin(), points.begin() + numNeighbors, points.end(), compareDistances);

    nearestNeighbors.assign(points.begin(), points.begin() + numNeighbors);

    return nearestNeighbors;
}

double BruteForce::euclideanDistance( const Point& p1, const Point& p2) {
    return std::sqrt(std::pow(p1.x - p2.x, 2) + std::pow(p1.y - p2.y, 2) + std::pow(p1.z - p2.z, 2));
}
