#include <iostream>
#include <map>
#include <gmp.h>
#include <gmpxx.h>

void CopPair(uint64_t i, bool even, mpz_class &m, mpz_class &n);

void OddFirm(mpz_class &res);

void OddFirmScm(mpz_class n1, mpz_class n2, mpz_class n3, mpz_class n4, mpz_class &res);

void FMK(mpz_t k, std::vector<mpz_t *> &res, std::vector<uint64_t> &exp);

void FMN(mpz_t n, std::vector<mpz_t> &res, std::vector<uint64_t> &exp);

void MergeFactor(std::vector<mpz_t> &res, std::vector<uint64_t> &exp, size_t nIndex);

void FactorizeMemoized(mpz_t key, mpz_t n, std::vector<mpz_t *> &res, std::vector<uint64_t> &exp);

void memoizedSearch(uint64_t min = 0, uint64_t max = 1000);