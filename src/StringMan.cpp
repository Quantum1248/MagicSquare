#include "../include/StringMan.h"

bool is_number(const std::string &s)
{
    return std::find_if(s.begin(),
                        s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
}

mpz_class NextNumber(std::string s, size_t &pos)
{
    while (!std::isdigit(s[pos]))
    {
        pos++;
    }
    size_t initPos = pos;
    while (pos < s.size() && std::isdigit(s[pos]))
    {
        pos++;
    }
    mpz_class res;
    if (initPos < pos)
    {
        std::cout << s.substr(initPos, pos-initPos) << std::endl;
        res = s.substr(initPos, pos - initPos);
        std::cout << res << std::endl;
    }
    return res;
}