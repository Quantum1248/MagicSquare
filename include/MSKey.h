#ifndef MSKEY_H
#define MSKEY_H

#include <string>
#include <gmp.h>
#include <gmpxx.h>

#include "EvenKey.h"

class MSKey
{
private:
public:
    MSKey();
    MSKey(const mpz_class &oddKey);
    MSKey(const mpz_class &oddKey, const mpz_class &evenKey, const mpz_class &a, const mpz_class &b);
    MSKey(const mpz_class &oddKey,const std::vector<EvenKey>& evenKeys);
    ~MSKey();

    std::string ToString() const;
    void FromString(const std::string &s);

    mpz_class oddKey;
    std::vector<EvenKey> evenKeys;
};

#endif