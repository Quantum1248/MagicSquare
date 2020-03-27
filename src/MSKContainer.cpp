#include "../include/MSKContainer.h"

MSKContainer::MSKContainer()
{
}

MSKContainer::~MSKContainer()
{
    for (auto i = m.begin(); i !=m.end(); i++)
    {
        for (size_t j = 0; j < i->second.size(); j++)
        {
            delete i->second[j];
        }
    }
}

void MSKContainer::Insert(const mpz_class &oddKey, const mpz_class &evenKey, const mpz_class &a, const mpz_class &b)
{
    m[oddKey].push_back(new Info(evenKey, a, b));
}

void MSKContainer::Save(std::string path) const
{
    std::ofstream f;
    f.open(path, std::ios::trunc);
    for (auto i = m.begin(); i != m.end(); ++i)
    {
        f << i->first;
        for (size_t j = 0; j < i->second.size(); j++)
        {
            f << " { " << i->second[j]->evenKey << ", " << i->second[j]->a << ", " << i->second[j]->b << " }";
        }
        f << std::endl;
    }
    f.close();
}

MSKContainer::Info::Info(const mpz_class &evenKey, const mpz_class &a, const mpz_class &b)
{
    this->evenKey = evenKey;
    this->a=a;
    this->b=b;
}