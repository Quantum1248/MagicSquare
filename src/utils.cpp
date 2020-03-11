#include "../include/utils.h"

void CopPair(uint64_t i, bool even, mpz_class &m, mpz_class &n)
{
    if (even)
    {
        m= 2;
        n = 1;
    }
    else
    {
        m = 3;
        n = 1;
    }
    if (i == 0)
        return;
    i--;
    std::vector<ushort> type;
    while (i >= 3)
    {
        type.push_back(i % 3);
        i /= 3;
        i--;
    }
    type.push_back((ushort)i);

    mpz_class old_m=0;

    for (size_t j = type.size(); j > 0; j--)
    {
        //TODO:ottimizzare scambi
        //TODO:ottimizzare scambi
        if (type[j - 1] == 0)
        {
            old_m = m;
            m *= 2;
            m -= n;
            n = old_m;
        }
        else if (type[j - 1] == 1)
        {
            old_m = m;
            m *= 2;
            m += n;
            n = old_m;
        }
        else
        {
            m += 2 * n;
        }
    }
}

void OddFirm(mpz_class& res)
{
    mpz_class i=1,isq=1;

    while (res>isq)
    {
        //isq è la sequenza delle potenze(1,4,9,...)
        i += 2;
        isq += i;
        while (mpz_divisible_p(res.get_mpz_t(), isq.get_mpz_t()) != 0)
        {
            mpz_divexact(res.get_mpz_t(), res.get_mpz_t(), isq.get_mpz_t());
        }
    }
}

//n1,n2,n3 devono(e sono nel mio caso) essere coprimi
void OddFirmScm(mpz_class n1, mpz_class n2, mpz_class n3, mpz_class& res)
{
    OddFirm(n1);
    OddFirm(n2);
    OddFirm(n3);
    res = n1 * n2 * n3;
}

void FMK(mpz_class k, std::vector<mpz_class> &res, std::vector<uint64_t> &exp)
{
    mpz_class i=1;
    while (k >= i)
    {
        if (mpz_divisible_p(k.get_mpz_t(), i.get_mpz_t()) != 0)
        {
            mpz_divexact(k.get_mpz_t(), k.get_mpz_t(), i.get_mpz_t());
            res.push_back(i);
            exp.push_back(1);
        }
        i++;
    }
}

void FMN(mpz_class n, std::vector<mpz_class> &res, std::vector<uint64_t> &exp)
{
    mpz_class i=1;
    mpz_class isq=1;

    while (mpz_cmp(n.get_mpz_t(), isq.get_mpz_t()) >= 0)
    {
        //isq è la sequenza delle potenze(1,4,9,...)
        i += 2;
        isq += i;
        if (mpz_divisible_p(n.get_mpz_t(), isq.get_mpz_t()) != 0)
        {
            res.push_back((i+1)/2);
            exp.push_back(0);
            while (mpz_divisible_p(n.get_mpz_t(), isq.get_mpz_t()) != 0)
            {
                mpz_divexact(n.get_mpz_t(), n.get_mpz_t(), isq.get_mpz_t());
                exp.back()+=2;
            }
        }
    }
}

void NormalizePrimeFactorList(std::vector<mpz_class> &res, std::vector<uint64_t> &exp, size_t nIndex)
{
    int ik = nIndex-1;      // se si mette size_t poi fa in overflow negativo nel ciclo sotto
    size_t in = res.size()-1;

    while (ik>=0 && in >= nIndex)
    {
        if(res[ik]>res[in])
        {
            ik--;
        }
        else if(res[ik]<res[in])
        {
            in--;
        }
        else if (res[ik]==res[in])
        {
            exp[ik] += exp[in];
            res[in] = res[res.size() - 1];
            exp[in] = exp[exp.size() - 1];
            res.resize(res.size() - 1);
            exp.resize(exp.size() - 1);
        }
    }

    size_t i = 2;
    while (res[1] != 2)
    {
        if(res[i]==2)
        {
            mpz_class rtmp = res[1];
            uint64_t etmp = exp[1];
            res[1] = res[i];
            exp[1] = exp[i];
            res[i] = rtmp;
            exp[i] = etmp;
        }
        i++;
    }
}

