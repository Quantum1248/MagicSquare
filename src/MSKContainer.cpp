#include "../include/MSKContainer.h"

MSKContainer::MSKContainer(uint64_t min, uint64_t max):min(min),max(max)
{
}

MSKContainer::~MSKContainer()
{
    for (auto i = m.begin(); i != m.end(); i++)
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

void MSKContainer::Save() const
{
    std::ofstream f;
    f.open(std::to_string(min) + "-" + std::to_string(max) + ".txt", std::ios::trunc);
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

void MSKContainer::Load(std::vector<std::string> paths, bool clean)
{
    std::string line,smin,smax, soddK, sevenK, sa, sb;
    mpz_class oddKey, evenKey, a, b;
    for (size_t p = 0; p < paths.size(); p++)
    {
        std::ifstream file;
        file.open(paths[p], std::ios::in);

        size_t f = 0;
        size_t ini = 0;

        while (paths[p][f] != '-')
        {
            f++;
        }
        smin = paths[p].substr(ini, f-ini);
        f++;
        ini = f;
        while (paths[p][f] != '.')
        {
            f++;
        }
        smax = paths[p].substr(ini, f-ini);
        if (std::stoul(smin) < min)
            min = std::stol(smin);
        if (std::stoul(smax) > max)
            max = std::stol(smax);
        while (getline(file, line))
        {
            f = 0;
            while (line[f] != ' ')
            {
                f++;
            }
            soddK = line.substr(0, f);
            oddKey = soddK;
            for (size_t i = f; i < line.size(); i++)
            {
                f = i;
                if(line[i]=='{')
                {
                    i+=2;
                    f = i;
                    while (line[f] != ',')
                        f++;
                    sevenK = line.substr(i, f-i);
                    evenKey = sevenK;
                    f += 2;
                    i = f;
                    while (line[f] != ',')
                        f++;
                    sa = line.substr(i, f-i);
                    a = sa;
                    f += 2;
                    i = f;
                    while (line[f] != ' ')
                        f++;
                    sb = line.substr(i, f-i);
                    b = sb;
                    //std::cout << soddK << " " << sevenK << " " << sa << " " << sb << std::endl;
                    m[oddKey].push_back(new Info(evenKey, a, b));
                }
            }
        }
        file.close();
        if(clean)
            std::remove(paths[p].c_str);
    }
    
    
}

MSKContainer::Info::Info(const mpz_class &evenKey, const mpz_class &a, const mpz_class &b)
{
    this->evenKey = evenKey;
    this->a=a;
    this->b=b;
}