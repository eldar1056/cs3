#include <iostream>
#include <fstream>
#include <iomanip>

int main()
{
	std::ifstream file("input.txt");
	
	size_t n, m;

	file >> n >> m;

	std::cout << std::right;

	for (size_t i = 0; i < n; i++)
	{
		for (size_t j = 0; j < m; j++)
		{
			int a;
			file >> a;

			if (j < m - 1)
			{
				char c;
				file >> c;
			}

			std::cout << std::setw(10) << a;

			if (j < m - 1)
				std::cout << ' ';
		}
		std::cout << std::endl;
	}

	file.close();

	return 0;
}