void FindAllDivisor(std::vector<mpz_class> &factors, std::vector<uint64_t> &exp,mpz_class n, std::vector<mpz_class> &divisors)
{
    std::vector<uint64_t> weight;
    for (size_t i = 0; i < exp.size();i++)
    {
        weight.push_back(0);
    }
    mpz_class div = 1;
    divisors.push_back(div);
    size_t n_div = 1;
    for (size_t i = 0; i < exp.size(); i++)
    {
        n_div *= exp[i]+1;
    }
    n_div /= 2;

    divisors.reserve(n_div);
    weight[0] = 1;

    mpz_class test = n;
    for (size_t i = 0; i < factors.size(); i++)
    {
        mpz_class tmp;
        mpz_pow_ui(tmp.get_mpz_t(), factors[i].get_mpz_t(), exp[i]);
        test /= tmp;
    }
    if(test!=1)
        std::cout << "error" << std::endl;
    while (div < n)
    {
        int i = 0;
        weight[1]++;
        for (size_t i = 1; weight[i] > exp[i];)
        {
            weight[i] = 0;
            i++;
            weight[i]++;
        }
        div = 1;
        mpz_class tmp;
        for (size_t i = 0; i < weight.size(); i++)
        {
            mpz_pow_ui(tmp.get_mpz_t(), factors[i].get_mpz_t(), weight[i]);
            div *= tmp;
        }

        divisors.push_back(div);
    }
    std::sort(divisors.begin(), divisors.end());

    std::cout << divisors.size() << ";"<<n_div<< std::endl;

}

void FactorizeMemoized(mpz_class key_odd, mpz_class key_even, std::vector<mpz_class> &res, std::vector<uint64_t> &exp)
{
    mpz_class n = key_odd * key_even;
    FMK(key_odd, res, exp);     //fattorizza la chiave

    size_t nIndex = res.size(); //indice al quale inizieranno i fattori di n
    FMN(key_even, res, exp);    //fattorizza il coefficiente quadratico
    std::cout << std::endl;
    std::cout << key_odd << ";" << key_even << ";" << n << std::endl;

    NormalizePrimeFactorList(res, exp, nIndex);
    std::vector<mpz_class> divisors;
    FindAllDivisor(res, exp, n, divisors);
}

void memoizedCenter(mpz_class key, mpz_class sqrFactor)
{
    std::vector<mpz_class> factors;
    std::vector<uint64_t> exp;
    FactorizeMemoized(key, sqrFactor, factors, exp);
}

void memoizedSearch(uint64_t min, uint64_t max)
{
    std::map<mpz_class, mpz_class> m;

    mpz_class a=0, b=0, apb=0, amb=0, n=0, oddKey=0;

    for (uint64_t i = min; i < max; i++)
    {
        CopPair(i / 2, i % 2 == 0, a, b);
        apb = a + b;
        amb = a - b;
        n = a * b * apb * amb;
        
        if(n==100800)
            std::cout << "stop" << std::endl;
        OddFirmScm(a, b, apb * amb, oddKey);

        std::cout << "a: " << a << ";  b: " << b << std::endl;
        std::cout << "n: " << n << ";  oddKey: " << oddKey << std::endl;

        if (mpz_divisible_p(n.get_mpz_t(), oddKey.get_mpz_t()) != 0)
        {
            mpz_divexact(n.get_mpz_t(), n.get_mpz_t(), oddKey.get_mpz_t());
            if (mpz_perfect_square_p(n.get_mpz_t()))
            {
                if (m.find(oddKey) == m.end())
                {
                    m[oddKey]= n;
                }
                else
                {
                    m[oddKey]*= n;
                }
                std::cout << "Giusto " << i<<std::endl;
            }
            else
            {
                std::cout << "SBAGLIATO" << std::endl;
            }
        }
        else
        {
            std::cout << "SBAGLIATO" << std::endl;
        }
    }
    std::cout << "m.size() is " << m.size() << std::endl;
    //TODO:bisognerebbe pulire le entry e chiavi della mappa

    for (auto i = m.begin(); i != m.end(); ++i)
    {
        memoizedCenter(i->first, i->second);
    }
}