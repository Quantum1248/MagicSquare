#ifndef EVENKEY_H
#define EVENKEY_H

#include <string>
#include <gmp.h>
#include <gmpxx.h>

#include "StringMan.h"

class EvenKey
{
private:
    /* data */
public:
    mpz_class evenKey, a, b;

    EvenKey();
    EvenKey(const mpz_class &evenKey, const mpz_class &a, const mpz_class &b);
    ~EvenKey();

    std::string ToString() const;
    void FromString(const std::string &s);

    EvenKey &operator=(const EvenKey &o);
};

#endif