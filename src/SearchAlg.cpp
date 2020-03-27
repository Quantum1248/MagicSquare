#include "../include/SearchAlg.h"
#include "MSKContainer.h"

void MemoizedSearch(uint64_t min, uint64_t max, int thNumber)
{

    std::cout << "Thread " << thNumber << " has started with min="<<min<<" and max="<<max << std::endl;
    int percentage;
    percentage = -1;
    
    MSKContainer container(min,max);
    mpz_class a = 0, b = 0, apb = 0, amb = 0, n = 0, oddKey = 0, evenKey = 0;
    for (uint64_t i = min; i < max; i++)
    {
        CoprimePair(i, a, b);
        apb = a + b;
        amb = a - b;
        n = a * b * apb * amb;
        //n/a maggiore uguale ad a
        if (b * apb >= a * amb)
        {
            FindOddFirmTris(a, b, apb, amb, oddKey);

            mpz_divexact(evenKey.get_mpz_t(), n.get_mpz_t(), oddKey.get_mpz_t());
            container.Insert(oddKey, evenKey, a*amb, b*amb);
        }
        if (int(((i - min) * 100) / (max - min)) > percentage)
        {
            percentage = (int)(((i - min) * 100) / (max - min));
            std::cout << "Thread " << thNumber << " at: " << percentage << "%" << std::endl;
        }
    }
    std::cout << "Thread " << thNumber << " at: 100%" << std::endl;

    std::cout << "Thread " << thNumber << " is saving the file..." << std::endl;
    container.Save();

    std::cout<< "Thread " << thNumber << " has finished!" << std::endl;
    /*
    std::cout << "Ricerca quadrati con " << counter << " numeri:" << std::endl;
    uint64_t checkCounter = 0;
    percentage = -1;
    for (auto i = m.begin(); i != m.end(); ++i)
    {
        std::vector<mpz_class> cent;
        memoizedCenter(i->first, i->second, cent);
        reverseCenter(cent);
        mpz_class c = i->first * i->second;
        for (auto it1 = cent.begin(); it1 != cent.end(); it1 += 2)
        {
            mpz_class in = c / (*it1);
            in -= (*it1);
            in += 1;
            for (auto it2 = it1 + 2; it2 != cent.end(); it2 += 2)
            {
                mpz_class fn = c / (*it2);
                fn += (*it2);
                fn -= 1;

                mpz_class D = ((in-2) +1) * ((in-2) - 1 + 2); //(in-2+1)*(in-2-1+2)
                D /= 4;
                mpz_class e = (fn + in) * (fn - in + 2);
                e /= 4;
                check(D, e, c);
            }
        }

        checkCounter++;
        if ((int)((checkCounter * 100) / counter) > percentage)
        {
            percentage = (int)((checkCounter*100)/counter);
            std::cout << percentage << "%" << std::endl;
        }
    }
    */
}