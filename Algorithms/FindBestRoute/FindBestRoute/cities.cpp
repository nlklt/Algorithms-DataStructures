#include "cities.h"
#include <iostream>
#include <random>

static std::random_device rd;
static std::mt19937 gen(rd());
static std::uniform_int_distribution<> weight_dist(1, 9);

bool DeleteCity(graph& matrix, char city) {
    int idx = -1;
    for (int i = 1; i < matrix.size(); i++)
    {
        if (matrix[0][i] == city)
        {
            idx = i;
            break;
        }
    }
    if (idx == -1) {
        std::cout << "City not found\n";
        return false;
    }

    matrix.erase(matrix.begin() + idx);
    for (auto& row : matrix) {
            row.erase(row.begin() + idx);
    }

    std::cout << "City deleted\n";
    return true;
}

void PrintCities(const graph& matrix) {
    int n = matrix.size();

    for (int i = 0; i < n; ++i) std::cout << char(matrix[0][i]) << ' ';
    std::cout << '\n';

    for (int i = 1; i < n; ++i) {
        std::cout << char(matrix[i][0]) << ' ';
        for (int j = 1; j < n; ++j) {
            std::cout << matrix[i][j] << ' ';
        }
        std::cout << '\n';
    }
}

void AddCity(graph& matrix, char city) {

    for (int i = 0; i < matrix.size(); ++i) {
        if (matrix[0][i] == city) {
            std::cout << "City alredy exist\n";
            return;
        }
    }

    int idx = matrix.size();
    int new_size = matrix.size() + 1;
    matrix.resize(new_size);
    
    for (int i = 0; i < new_size; ++i) {
        matrix[i].resize(new_size, 0);
    }

    for (int i = 1; i < new_size; ++i) {
        for (int j = 1; j < new_size; ++j) {
            if (i == j) {
                matrix[i][j] = 0;
            }
            else if (i == idx || j == idx) {
                matrix[i][j] = weight_dist(gen);
            }
        }
    }
    matrix[0][idx] = city;
    matrix[idx][0] = city;

    std::cout << "City added\n";
}
