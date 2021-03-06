#include "../include/SearchAlg.h"
#include "MSKContainer.h"

void MemoizedSearch(uint64_t min, uint64_t max, int thNumber)
{
    std::cout << "Thread " << thNumber << " has started with min="<<min<<" and max="<<max << std::endl;
    
    MSKContainer container(min,max);
    mpz_class a = 0, b = 0, apb = 0, amb = 0, n = 0, oddKey = 0, evenKey = 0;
    for (uint64_t i = min; i < max; i++)    //min parte da 0, non da 1
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
    }
    std::cout << "Thread " << thNumber << " is saving the file..." << std::endl;
    container.Save();

    std::cout<< "Thread " << thNumber << " has finished!" << std::endl;
}