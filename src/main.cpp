#include <iostream>
#include "../include/utils.h"

int main(int argc, char **argv)
{
	std::cout << "You have entered " << argc - 1
			<< " arguments:" << std::endl;

	for (int i = 1; i < argc; i++)
	{
		std::cout << "Argument " << i - 1 << ": " << argv[i] << std::endl;
	}
	std::cout << argv[0] << std::endl;
	if (argc > 1)
	{
		memoizedSearch(strtol(argv[1], nullptr, 10), strtol(argv[2], nullptr, 10));
	}
	else
	{
		memoizedSearch(1,10000);
	}
}