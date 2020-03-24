#include <iostream>
#include <fstream>
#include <map>
#include <gmp.h>
#include <gmpxx.h>

void CopPair(uint64_t i, bool even, mpz_class &m, mpz_class &n);

void OddFirm(mpz_class &res);

void OddFirmScm(mpz_class n1, mpz_class n2, mpz_class n3, mpz_class &res);

void FMK(mpz_class k, std::vector<mpz_class> &res, std::vector<uint64_t> &exp);

void FMN(mpz_class n, std::vector<mpz_class> &res, std::vector<uint64_t> &exp);

void NormalizePrimeFactorList(std::vector<mpz_class> &res, std::vector<uint64_t> &exp, size_t nIndex);

void FindAllDivisors(std::vector<mpz_class> &factors, std::vector<uint64_t> &exp, mpz_class n, std::vector<mpz_class> &divisors);

void FactorizeMemoized(mpz_class key_odd, mpz_class key_even, std::vector<mpz_class> &res, std::vector<uint64_t> &exp);

void FilterDivisors(mpz_class &n, std::vector<mpz_class> &div);

void memoizedCenter(mpz_class key_odd, mpz_class key_even, std::vector<mpz_class> &centers);

void reverseCenter(std::vector<mpz_class> &c);

int check(mpz_class D, mpz_class e, mpz_class c);

void memoizedSearch(uint64_t min = 0, uint64_t max = 1000, int thNumber=0);