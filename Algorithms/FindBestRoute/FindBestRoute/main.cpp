#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <numeric>
#include <iomanip>
#include <fstream>

const double MUTATION_RATE = 0.3; 

std::random_device rd;
std::mt19937 gen(rd());

struct Individual {
    std::vector<int> path;
    int fitness; 
};

std::vector<std::vector<int>> createGraph(int N) {
    std::uniform_int_distribution<> weight_dist(1, 99);
    std::vector<std::vector<int>> matrix(N, std::vector<int>(N));

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (i == j) matrix[i][j] = 0;
            else matrix[i][j] = weight_dist(gen);
        }
    }
    return matrix;
}

int calculateFitness(const std::vector<int>& path, const std::vector<std::vector<int>>& matrix) {
    int dist = 0;
    for (size_t i = 0; i < path.size() - 1; ++i) {
        dist += matrix[path[i]][path[i + 1]];
    }
    return dist;
}

std::vector<Individual> createInitialPopulation(const std::vector<std::vector<int>>& matrix, int POPULATION_SIZE) {
    std::vector<Individual> population;
    std::vector<int> base_path(matrix.size());
    std::iota(base_path.begin(), base_path.end(), 0);

    for (int i = 0; i < POPULATION_SIZE; ++i) {
        std::shuffle(base_path.begin(), base_path.end(), gen);
        base_path.push_back(base_path[0]);
        population.push_back({ base_path, calculateFitness(base_path, matrix) });
        base_path.erase(base_path.begin() + base_path.size() - 1);
    }
    return population;
}

std::vector<int> crossover(const std::vector<int>& p1, const std::vector<int>& p2) {
    int child_size = p1.size();
    std::vector<int> child(child_size, -1);

    int idx_size = p1.size() - 2;
    std::uniform_int_distribution<> dist_p(0, idx_size);
    int start = dist_p(gen);
    int end = dist_p(gen);
    if (start > end) std::swap(start, end);

    for (int i = start; i <= end; ++i) {
        child[i] = p1[i];
    }

    int p2_idx = 0;
    for (int i = 0; i < child.size() - 1; ++i) {
        if (i < start || i > end || child[i] == -1) {
            while (p2_idx < idx_size - 1 && std::find(child.begin(), child.end(), p2[p2_idx]) != child.end()) { p2_idx++; }
            if (child[i] == -1) {
                child[i] = p2[p2_idx];
            }
        }
    }
    child[child_size - 1] = child[0];
    return child;
}

void mutate(std::vector<int>& path) {
    std::uniform_real_distribution<> dist(0.0, 1.0);
    if (dist(gen) < MUTATION_RATE) {
        std::uniform_int_distribution<> idx_dist(0, path.size() - 2);
        int si = idx_dist(gen);
        int i = si;
        int j = si + 1;
        std::swap(path[i], path[j]);
    }
}

bool isEdgeInPath(int u, int v, const std::vector<int>& bestPath) {
    for (size_t i = 0; i + 1 < bestPath.size(); ++i) {
        int a = bestPath[i];
        int b = bestPath[i + 1];
        if ((a == u && b == v) || (a == v && b == u)) return true;
    }
    return false;
}

void writeDotFiles(const std::vector<std::vector<int>>& matrix, const std::vector<int>& bestPath,
    const std::string& file_plain, const std::string& file_highlighted) {

    int N = matrix.size();

    {
        std::ofstream fo(file_plain);
        fo << "graph G {\n";
        fo << "  node [shape=circle, style=filled, fillcolor=white];\n";
        for (int i = 0; i < N; ++i) {
            fo << "  " << (char)('A' + i) << ";\n";
        }
        for (int i = 0; i < N; ++i) {
            for (int j = i + 1; j < N; ++j) {
                fo << "  " << (char)('A' + i) << " -- " << (char)('A' + j)
                    << " [label=\"" << matrix[i][j] << "\"];\n";
            }
        }
        fo << "}\n";
    }

    {
        std::ofstream fo(file_highlighted);
        fo << "graph G {\n";
        fo << "  node [shape=circle, style=filled, fillcolor=white];\n";
        for (int i = 0; i < N; ++i) {
            fo << "  " << (char)('A' + i) << ";\n";
        }
        for (int i = 0; i < N; ++i) {
            for (int j = i + 1; j < N; ++j) {
                bool inPath = isEdgeInPath(i, j, bestPath);
                fo << "  " << (char)('A' + i) << " -- " << (char)('A' + j)
                    << " [label=\"" << matrix[i][j] << "\"";
                if (inPath) {
                    fo << ", color=green, penwidth=3";
                }
                fo << "];\n";
            }
        }
        fo << "}\n";
    }
}

int main() {
    int N = 8;   
    int POPULATION_SIZE = 5; 
    int GENERATIONS = 100;   
    int CHILD_COUNT = 8;   

    // std::cout << "Population size:\n  ";
    // std::cin >> POPULATION_SIZE;
    // std::cout << "Count evolutions:\n  ";
    // std::cin >> GENERATIONS;
    // std::cout << "Count childs:\n  ";
    // std::cin >> CHILD_COUNT;

    auto matrix = createGraph(N);

    std::cout << "Distance cities:\n  ";
    for (int i = 0; i < N; ++i) std::cout << (char)('A' + i) << "  ";
    std::cout << "\n";
    for (int i = 0; i < N; ++i) {
        std::cout << (char)('A' + i) << ' ';
        for (int j = 0; j < N; ++j) {
            std::cout << std::setw(3) << std::left << matrix[i][j];
        }
        std::cout << "\n";
    }
    std::cout << "\n";

    auto population = createInitialPopulation(matrix, POPULATION_SIZE);

    for (int generation = 1; generation <= GENERATIONS; ++generation) {

        std::sort(population.begin(), population.end(), [](const Individual& a, const Individual& b) {
            return a.fitness < b.fitness;
            });

        if (generation % 5 == 0) {
            std::cout << "Generation " << std::setw(4) << std::left << generation;
            std::cout << "[" << (char)('A' + population[0].path[0]);
            for (size_t i = 1; i < population[0].path.size(); ++i) {
                std::cout << " -> " << (char)('A' + population[0].path[i]);
            }
            std::cout << "] Best Dist: " << population[0].fitness << "\n";
        }

        std::vector<Individual> new_population;

        new_population.push_back(population[0]);
        new_population.push_back(population[1]);

        while (new_population.size() < CHILD_COUNT) {
            std::uniform_int_distribution<> pop_dist(0, static_cast<int>(POPULATION_SIZE - 3));

            int ch = pop_dist(gen);
            const auto& parent1 = population[ch];
            const auto& parent2 = population[ch + 1];

            std::vector<int> child_path = crossover(parent1.path, parent2.path);
            mutate(child_path);

            new_population.push_back({ child_path, calculateFitness(child_path, matrix) });
        }

        population = new_population;
    }

    // –≈«”À‹“¿“
    std::sort(population.begin(), population.end(), [](const Individual& a, const Individual& b) {
        return a.fitness < b.fitness;
        });

    std::cout << "\nFinal Best Route:\n";
    std::cout << "Distance: " << population[0].fitness << "\nPath: ";

    std::cout << (char)('A' + population[0].path[0]);
    for (size_t i = 1; i < population[0].path.size(); ++i) {
        std::cout << " -> " << (char)('A' + population[0].path[i]);
    }

    writeDotFiles(matrix, population[0].path, "graph.dot", "graph_highlighted.dot");
    std::cout << "Wrote graph.dot and graph_highlighted.dot\n";

    return 0;
}