#include "../include/EvenKey.h"

EvenKey::EvenKey() : evenKey(0), a(0), b(0)
{
}

EvenKey::EvenKey(const mpz_class &evenKey, const mpz_class &a, const mpz_class &b) : evenKey(evenKey), a(a), b(b)
{
}

EvenKey::~EvenKey()
{
}

std::string EvenKey::ToString() const
{
    return "{ " + evenKey.get_str() + ", " + a.get_str() + ", " + b.get_str() + " }";
}

void EvenKey::FromString(const std::string &s)
{
    size_t pos = 0;
    this->evenKey = NextNumber(s, pos);
    this->a = NextNumber(s, pos);
    this->b = NextNumber(s, pos);
}

EvenKey &EvenKey::operator=(const EvenKey &o)
{
    this->evenKey = o.evenKey;
    this->a = o.a;
    this->b = o.b;
    return *this;
}