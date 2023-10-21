#include <iostream>
#include <fstream>
#include <iomanip>

int main()
{
	std::ifstream input("input.txt");
	std::ofstream output("output.txt");
	
	size_t n, m;

	std::string s;
	while (input >> s)
		output << s << std::endl;

	input.close();
	output.close();

	return 0;
}
