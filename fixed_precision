#include <iostream>
#include <fstream>
#include <iomanip>

int main()
{
	std::ifstream file("input.txt");
	
	double d;
	while (file >> d)
		std::cout << std::setprecision(3) << std::fixed << d << std::endl;

	file.close();

	return 0;
}
