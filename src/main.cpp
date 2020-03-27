#include <iostream>
#include <vector>
#include <thread>
#include <chrono>

typedef std::chrono::high_resolution_clock Clock;
#include "../include/SearchAlg.h"
#include "../include/MSKContainer.h"

bool is_number(const std::string &s)
{
	return std::find_if(s.begin(),
								s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
}

int main(int argc, char **argv)
{
	std::string opt = "";
	while (opt != "1" && opt != "2")
	{
		std::cout << "Cosa vuoi fare(1/2)?" << std::endl
				  << "1 -Genera nuovi risultati." << std::endl
				  << "2 -Compatta risultati." << std::endl;
		std::cin >> opt;
		if(opt!="1" && opt!="2")
			std::cout << "Sbagliato." << std::endl;
	}
	if(opt=="1")
	{
		int threadToSpawn = 12;
		std::cout << "Quanti thread vuoi usare?(Default=" << threadToSpawn << ")" << std::endl;
		opt = "";
		std::cin >> opt;
		while (!(is_number(opt) || opt == ""))
		{
			std::cout << "Inserisci un numero." << std::endl;
			std::cin >> opt;
		}
		if (opt != "")
			threadToSpawn = std::strtoul(opt.c_str(), nullptr, 10);

		uint64_t min = 0;
		std::cout << "Seleziona l'intervallo." << std::endl;
		std::cout << "Inserisci il limite inferiore.(Default=" << min << ")" << std::endl;
		opt = "";
		std::cin >> opt;
		while (!(is_number(opt) || opt == ""))
		{
			std::cout << "Inserisci un numero." << std::endl;
			std::cin >> opt;
		}
		if (opt != "")
			min = std::strtoul(opt.c_str(), nullptr, 10);

		uint64_t max = 1000000;
		std::cout << "Inserisci il limite superiore.(Default=" << max << ")" << std::endl;
		opt = "";
		std::cin >> opt;
		while (!(is_number(opt) || opt == ""))
		{
			std::cout << "Inserisci un numero." << std::endl;
			std::cin >> opt;
		}
		if (opt != "")
			max = std::strtoul(opt.c_str(), nullptr, 10);

		std::cout << "Searching from " << min << " to " << max << ", " << max - min << " numbers in total, with " << threadToSpawn << " threads." << std::endl;
		uint64_t init = min;
		uint64_t module = (max - min) / threadToSpawn;
		uint64_t remainder = (max - min) % threadToSpawn;
		std::cout << "The first thread will check " << module + remainder << " number, while the others " << module << " numbers each for a total of "
				  << remainder + module * threadToSpawn << " numbers." << std::endl;

		auto t1 = Clock::now();
		std::vector<std::thread> threads;

		std::vector<std::string> paths;
		threads.push_back(std::thread(MemoizedSearch, init, init + module + remainder, 0));
		paths.push_back(std::to_string(init) + "-" + std::to_string(init + module + remainder) + ".txt");
		init += module + remainder;
		for (int i = 1; i < threadToSpawn; i++)
		{
			threads.push_back(std::thread(MemoizedSearch, init, init + module, i));
			paths.push_back(std::to_string(init) + "-" + std::to_string(init + module) + ".txt");
			init += module;
		}
		std::cout << "All " << threadToSpawn << " threads have been initialized." << std::endl;
		for (size_t i = 0; i < threads.size(); i++)
		{
			threads[i].join();
		}
		auto t2 = Clock::now();
		std::cout << "All " << threadToSpawn << " threads have finished in "
		<< std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count() / 1000000000
		<< " seconds." << std::endl;
		std::cout << "Vuoi accorpare i risultati?(S/n)" << std::endl;
		bool acc = true;
		opt = "";
		std::cin >> opt;
		while (opt != "s" && opt!="n" && opt!="S"&&opt!="N"&&opt!="")
		{
			std::cout << "Digita s o n." << std::endl;
			std::cin >> opt;
		}
		if(opt=="n" || opt=="N")
			acc = false;
		if(acc)
		{
			std::cout << "Vuoi cancellare i file dei risultati non accorpati?(S/n)" << std::endl;
			bool clean = true;
			opt = "";
			std::cin >> opt;
			while (opt != "s" && opt != "n" && opt != "S" && opt != "N" && opt != "")
			{
				std::cout << "Digita s o n." << std::endl;
				std::cin >> opt;
			}
			if (opt == "n" || opt == "N")
				clean = false;
			MSKContainer c;
			c.Load(paths,clean);
			c.Save();
		}
	}
}