#include <iostream>
#include <iomanip>
#include <math.h>
#include <chrono>
#include <thread>

const double e = 2.7182818284590452353602874713527;

double f(double x)
{
	x = x / 50;

	return x * (2 + pow(e, x*x + 2*x));
}

size_t percentage(double current, double maximum)
{
	//return 100 * current / maximum;
	return  100 * (f(current) / f(maximum));
}

int main()
{
	size_t cycle_size = 100;
	size_t bar_size = 100;
	size_t prev = -1;

	for (size_t i = 0; i <= cycle_size; i++)
	{
		size_t p = percentage(i, cycle_size);
		size_t b = p * bar_size / 100;

		if (p != prev)
		{
			std::cout << "\r" << std::setw(b+1) << std::setfill('#') << std::left << '[';
			
			std::cout << std::right << std::setw(1 + bar_size - b) << std::setfill('.') << ']' << ' ' << p << "%";

			prev = p;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(30));
	}
}
