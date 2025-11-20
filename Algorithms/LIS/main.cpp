#include "lis_algorithm.h"
#include "utils.h"

#include <iostream>
#include <windows.h>
#include <fcntl.h>
#include <io.h>

int main()
{
	SetConsoleOutputCP(65001);

	// простой тест
	std::vector<int> data = { 5, 10, 6, 12, 3, 24, 7, 8 };
	
	std::cout << "\n\n--- running O(n2) solution ---\n";
	std::vector<int> lis_n2;
	lis_n2 = getLIS_n2(data);
	std::cout << "lis: " << lis_n2.size() << "\n-> ";
	printVector(lis_n2);
	
	
	std::cout << "\n\n--- running O(n*log(n)) solution ---\n";
	std::vector<int> lis_nlogn;
	lis_nlogn = getLIS_nlogn(data);
	std::cout << "lis: " << lis_nlogn.size() << "\n-> ";
	printVector(lis_nlogn);

	//// БЕНЧМАРК
	//const size_t n = 1000;
	//std::vector<int> bigData = generateRandomVector(n);
	//std::cout << "data generated.";

	//std::cout << "\n\n--- running O(n2) solution ---\n";
	//std::vector<int> lis_n2;
	//auto start_n2 = std::chrono::high_resolution_clock::now();
	//lis_n2 = getLIS_n2(bigData);
	//auto end_n2 = std::chrono::high_resolution_clock::now();
	//std::chrono::duration<double, std::milli> elapsed_n2 = end_n2 - start_n2;
	//std::cout << "lis: " << lis_n2.size() << ", time: ";
	//printTime(end_n2 - start_n2);
	////std::cout << "\n-> "; 
	////printVector(lis_n2);


	//std::cout << "\n\n--- running O(n*log(n)) solution ---\n";
	//std::vector<int> lis_nlogn;
	//auto start_nlogn = std::chrono::high_resolution_clock::now();
	//lis_nlogn = getLIS_nlogn(bigData);
	//auto end_nlogn = std::chrono::high_resolution_clock::now();
	//std::chrono::duration<double, std::milli> elapsed_nlogn = end_nlogn - start_nlogn;
	//std::cout << "lis: " << lis_nlogn.size() << ", time: ";
	//printTime(end_nlogn - start_nlogn);
	////std::cout << "\n-> "; 
	////printVector(lis_nlogn);

	//// сравнение результатов (они должны быть одинаковой длины)
	//if (lis_n2.size() == lis_nlogn.size())
	//{
	//	std::cout << "\n\nSUCCESS: both algorithms found lis of length " << lis_n2.size() << "\n"
	//		<< "math difference " << (size_t)((n * n) / (n * log2(n))) << "-fold\n"
	//		<< "practic difference " << (size_t)(elapsed_n2 / elapsed_nlogn) << "-fold\n~"
	//		<< (size_t)((elapsed_n2 / elapsed_nlogn) / ((n * n) / (n * log2(n))) * 100) << " percent discrepancy\n";
	//}
	//else
	//{
	//	std::cout << "\n\nERROR: lengths differ!\n";
	//}

	return 0;
}
