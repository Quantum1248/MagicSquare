#include <iostream>
#include <vector>
#include <thread>
#include "../include/SearchAlg.h"

int main(int argc, char **argv)
{
	std::cout << "You have entered " << argc - 1
			<< " arguments:" << std::endl;

	for (int i = 1; i < argc; i++)
	{
		std::cout << "Argument " << i - 1 << ": " << argv[i] << std::endl;
	}
	
	if (argc > 1)
	{
		int threadToSpawn = strtol(argv[1], nullptr, 10);
		uint64_t min = strtol(argv[2], nullptr, 10);
		uint64_t max = strtol(argv[3], nullptr, 10);
		std::cout << "Searching from " << min << " to " << max << ", " <<max-min<< " numbers in total, with " << threadToSpawn << " threads."<<std::endl;
		uint64_t init = min;
		uint64_t module = (max - min) / threadToSpawn;
		uint64_t remainder = (max - min) % threadToSpawn;
		std::cout << "The first thread will check " << module + remainder << " number, while the others " << module << " numbers for a total of "
				  << remainder + module * threadToSpawn << " numbers." << std::endl;

		std::vector<std::thread> threads;

		threads.push_back(std::thread(MemoizedSearch, init, init + module + remainder, 0));
		init+=module+remainder;
		for (int i =1 ; i < threadToSpawn; i++)
		{
			threads.push_back(std::thread(MemoizedSearch, init, init + module, i));
			init += module;
		}
		std::cout << "All " << threadToSpawn << " threads have been initialized."<<std::endl;
		for (size_t i = 0; i < threads.size(); i++)
		{
			threads[i].join();
		}
		std::cout << "All " << threadToSpawn << " threads have finished."<<std::endl;
	}
	else
	{
		MemoizedSearch(0, 1000000, 0);
	}
}