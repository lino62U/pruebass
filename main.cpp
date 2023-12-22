#include <iostream>
#include <fstream>
#include <sstream>
#include "KdTree.h"
#include <limits>
#include "BruteForce.h"

#include <chrono>

// Función para leer puntos desde un archivo CSV
std::vector<Point> readPointsFromCSV(const std::string& filename) {
    std::vector<Point> points;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error al abrir el archivo: " << filename << std::endl;
        return points;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        double x, y, z;
        char delimiter;  // Coma u otro delimitador utilizado en el archivo CSV

        if (!(iss >> x >> delimiter >> y >> delimiter >> z)) {
            std::cerr << "Error al leer línea del archivo CSV: " << line << std::endl;
            continue;
        }

        points.emplace_back(x, y, z);
    }

    file.close();
    return points;
}


void ejecutionTimeInsertionKdtree() {
    std::string files[4] = { "15.csv", "1000.csv", "10000.csv", "20000.csv" };
    std::vector<long long> insertionTimes;  // Vector para almacenar los tiempos de inserción

    for (size_t i = 0; i < 4; i++) {
        std::vector<Point> points = readPointsFromCSV(files[i]);

        // Medir tiempo para la inserción en el Kd-Tree
        auto startKdTree = std::chrono::high_resolution_clock::now();
        KdTree kdTree(points);
        auto endKdTree = std::chrono::high_resolution_clock::now();

        // Calcular y almacenar el tiempo transcurrido en el Kd-Tree
        auto durationKdTree = std::chrono::duration_cast<std::chrono::microseconds>(endKdTree - startKdTree);
        insertionTimes.push_back(durationKdTree.count());
    }

    // Imprimir los tiempos almacenados
    for (size_t i = 0; i < insertionTimes.size(); i++) {
        std::cout << "Tiempo de insercion con " << files[i] << ": " << insertionTimes[i] << " microsegundos\n";
    }

    // Guardar los tiempos en un archivo de texto
    std::ofstream outputFile("tiempos_insercion_kdtree.txt");
    if (outputFile.is_open()) {
        for (size_t i = 0; i < insertionTimes.size(); i++) {
            outputFile << "Tiempo de insercion con " << files[i] << ": " << insertionTimes[i] << " microsegundos\n";
        }
        outputFile.close();
        std::cout << "Resultados guardados en 'tiempos_insercion_kdtree.txt'\n";
    }
    else {
        std::cerr << "Error al abrir el archivo de salida\n";
    }

}


