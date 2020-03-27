#ifndef MSKCONTAINER_H
#define MSKCONTAINER_H

#include <iostream>
#include <fstream>
#include <map>
#include <gmp.h>
#include <gmpxx.h>

class MSKContainer
{
    public:
        MSKContainer(uint64_t min=0, uint64_t max=0);
        ~MSKContainer();

        void Insert(const mpz_class &oddKey, const mpz_class &evenKey, const mpz_class &a, const mpz_class &b);
        //void Insert(mpz_class oddKey, mpz_class evenKey, mpz_class a, mpz_class b);

        const mpz_class &Get(mpz_class oddKey) const;

        void Save() const;
        void Load(std::vector<std::string> path, bool clean = false);

    private:
        class Info
        {
        public:
            Info(const mpz_class &evenKey = 0, const mpz_class &a = 0, const mpz_class &b = 0);
            mpz_class evenKey;
            mpz_class a;
            mpz_class b;
        };
        std::map<mpz_class, std::vector<Info *>> m;
        uint64_t min, max;
};

#endif