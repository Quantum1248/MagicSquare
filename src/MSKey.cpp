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

void MSKey::Normalize()
{
    //per il momento non riduco le potenze simili
    for (size_t i = 0; i < evenKeys.size(); i++)
    {
        oddKey *= evenKeys[i].evenKey;
        mpz_class root = sqrt(evenKeys[i].evenKey);
        evenKeys[i].evenKey = 1;

        for (size_t j = 0; j < evenKeys.size(); j++)
        {
            if(j!=i)
            {
                evenKeys[j].a *= root;
                evenKeys[j].b *= root;
            }
        }       
    }

    //faccio diventare tutti gli a e b pari
    for (size_t i = 0; i < evenKeys.size(); i++)
    {
        if(evenKeys[i].a%2==1 || evenKeys[i].b%2==1)
        {
            oddKey *= 4;
            for (size_t j = 0; j < evenKeys.size(); j++)
            {
                evenKeys[j].a *= 2;
                evenKeys[j].b *= 2;
            }
        }
    }
    /*
    std::cout << "Chiavi non ordinate:" << std::endl;
    for (size_t i = 0; i < evenKeys.size(); i++)
    {
        std::cout << evenKeys[i].ToString() << std::endl;
    }
    std::cout << std::endl;*/
    //le ordino dalla a più grande alla più piccola

    for (size_t i = 0; i < evenKeys.size(); i++)
    {
        EvenKey tmp = evenKeys[i];
        for (size_t j = i; j < evenKeys.size(); j++)
        {
            if (evenKeys[j].a > tmp.a)
            {
                evenKeys[i] = evenKeys[j];
                evenKeys[j] = tmp;
                tmp = evenKeys[i];
            }
        }
    }
        /*
    std::cout << "Chiavi ordinate:" << std::endl;
    for (size_t i = 0; i < evenKeys.size(); i++)
    {
        std::cout << evenKeys[i].ToString() << std::endl;
    }
    std::cout << "----------" << std::endl;*/
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