void ejecutionTimeSearchNN()
{
    std::string files[4] = { "15.csv", "1000.csv", "10000.csv", "20000.csv" };
    std::vector<long long> searchTimesKdTree;  // Vector para almacenar los tiempos de búsqueda en el KD-tree
    std::vector<long long> searchTimesBruteForce;  // Vector para almacenar los tiempos de búsqueda de fuerza bruta

    for (size_t i = 0; i < 4; i++) {
        std::vector<Point> points = readPointsFromCSV(files[i]);

        // Crear KD-tree
        KdTree kdTree(points);

        // Punto de consulta (ajusta según tus necesidades)
        Point queryPoint(5, 5, 5);

        // Número de vecinos más cercanos que deseas encontrar
        int n = 5;

        // Medir tiempo para la búsqueda en el KD-tree
        auto startKdTree = std::chrono::high_resolution_clock::now();
        std::vector<Node*> nearestNeighborsKdTree;
        double currentBestDistanceKdTree = std::numeric_limits<double>::infinity();
        kdTree.kdtreeSearchnn(queryPoint, nearestNeighborsKdTree, currentBestDistanceKdTree, n);
        auto endKdTree = std::chrono::high_resolution_clock::now();

        // Calcular y almacenar el tiempo transcurrido en la búsqueda en el KD-tree
        auto durationKdTree = std::chrono::duration_cast<std::chrono::microseconds>(endKdTree - startKdTree);
        searchTimesKdTree.push_back(durationKdTree.count());

        // Medir tiempo para la búsqueda de fuerza bruta
        BruteForce bruteForceData(points);
        auto startBruteForce = std::chrono::high_resolution_clock::now();
        std::vector<Point> nearestNeighborBruteForce = bruteForceData.bruteForceNearestNeighbors(queryPoint, n);
        auto endBruteForce = std::chrono::high_resolution_clock::now();

        // Calcular y almacenar el tiempo transcurrido en la búsqueda de fuerza bruta
        auto durationBruteForce = std::chrono::duration_cast<std::chrono::microseconds>(endBruteForce - startBruteForce);
        searchTimesBruteForce.push_back(durationBruteForce.count());
    }

    // Imprimir y guardar los tiempos para la búsqueda en el KD-tree
    std::ofstream outputFileKdTree("tiempos_busqueda_kdtree.txt");
    if (outputFileKdTree.is_open()) {
        for (size_t i = 0; i < searchTimesKdTree.size(); i++) {
            std::cout << "Tiempo de búsqueda KD-tree con " << files[i] << ": " << searchTimesKdTree[i] << " microsegundos\n";
            outputFileKdTree << "Tiempo de búsqueda KD-tree con " << files[i] << ": " << searchTimesKdTree[i] << " microsegundos\n";
        }
        outputFileKdTree.close();
        std::cout << "Resultados guardados en 'tiempos_busqueda_kdtree.txt'\n";
    }
    else {
        std::cerr << "Error al abrir el archivo de salida para KD-tree\n";
    }

    // Imprimir y guardar los tiempos para la búsqueda de fuerza bruta
    std::ofstream outputFileBruteForce("tiempos_busqueda_fuerza_bruta.txt");
    if (outputFileBruteForce.is_open()) {
        for (size_t i = 0; i < searchTimesBruteForce.size(); i++) {
            std::cout << "Tiempo de búsqueda fuerza bruta con " << files[i] << ": " << searchTimesBruteForce[i] << " microsegundos\n";
            outputFileBruteForce << "Tiempo de búsqueda fuerza bruta con " << files[i] << ": " << searchTimesBruteForce[i] << " microsegundos\n";
        }
        outputFileBruteForce.close();
        std::cout << "Resultados guardados en 'tiempos_busqueda_fuerza_bruta.txt'\n";
    }
    else {
        std::cerr << "Error al abrir el archivo de salida para fuerza bruta\n";
    }
}

void executionTimeSearchKdtree() {
    std::string files[3] = { "1000.csv", "10000.csv", "20000.csv" };
    std::vector<std::vector<long long>> searchTimesKdTree;  // Vector de vectores para almacenar los tiempos de búsqueda en el KD-tree

    // Rango de valores de n a probar
    std::vector<int> nValues = { 1, 5, 10, 25,20, 25 };

    for (size_t i = 0; i < 3; i++) {
        std::vector<Point> points = readPointsFromCSV(files[i]);

        // Crear KD-tree
        KdTree kdTree(points);

        // Punto de consulta (ajusta según tus necesidades)
        Point queryPoint(5, 5, 5);

        std::vector<long long> searchTimesKdTreeForFile;

        for (size_t j = 0; j < nValues.size(); j++) {
            // Medir tiempo para la búsqueda en el KD-tree
            auto startKdTree = std::chrono::high_resolution_clock::now();
            std::vector<Node*> nearestNeighborsKdTree;
            double currentBestDistanceKdTree = std::numeric_limits<double>::infinity();
            kdTree.kdtreeSearchnn(queryPoint, nearestNeighborsKdTree, currentBestDistanceKdTree, nValues[j]);
            auto endKdTree = std::chrono::high_resolution_clock::now();

            // Calcular y almacenar el tiempo transcurrido en la búsqueda en el KD-tree
            auto durationKdTree = std::chrono::duration_cast<std::chrono::microseconds>(endKdTree - startKdTree);
            searchTimesKdTreeForFile.push_back(durationKdTree.count());
        }

        searchTimesKdTree.push_back(searchTimesKdTreeForFile);
    }

    // Imprimir y guardar los tiempos para la búsqueda en el KD-tree
    std::ofstream outputFileKdTree("tiempos_busqueda_kdtree_n.txt");
    if (outputFileKdTree.is_open()) {
        for (size_t i = 0; i < searchTimesKdTree.size(); i++) {
            std::cout << "Resultados para KD-tree con " << files[i] << ":\n";
            outputFileKdTree << "Resultados para KD-tree con " << files[i] << ":\n";
            for (size_t j = 0; j < nValues.size(); j++) {
                std::cout << "Tiempo de búsqueda con n=" << nValues[j] << ": " << searchTimesKdTree[i][j] << " microsegundos\n";
                outputFileKdTree << "Tiempo de búsqueda con n=" << nValues[j] << ": " << searchTimesKdTree[i][j] << " microsegundos\n";
            }
        }
        outputFileKdTree.close();
        std::cout << "Resultados guardados en 'tiempos_busqueda_kdtree.txt'\n";
    }
    else {
        std::cerr << "Error al abrir el archivo de salida para KD-tree\n";
    }
}

