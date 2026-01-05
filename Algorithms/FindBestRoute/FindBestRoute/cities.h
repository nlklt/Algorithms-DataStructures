#pragma once

#include <vector>

typedef std::vector<std::vector<int>> graph;

bool DeleteCity(graph& matrix, char city);
void PrintCities(const graph& matrix);
void AddCity(graph& matrix, char city);
