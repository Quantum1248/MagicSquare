#include <iostream>
#include <map>
#include <gmp.h>
#include <gmpxx.h>

void CopPair(uint64_t i, bool even, mpz_class m, mpz_class n)
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
            n = m;
        }
        else if (type[j - 1] == 1)
        {
            old_m = m;
            m *= 2;
            m += n;
            n = m;
        }
        else
        {
            m += 2 * n;
        }
    }
}

void OddFirm(mpz_t n, mpz_t res)
{
    mpz_set(res, n);

    mpz_t i;
    mpz_t isq;
    mpz_init_set_ui(i, 1);
    mpz_init_set_ui(isq, 1);

    while (mpz_cmp(res, isq) >= 0)
    {
        //isq è la sequenza delle potenze(1,4,9,...)
        mpz_add_ui(i, i, 2);
        mpz_add(isq, isq, i);
        while (mpz_divisible_p(res, isq) != 0)
        {
            mpz_divexact(res, res, isq);
        }
    }
    mpz_clears(i, isq, nullptr);
}

//n1,n2,n3 ed n4 devono(e sono nel mio caso) essere coprimi
void OddFirmScm(mpz_t n1, mpz_t n2, mpz_t n3, mpz_t n4, mpz_t res)
{
    mpz_t kn1, kn2, kn3, kn4;
    mpz_inits(kn1, kn2, kn3, kn4, nullptr);
    mpz_set_ui(res, 1);

    OddFirm(n1, kn1);
    OddFirm(n2, kn2);
    OddFirm(n3, kn3);
    OddFirm(n4, kn4);

    mpz_mul(res, res, kn1);
    mpz_mul(res, res, kn2);
    mpz_mul(res, res, kn3);
    mpz_mul(res, res, kn4);

    mpz_clears(kn1, kn2, kn3, kn4, nullptr);
}

void FMK(mpz_t k, std::vector<mpz_t*> &res, std::vector<uint64_t> &exp)
{
    mpz_t tmp_k;
    mpz_init_set(tmp_k, k);
    mpz_t i;
    mpz_init_set_ui(i, 1);
    while (mpz_cmp(tmp_k, i) >= 0)
    {
        mpz_add_ui(i, i, 2);
        if (mpz_divisible_p(tmp_k, i) != 0)
        {
            mpz_divexact(tmp_k, tmp_k, i);
            mpz_t f;
            mpz_init_set(f, i);
            res.push_back(&f);
            exp.push_back(1);
        }
    }
    mpz_clears(tmp_k, i, nullptr);
}

void FMN(mpz_t n, std::vector<mpz_t> &res, std::vector<uint64_t>& exp)
{
    mpz_t tmp_n;
    mpz_init_set(tmp_n, n);
    mpz_t i;
    mpz_t isq;
    mpz_init_set_ui(i, 1);
    mpz_init_set_ui(isq, 1);

    while (mpz_cmp(tmp_n, isq) >= 0)
    {
        //isq è la sequenza delle potenze(1,4,9,...)
        mpz_add_ui(i, i, 2);
        mpz_add(isq, isq, i);
        if (mpz_divisible_p(tmp_n, isq) != 0)
        {
            res.push_back(isq);
            exp.push_back(0);
            while (mpz_divisible_p(tmp_n, isq) != 0)
            {
                mpz_divexact(tmp_n, tmp_n, isq);
                exp.back()++;
            }
        }
    }
    mpz_clears(tmp_n, i, isq, nullptr);
}

void MergeFactor(std::vector<mpz_t> &res, std::vector<uint64_t> &exp, size_t nIndex)
{
    size_t kf = nIndex; //numero di fattori di key;
    size_t nf = res.size() - nIndex; //numero di fattori di n
    size_t min;
    kf < nf ? min=kf : min=nf;
    size_t len = res.size();
    size_t i = 0;
    while (i<min)
    {
        if(mpz_cmp(res[i],res[i+nIndex])==0)
        {
            exp[i] += exp[i + nIndex];
        }
        else if (mpz_cmp(res[i], res[i + nIndex]) < 0)
        {
            //vettore di mpz_t?
        }
            i++;
    }
}

void FactorizeMemoized(mpz_t key, mpz_t n, std::vector<mpz_t*> &res, std::vector<uint64_t>& exp)
{
    FMK(key, res, exp); //fattorizza la chiave
    size_t nIndex = res.size(); //indice al quale inizieranno i fattori di n
    //FMN(n, res, exp); //fattorizza il coefficiente quadratico

    //MergeFactor(res, exp, nIndex);
}

void memoizedSearch(uint64_t min = 0, uint64_t max = 1000)
{
    std::map<mpz_class, mpz_class> m;

    mpz_class a=0, b=0, apb=0, amb=0, n=0, oddKey=0;

    for (uint64_t i = min; i < max; i++)
    {
        CopPair(i / 2, i % 2 == 0, a, b);
        apb = a + b;
        amb = a - b;
        //OddFirmScm(a, b, apb, amb, oddKey);

        n = a * b * apb * amb;
        
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
                std::cout << "Giusto" << std::endl;
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
}