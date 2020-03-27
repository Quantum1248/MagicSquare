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

void MSKContainer::Load(std::vector<std::string> paths)
{
    std::string line, oddK, evenK, a,b;
    for (size_t p = 0; p < paths.size(); p++)
    {
        std::ifstream file;
        file.open(paths[p], std::ios::in);

        while (getline(file, line))
        {
            size_t f = 0;
            while (line[f] != ' ')
            {
                f++;
            }
            oddK = line.substr(0, f);
            for (size_t i = f; i < line.size(); i++)
            {
                f = i;
                if(line[i]=='{')
                {
                    i+=2;
                    f = i;
                    while (line[f] != ',')
                        f++;
                    evenK = line.substr(i, f-i);
                    f += 2;
                    i = f;
                    while (line[f] != ',')
                        f++;
                    a = line.substr(i, f-i);
                    f += 2;
                    i = f;
                    while (line[f] != ' ')
                        f++;
                    b = line.substr(i, f-i);
                    std::cout << oddK << " " << evenK << " " << a << " " << b << std::endl;
                }
            }
        }
        file.close();
    }
    
    
}

MSKContainer::Info::Info(const mpz_class &evenKey, const mpz_class &a, const mpz_class &b)
{
    this->evenKey = evenKey;
    this->a=a;
    this->b=b;
}