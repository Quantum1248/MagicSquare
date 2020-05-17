#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <filesystem>

typedef std::chrono::high_resolution_clock Clock;
#include "../include/SearchAlg.h"
#include "../include/MSKContainer.h"
#include "../include/Merger.h"
#include "../include/Tester.h"
#include "../include/filepath.h"
#include "../include/StringMan.h"

std::vector<std::string> Search(uint64_t min, uint64_t max, int threadToSpawn)
{
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
	return paths;
}

int main()
{
	std::string opt = "";
    while (opt != "1" && opt != "2" && opt != "3" && opt != "4")
    {
        std::cout << "Cosa vuoi fare(1/2)?" << std::endl
                << "1 -Genera nuovi risultati." << std::endl
                << "2 -Test risultati." << std::endl
                << "3 -Accorpa risultati." << std::endl
                << "4 -Genera nuovi risultati con impostazioni automatiche." << std::endl;
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

		uint64_t modulo = 20000000;
		uint64_t i = min;
		std::vector<std::string> paths;
		std::vector<std::string> pathsTmp;



		auto t1 = Clock::now();
		while (i + modulo < max)
		{
			pathsTmp=Search(i, i + modulo, threadToSpawn);
			for (size_t j = 0; j < pathsTmp.size(); j++)
			{
				paths.push_back(pathsTmp[j]);
			}
			i += modulo;
		}
		pathsTmp = Search(i, max, threadToSpawn);
		for (size_t j = 0; j < pathsTmp.size(); j++)
		{
			paths.push_back(pathsTmp[j]);
		}

		auto t2 = Clock::now();
		std::cout << "Work finished in "
				  << std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count() / 1000000000
				  << " seconds." << std::endl;
		
		

		std::cout << "Vuoi accorpare i risultati?(S/n)" << std::endl;
		bool acc = true;
		opt = "";
		//std::cin >> opt;
        opt = "s";
        while (opt != "s" && opt != "n" && opt != "S" && opt != "N" && opt != "")
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
			//std::cin >> opt;
            opt = "s";
            while (opt != "s" && opt != "n" && opt != "S" && opt != "N" && opt != "")
            {
				std::cout << "Digita s o n." << std::endl;
				std::cin >> opt;
			}
			if (opt == "n" || opt == "N")
				clean = false;
			
			size_t pos = 0;
			mpz_class minN = NextNumber(paths[0], pos), maxN = NextNumber(paths[0], pos);
			for (size_t i = 0; i < paths.size(); i++)
			{
				pos = 0;
				mpz_class tmpMin, tmpMax;
				tmpMin = NextNumber(paths[i], pos);
				tmpMax = NextNumber(paths[i], pos);
                paths[i] = FILE_BASE_PATH + paths[i];
                if (tmpMin < minN)
					minN = tmpMin;
				if (tmpMax > maxN)
					maxN = tmpMax;
			}

            std::string resPath = FILE_BASE_PATH + minN.get_str() + "-" + maxN.get_str() + ".txt";
            std::cout << "Merging files in " << resPath << std::endl;
			t1 = Clock::now();
			Merge(paths, resPath, clean);
			t2 = Clock::now();
			std::cout << "All files merged in "
					  << std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count() / 1000000000
					  << " seconds." << std::endl;
		}
	}
	else if(opt=="2")
	{
		std::string nomeFile = "/run/media/manuel/29703f9d-5247-4b9b-8a98-bc007a395130/resfin.txt";
		std::cout << "Test 1..." << std::endl;
		test(nomeFile);
		std::cout << "Test 2..." << std::endl;
		test2(nomeFile);
	}
	else if(opt=="3")
	{
		std::cout << "Merging files in " << "res.txt" << std::endl;
		auto t1 = Clock::now();
		std::vector<std::string> paths;
		std::string base = FILE_BASE_PATH;
        for (const auto &entry : std::filesystem::directory_iterator(base))
        {
            paths.push_back(entry.path());
        }
        std::sort(paths.begin(), paths.end());
        mpz_class beg;
        mpz_class end = 11000000000;
/*
        for (const std::string &i : paths)
        {
            std::string filename = i.substr(FILE_BASE_PATH.size(), i.size() - FILE_BASE_PATH.size());
            size_t pos = 0;
            //std::cout << i << std::endl;
            beg = NextNumber(filename, pos);
            //std::cout << beg << "," << end << std::endl;
            if(beg!=end)
            {
                std::cout << "Mancano file" << std::endl;
                return 1;
            }
            end = NextNumber(filename, pos);
        }
*/
        std::vector<std::string> cent;
        int i = 1;
        size_t fileToMerge = 100;
        std::reverse(paths.begin(), paths.end());
        while (paths.size() > 0)
        {
            cent.resize(0);
            while (cent.size() < fileToMerge && paths.size() > 0)
            {
                cent.push_back(paths.back());
                paths.pop_back();
            }
            std::cout << "Merging " << cent.size() << " files in pass " << i << " with names:" << std::endl;
            for (const std::string &e : cent)
            {
                std::cout << e << std::endl;
            }
            Merge(cent, "/run/media/manuel/MS2/" + std::to_string(i) + ".txt", false);
            i++;
        }
        //Merge(paths, base + "resfin.txt", false);
        auto t2 = Clock::now();
        std::cout << "All files merged in "
                    << i<< " passes and "
                    << std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count() / 1000000000
                    << " seconds." << std::endl;
        /*
		std::cout << "Merging files in res.txt" << std::endl;
		auto t1 = Clock::now();
		std::vector<std::string> paths;
		std::string base = FILE_BASE_PATH;
        for (const auto &entry : std::filesystem::directory_iterator(base))
        {
            paths.push_back(entry.path());
        }
        std::sort(paths.begin(), paths.end());
        mpz_class beg;
        mpz_class end = 11000000000;

        for (const std::string &i : paths)
        {
            std::string filename = i.substr(FILE_BASE_PATH.size(), i.size() - FILE_BASE_PATH.size());
            size_t pos = 0;
            //std::cout << i << std::endl;
            beg = NextNumber(filename, pos);
            //std::cout << beg << "," << end << std::endl;
            if(beg!=end)
            {
                std::cout << "Mancano file" << std::endl;
                return 1;
            }
            end = NextNumber(filename, pos);
        }

        Merge(paths, "/run/media/manuel/29703f9d-5247-4b9b-8a98-bc007a395130/half/res.txt", false);
        //Merge(paths, base + "resfin.txt", false);
        auto t2 = Clock::now();
        std::cout << "All files merged in "
                  << std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count() / 1000000000
                  << " seconds." << std::endl;
                  */
    }
    else if(opt=="4")
    {
        int threadToSpawn = 12;
        std::cout << "Quanti thread vuoi usare?" << std::endl;
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
        std::cout << "Inserisci il limite inferiore." << std::endl;
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
        std::cout << "Inserisci il limite superiore." << std::endl;
        opt = "";
        std::cin >> opt;
        while (!(is_number(opt) || opt == ""))
        {
            std::cout << "Inserisci un numero." << std::endl;
            std::cin >> opt;
        }
        if (opt != "")
            max = std::strtoul(opt.c_str(), nullptr, 10);

        uint64_t modulo = 20000000;
        uint64_t i = min;
        std::vector<std::string> paths;
        std::vector<std::string> pathsTmp;

        auto t1 = Clock::now();
        while (i + modulo < max)
        {
            pathsTmp = Search(i, i + modulo, threadToSpawn);
            for (size_t j = 0; j < pathsTmp.size(); j++)
            {
                paths.push_back(pathsTmp[j]);
            }
            i += modulo;
        }
        pathsTmp = Search(i, max, threadToSpawn);
        for (size_t j = 0; j < pathsTmp.size(); j++)
        {
            paths.push_back(pathsTmp[j]);
        }

        auto t2 = Clock::now();
        std::cout << "Work finished in "
                  << std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count() / 1000000000
                  << " seconds." << std::endl;
        bool accorpare = false;
        if (accorpare)
        {
            std::cout << "Vuoi cancellare i file dei risultati non accorpati?(S/n)" << std::endl;
            bool clean = true;
            opt = "";
            //std::cin >> opt;
            opt = "s";
            while (opt != "s" && opt != "n" && opt != "S" && opt != "N" && opt != "")
            {
                std::cout << "Digita s o n." << std::endl;
                std::cin >> opt;
            }
            if (opt == "n" || opt == "N")
                clean = false;

            size_t pos = 0;
            mpz_class minN = NextNumber(paths[0], pos), maxN = NextNumber(paths[0], pos);
            for (size_t i = 0; i < paths.size(); i++)
            {
                pos = 0;
                mpz_class tmpMin, tmpMax;
                tmpMin = NextNumber(paths[i], pos);
                tmpMax = NextNumber(paths[i], pos);
                paths[i] = FILE_BASE_PATH + paths[i];
                if (tmpMin < minN)
                    minN = tmpMin;
                if (tmpMax > maxN)
                    maxN = tmpMax;
            }

            std::string resPath = FILE_BASE_PATH + minN.get_str() + "-" + maxN.get_str() + ".txt";
            std::cout << "Merging files in " << resPath << std::endl;
            t1 = Clock::now();
            Merge(paths, resPath, clean);
            t2 = Clock::now();
            std::cout << "All files merged in "
                      << std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count() / 1000000000
                      << " seconds." << std::endl;
        }
    }
}