void executionTimeSearchBruteForce() {
    std::string files[3] = { "1000.csv", "10000.csv", "20000.csv" };
    std::vector<std::vector<long long>> searchTimesBruteForce;  // Vector de vectores para almacenar los tiempos de búsqueda de fuerza bruta
    std::vector<long long> searchTimesBruteForceForFile;
    // Rango de valores de n a probar
   int nValues[6] = {1, 5, 10, 15, 20, 25};
    // Punto de consulta (ajusta según tus necesidades)
    Point queryPoint(5, 5, 5);

    for (size_t i = 0; i < 3; i++) {
        std::vector<Point> points = readPointsFromCSV(files[i]);
        
        for (int j = 0; j < 6; j++) {
            BruteForce bruteForceData(points);
            // Medir tiempo para la búsqueda de fuerza bruta
            auto startBruteForce = std::chrono::high_resolution_clock::now();
            
            std::vector<Point> nearestNeighborBruteForce = bruteForceData.bruteForceNearestNeighbors(queryPoint, nValues[j]);
            auto endBruteForce = std::chrono::high_resolution_clock::now();

            // Calcular y almacenar el tiempo transcurrido en la búsqueda de fuerza bruta
            auto durationBruteForce = std::chrono::duration_cast<std::chrono::microseconds>(endBruteForce - startBruteForce);
            searchTimesBruteForceForFile.push_back(durationBruteForce.count());
        }

        searchTimesBruteForce.push_back(searchTimesBruteForceForFile);
        searchTimesBruteForceForFile.clear();
    }
    

    // Imprimir y guardar los tiempos para la búsqueda de fuerza bruta
    std::ofstream outputFileBruteForce("tiempos_busqueda_fuerza_bruta_n.txt");
    if (outputFileBruteForce.is_open()) {
        for (size_t i = 0; i < searchTimesBruteForce.size(); i++) {
            std::cout << "Resultados para búsqueda de fuerza bruta con " << files[i] << ":\n";
            outputFileBruteForce << "Resultados para búsqueda de fuerza bruta con " << files[i] << ":\n";
            for (size_t j = 0; j < 6; j++) {
                std::cout << "Tiempo de búsqueda con n=" << nValues[j] << ": " << searchTimesBruteForce[i][j] << " microsegundos\n";
                outputFileBruteForce << "Tiempo de búsqueda con n=" << nValues[j] << ": " << searchTimesBruteForce[i][j] << " microsegundos\n";
            }
        }
        outputFileBruteForce.close();
        std::cout << "Resultados guardados en 'tiempos_busqueda_fuerza_bruta.txt'\n";
    }
    else {
        std::cerr << "Error al abrir el archivo de salida para búsqueda de fuerza bruta\n";
    }
}



