#include "KdTree.h"
#include <iostream>


KdTree::KdTree(std::vector<Point>& points) : root(nullptr) {
    root = buildKdTree(points, 0);
}

KdTree::~KdTree() {
    // Implementa la liberación de memoria si es necesario
}

Node* KdTree::buildKdTree(std::vector<Point>& points, int depth) {
    if (points.empty()) {
        return nullptr;
    }

    int k = depth % 3; // Alternamos entre las dimensiones x, y, z
    if (k == 0) {
        std::sort(points.begin(), points.end(), compareX);
    }
    else if (k == 1) {
        std::sort(points.begin(), points.end(), compareY);
    }
    else {
        std::sort(points.begin(), points.end(), compareZ);
    }

    int medianIndex = points.size() / 2;
    Node* node = new Node(points[medianIndex]);

    std::vector<Point> leftPoints(points.begin(), points.begin() + medianIndex);
    std::vector<Point> rightPoints(points.begin() + medianIndex + 1, points.end());

    node->left = buildKdTree(leftPoints, depth + 1);
    node->right = buildKdTree(rightPoints, depth + 1);

    return node;
}

void KdTree::printInorder() {
    std::cout << "Kd-Tree Inorder Traversal:\n";
    printKdTree(root, 0);
}

void KdTree::kdtreeSearch(const Point& query, Node*& currentBest, double& currentBestDistance) {
    kdtreeSearch(root, query, currentBest, currentBestDistance, 0);
}

void KdTree::kdtreeSearchnn(const Point& query, std::vector<Node*>& currentBestSet, double& currentBestDistance, int n)
{
    nnkdtreeSearch(root, query, currentBestSet, currentBestDistance, 0, n);

}

void KdTree::printKdTree(Node* root, int depth) {
    if (root) {
        printKdTree(root->left, depth + 1);
        std::cout << "(" << root->point.x << ", " << root->point.y << ", " << root->point.z << ") at depth " << depth << "\n";
        printKdTree(root->right, depth + 1);
    }
}

double KdTree::euclideanDistance(const Point& p1, const Point& p2) {
    return std::sqrt(std::pow(p1.x - p2.x, 2) + std::pow(p1.y - p2.y, 2) + std::pow(p1.z - p2.z, 2));
}

double KdTree::distanceToSplittingPlane(const Point& query, int axis, const Node* node) {
    if (axis == 0) return std::abs(query.x - node->point.x);
    if (axis == 1) return std::abs(query.y - node->point.y);
    if (axis == 2) return std::abs(query.z - node->point.z);
    return 0.0; // Default case, should not happen
}

bool KdTree::compareX(const Point& a, const Point& b) {
    return a.x < b.x;
}

bool KdTree::compareY(const Point& a, const Point& b) {
    return a.y < b.y;
}

bool KdTree::compareZ(const Point& a, const Point& b) {
    return a.z < b.z;
}

void KdTree::kdtreeSearch(Node* root, const Point& query, Node*& currentBest, double& currentBestDistance, int depth) {
    if (!root) return;

    int axis = depth % 3; // Alternamos entre las dimensiones x, y, z

    double distanceToCurrentNode = euclideanDistance(query, root->point);

    if (distanceToCurrentNode < currentBestDistance) {
        currentBest = root;
        currentBestDistance = distanceToCurrentNode;
    }

    // Decide qué lado del árbol explorar
    Node* firstBranch, * secondBranch;
    if (axis == 0) {
        firstBranch = (query.x < root->point.x) ? root->left : root->right;
        secondBranch = (query.x < root->point.x) ? root->right : root->left;
    }
    else if (axis == 1) {
        firstBranch = (query.y < root->point.y) ? root->left : root->right;
        secondBranch = (query.y < root->point.y) ? root->right : root->left;
    }
    else {
        firstBranch = (query.z < root->point.z) ? root->left : root->right;
        secondBranch = (query.z < root->point.z) ? root->right : root->left;
    }

    // Recursivamente explorar el lado más cercano
    kdtreeSearch(firstBranch, query, currentBest, currentBestDistance, depth + 1);

    // Verificar si es necesario explorar el otro lado
    if (distanceToSplittingPlane(query, axis, root) < currentBestDistance) {
        kdtreeSearch(secondBranch, query, currentBest, currentBestDistance, depth + 1);
    }
}

void KdTree::nnkdtreeSearch(Node* root, const Point& query, std::vector<Node*>& currentBestSet, double& currentBestDistance, int depth, int n) {
    if (!root) return;

    int axis = depth % 3; // Alternamos entre las dimensiones x, y, z

    double distanceToCurrentNode = euclideanDistance(query, root->point);

    if (currentBestSet.size() < n || distanceToCurrentNode < currentBestDistance) {
        currentBestSet.push_back(root);

        // Ordenar el vector por distancia
        std::sort(currentBestSet.begin(), currentBestSet.end(), [query, this](const Node* a, const Node* b) {
            return euclideanDistance(query, a->point) < euclideanDistance(query, b->point);
            });

        if (currentBestSet.size() > n) {
            currentBestSet.pop_back();
        }

        currentBestDistance = euclideanDistance(query, currentBestSet.back()->point);
    }

    // Decide qué lado del árbol explorar
    Node* firstBranch, * secondBranch;
    if (axis == 0) {
        firstBranch = (query.x < root->point.x) ? root->left : root->right;
        secondBranch = (query.x < root->point.x) ? root->right : root->left;
    }
    else if (axis == 1) {
        firstBranch = (query.y < root->point.y) ? root->left : root->right;
        secondBranch = (query.y < root->point.y) ? root->right : root->left;
    }
    else {
        firstBranch = (query.z < root->point.z) ? root->left : root->right;
        secondBranch = (query.z < root->point.z) ? root->right : root->left;
    }

    // Recursivamente explorar el lado más cercano
    nnkdtreeSearch(firstBranch, query, currentBestSet, currentBestDistance, depth + 1, n);

    // Verificar si es necesario explorar el otro lado
    if (distanceToSplittingPlane(query, axis, root) < currentBestDistance) {
        nnkdtreeSearch(secondBranch, query, currentBestSet, currentBestDistance, depth + 1, n);
    }
}
