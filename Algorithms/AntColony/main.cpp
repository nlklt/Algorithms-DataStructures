#include <iostream>
#include <vector>
#include <limits>
#include <random>
#include <cmath>
#include <iomanip>
#include <algorithm>

struct AntPath {
    std::vector<int> cities;
    double length = 0.0;
};

void printMatrix(const std::string& name, const std::vector<std::vector<int>>& matrix, int n) {
    std::cout << name << '\n';
    std::cout << "   ";
    for (int i = 0; i < n; i++) {
        std::cout << std::setw(3) << (char)('A' + i) << ' ';
    }
    std::cout << '\n';
    for (int i = 0; i < n; i++) {
        std::cout << std::setw(2) << (char)('A' + i) << ' ';
        for (int j = 0; j < n; j++) {
            std::cout << std::setw(3) << matrix[i][j] << ' ';
        }
        std::cout << '\n';
    }
    std::cout << '\n';
}

int selectNextCity(
    int curr_city,
    int n_cities,
    const std::vector<std::vector<double>>& pheromones,
    const std::vector<std::vector<int>>& dists,
    const std::vector<bool>& visited,
    double alpha,
    double beta,
    std::mt19937& gen
) {
    std::vector<double> probs(n_cities, 0.0);
    double sum_prob = 0.0;

    for (int i = 0; i < n_cities; ++i) {
        if (!visited[i]) {
            double d = (double)dists[curr_city][i];
            double visibility = 1.0 / d;

            double tau = std::pow(pheromones[curr_city][i], alpha);
            double eta = std::pow(visibility, beta);

            probs[i] = tau * eta;
            sum_prob += probs[i];
        }
    }

    std::uniform_real_distribution<> dist_prob(0.0, sum_prob);
    double r = dist_prob(gen);
    double current_slice = 0.0;

    for (int i = 0; i < n_cities; ++i) {
        if (!visited[i]) {
            current_slice += probs[i];
            if (r <= current_slice) {
                return i;
            }
        }
    }

    return -1;
}

int main() {
    setlocale(LC_ALL, "rus");
    std::random_device rd;
    std::mt19937 gen(rd());

    int N;
    std::cout << "Введите количество городов (N): ";
    std::cin >> N;

    std::vector<std::vector<int>> cities_matrix(N, std::vector<int>(N));
    std::uniform_int_distribution<> dist_width(10, 100);

    for (int i = 0; i < N; ++i) {
        for (int j = i; j < N; ++j) {
            if (i == j) {
                cities_matrix[i][j] = 0;
            }
            else {
                int d = dist_width(gen);
                cities_matrix[i][j] = d;
                cities_matrix[j][i] = d;
            }
        }
    }

    printMatrix("Матрица расстояний:", cities_matrix, N);

    double initial_pheromones, alpha, beta, evaporation_rate;
    int iterations_count;
    double Q = 1000.0;

    std::cout << "Введите начальное значение феромонов (например, 0.2): ";
    std::cin >> initial_pheromones;
    std::cout << "Введите Alpha (вес феромона, обычно 1.0): ";
    std::cin >> alpha;
    std::cout << "Введите Beta  (вес дистанции, обычно 2.0 - 5.0): ";
    std::cin >> beta;
    std::cout << "Введите коэффициент испарения (0.0 - 1.0): ";
    std::cin >> evaporation_rate;
    std::cout << "Введите количество итераций: ";
    std::cin >> iterations_count;

    std::vector<std::vector<double>> pheromones_matrix(N, std::vector<double>(N, initial_pheromones));

    double global_best_length = std::numeric_limits<double>::max();
    std::vector<int> global_best_path;

    std::cout << "\n--- Начало поиска ---\n";

    for (int iter = 0; iter < iterations_count; ++iter) {

        std::vector<AntPath> colony_paths;

        for (int ant = 0; ant < N; ++ant) {
            AntPath current_ant;
            std::vector<bool> visited(N, false);

            int start_node = ant;
            int curr_node = start_node;

            current_ant.cities.push_back(curr_node);
            visited[curr_node] = true;

            for (int step = 0; step < N - 1; ++step) {
                int next_node = selectNextCity(
                    curr_node, N, pheromones_matrix, cities_matrix, visited, alpha, beta, gen
                );

                current_ant.length += cities_matrix[curr_node][next_node];
                current_ant.cities.push_back(next_node);
                visited[next_node] = true;
                curr_node = next_node;
            }

            current_ant.length += cities_matrix[curr_node][start_node];

            colony_paths.push_back(current_ant);
        }

        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                pheromones_matrix[i][j] *= (1.0 - evaporation_rate);
            }
        }

        for (const auto& ant : colony_paths) {
            double delta_tau = Q / ant.length;
            for (size_t i = 0; i < ant.cities.size() - 1; ++i) {
                int from = ant.cities[i];
                int to = ant.cities[i + 1];
                pheromones_matrix[from][to] += delta_tau;
                pheromones_matrix[to][from] += delta_tau;
            }
            int last = ant.cities.back();
            int first = ant.cities.front();
            pheromones_matrix[last][first] += delta_tau;
            pheromones_matrix[first][last] += delta_tau;
        }

        for (const auto& ant : colony_paths) {
            if (ant.length < global_best_length) {
                global_best_length = ant.length;
                global_best_path = ant.cities;
            }
        }

        std::cout << "Итерация " << std::setw(3) << iter + 1
            << " | Лучшая длина: " << std::fixed << std::setprecision(2) << global_best_length
            << " | Маршрут: ";

        for (size_t i = 0; i < global_best_path.size(); ++i) {
            std::cout << (char)('A' + global_best_path[i]);
            if (i < global_best_path.size() - 1) std::cout << "-";
        }
        std::cout << "-" << (char)('A' + global_best_path[0]);
        std::cout << '\n';
    }

    std::cout << "\n----------------------------------------\n";
    std::cout << "Финальный результат:\n";
    std::cout << "Длина: " << global_best_length << '\n';
    std::cout << "Путь: ";
    for (int city : global_best_path) {
        std::cout << (char)('A' + city) << " -> ";
    }
    std::cout << (char)('A' + global_best_path[0]);
    std::cout << "\n----------------------------------------\n";

    return 0;
}