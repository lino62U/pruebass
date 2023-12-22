#ifndef KDTREE_H
#define KDTREE_H

#include <vector>
#include <algorithm>
#include <cmath>
#include "Point.h"
#include <set>


struct Node {
    Point point;
    Node* left;
    Node* right;

    Node(Point p) : point(p), left(nullptr), right(nullptr) {}
};


// Dentro de la clase KdTree


class KdTree {
public:
    KdTree(std::vector<Point>& points);
    ~KdTree();
    void printInorder();
    void kdtreeSearch(const Point& query, Node*& currentBest, double& currentBestDistance);
    void kdtreeSearchnn(const Point& query, std::vector<Node*>& currentBestSet, double& currentBestDistance, int n);
    static double euclideanDistance(const Point& p1, const Point& p2);
private:
    Node* root;
    Node* buildKdTree(std::vector<Point>& points, int depth);
    void printKdTree(Node* root, int depth);
    
    double distanceToSplittingPlane(const Point& query, int axis, const Node* node);
    static bool compareX(const Point& a, const Point& b);
    static bool compareY(const Point& a, const Point& b);
    static bool compareZ(const Point& a, const Point& b);
    void kdtreeSearch(Node* root, const Point& query, Node*& currentBest, double& currentBestDistance, int depth);
    void nnkdtreeSearch(Node* root, const Point& query, std::vector<Node*>& currentBestSet, double& currentBestDistance, int depth, int n);

    struct NodeComparator {
        const Point& query;

        NodeComparator(const Point& q) : query(q) {}

        bool operator()(const Node* a, const Node* b) const {
            return euclideanDistance(query, a->point) < euclideanDistance(query, b->point);
        }
    };

};

#endif // KDTREE_H
