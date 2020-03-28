#include "../include/MSKey.h"

MSKey::MSKey()
{
}

MSKey::MSKey(const mpz_class &oddKey) : oddKey(oddKey)
{
}

MSKey::MSKey(const mpz_class &oddKey, const mpz_class &evenKey, const mpz_class &a, const mpz_class &b) : oddKey(oddKey)
{
    this->evenKeys.push_back(EvenKey(evenKey, a, b));
}

MSKey::MSKey(const mpz_class &oddKey, const std::vector<EvenKey> &evenKeys) : oddKey(oddKey)
{
    for (size_t i = 0; i < evenKeys.size(); i++)
    {
        this->evenKeys.push_back(evenKeys[i]);
    }
}

MSKey::~MSKey()
{
}

std::string MSKey::ToString() const
{
    std::string str = oddKey.get_str();
    for (size_t i = 0; i < evenKeys.size(); i++)
    {
        str += " " + evenKeys[i].ToString();
    }
    return str;
}

void MSKey::FromString(const std::string& s)
{
    size_t firstPos = 0, endPos = 0;
    this->oddKey = NextNumber(s, firstPos);

    evenKeys.resize(0);
    endPos = firstPos;
    while(endPos<s.size())
    {
        while (s[firstPos] != '{')
        {
            firstPos++;
        }
        endPos = firstPos;
        while(s[endPos]!='}')
        {
            endPos++;
        }
        this->evenKeys.push_back(EvenKey());
        evenKeys.back().FromString(s.substr(firstPos, endPos - firstPos));
        endPos++;
        firstPos = endPos;
    }
}