void ejectionTimeSearchNNtoNpoints()
{
    std::string files[1] = { "20000.csv" };
    std::vector<std::vector<long long>> searchTimesKdTree;  // Vector de vectores para almacenar los tiempos de búsqueda en el KD-tree
    std::vector<std::vector<long long>> searchTimesBruteForce;  // Vector de vectores para almacenar los tiempos de búsqueda de fuerza bruta

    // Rango de valores de n a probar
    std::vector<int> nValues = { 1, 5, 10, 15, 20, 25 };

    for (size_t i = 0; i < 1; i++) {  // Solo se busca en "20000.csv"
        std::vector<Point> points = readPointsFromCSV(files[i]);

        // Crear KD-tree
        KdTree kdTree(points);

        // Punto de consulta (ajusta según tus necesidades)
        Point queryPoint(5, 5, 5);

        std::vector<long long> searchTimesKdTreeForFile;
        std::vector<long long> searchTimesBruteForceForFile;

        for (size_t j = 0; j < nValues.size(); j++) {
            // Medir tiempo para la búsqueda en el KD-tree
            auto startKdTree = std::chrono::high_resolution_clock::now();
            std::vector<Node*> nearestNeighborsKdTree;
            double currentBestDistanceKdTree = std::numeric_limits<double>::infinity();
            kdTree.kdtreeSearchnn(queryPoint, nearestNeighborsKdTree, currentBestDistanceKdTree, nValues[j]);
            auto endKdTree = std::chrono::high_resolution_clock::now();

            // Calcular y almacenar el tiempo transcurrido en la búsqueda en el KD-tree
            auto durationKdTree = std::chrono::duration_cast<std::chrono::microseconds>(endKdTree - startKdTree);
            searchTimesKdTreeForFile.push_back(durationKdTree.count());

            // Medir tiempo para la búsqueda de fuerza bruta
            BruteForce bruteForceData(points);
            auto startBruteForce = std::chrono::high_resolution_clock::now();
            std::vector<Point> nearestNeighborBruteForce = bruteForceData.bruteForceNearestNeighbors(queryPoint, nValues[j]);
            auto endBruteForce = std::chrono::high_resolution_clock::now();

            // Calcular y almacenar el tiempo transcurrido en la búsqueda de fuerza bruta
            auto durationBruteForce = std::chrono::duration_cast<std::chrono::microseconds>(endBruteForce - startBruteForce);
            searchTimesBruteForceForFile.push_back(durationBruteForce.count());
        }

        searchTimesKdTree.push_back(searchTimesKdTreeForFile);
        searchTimesBruteForce.push_back(searchTimesBruteForceForFile);
    }

    // Imprimir y guardar los tiempos para la búsqueda en el KD-tree
    std::ofstream outputFileKdTree("tiempos_busqueda_kdtree_n.txt");
    if (outputFileKdTree.is_open()) {
        for (size_t i = 0; i < searchTimesKdTree.size(); i++) {
            std::cout << "Resultados para KD-tree con " << files[i] << ":\n";
            outputFileKdTree << "Resultados para KD-tree con " << files[i] << ":\n";
            for (size_t j = 0; j < nValues.size(); j++) {
                std::cout << "Tiempo de búsqueda con n=" << nValues[j] << ": " << searchTimesKdTree[i][j] << " microsegundos\n";
                outputFileKdTree << "Tiempo de búsqueda con n=" << nValues[j] << ": " << searchTimesKdTree[i][j] << " microsegundos\n";
            }
        }
        outputFileKdTree.close();
        std::cout << "Resultados guardados en 'tiempos_busqueda_kdtree.txt'\n";
    }
    else {
        std::cerr << "Error al abrir el archivo de salida para KD-tree\n";
    }

    // Imprimir y guardar los tiempos para la búsqueda de fuerza bruta
    std::ofstream outputFileBruteForce("tiempos_busqueda_fuerza_bruta_n.txt");
    if (outputFileBruteForce.is_open()) {
        for (size_t i = 0; i < searchTimesBruteForce.size(); i++) {
            std::cout << "Resultados para búsqueda de fuerza bruta con " << files[i] << ":\n";
            outputFileBruteForce << "Resultados para búsqueda de fuerza bruta con " << files[i] << ":\n";
            for (size_t j = 0; j < nValues.size(); j++) {
                std::cout << "Tiempo de búsqueda con n=" << nValues[j] << ": " << searchTimesBruteForce[i][j] << " microsegundos\n";
                outputFileBruteForce << "Tiempo de búsqueda con n=" << nValues[j] << ": " << searchTimesBruteForce[i][j] << " microsegundos\n";
            }
        }
        outputFileBruteForce.close();
        std::cout << "Resultados guardados en 'tiempos_busqueda_fuerza_bruta.txt'\n";
    }
    else {
        std::cerr << "Error al abrir el archivo de salida para búsqueda de fuerza bruta\n";
    }
}


 int main(int argc, char** argv) {

    ejecutionTimeInsertionKdtree();

    //ejecutionTimeSearchNN();

    //ejectionTimeSearchNNtoNpoints();
    //executionTimeSearchKdtree();
    //executionTimeSearchBruteForce();



    //testing::InitGoogleTest(&argc, argv);
    //return RUN_ALL_TESTS();
    return 0;
}


