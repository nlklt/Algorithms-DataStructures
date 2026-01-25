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

	return 0;
}
