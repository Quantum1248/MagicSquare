#include <iostream>
#include <map>
#include <gmpxx.h>

void CopPair(uint64_t i, bool even, mpz_t m, mpz_t n)
{
    if (even)
    {
        mpz_set_ui(m, 2);
        mpz_set_ui(n, 1);
    }
    else
    {
        mpz_set_ui(m, 3);
        mpz_set_ui(n, 1);
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

    mpz_t old_m, old_n;
    mpz_init(old_m);
    mpz_init(old_n);

    for (size_t j = type.size(); j > 0; j--)
    {
        //TODO:ottimizzare
        if (type[j - 1] == 0)
        {
            mpz_set(old_m, m);
            mpz_mul_ui(m, m, 2);
            mpz_sub(m, m, n);
            mpz_set(n, old_m);
        }
        else if (type[j - 1] == 1)
        {
            mpz_set(old_m, m);
            mpz_mul_ui(m, m, 2);
            mpz_add(m, m, n);
            mpz_set(n, old_m);
        }
        else
        {
            mpz_set(old_n, n);
            mpz_mul_ui(n, n, 2);
            mpz_add(m, n, m);
            mpz_set(n, old_n);
        }
    }
    mpz_clear(old_m);
    mpz_clear(old_n);
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

void FMK(mpz_t k, std::vector<mpz_t> &res, std::vector<uint64_t> &exp)
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
            res.push_back(i);
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

void FactorizeMemoized(mpz_t key, mpz_t n, std::vector<mpz_t> &res, std::vector<uint64_t>& exp)
{
    FMK(key, res, exp); //fattorizza la chiave
    size_t nIndex = res.size(); //indice al quale inizieranno i fattori di n
    FMN(n, res, exp); //fattorizza il coefficiente quadratico

    MergeFactor(res, exp, nIndex);
}

void memoizedSearch(uint64_t min = 0, uint64_t max = 1000)
{
    struct mpzCmp
    {
        bool operator()(const mpz_t val1, const mpz_t val2) const
        {
            int res = mpz_cmp(val1, val2);
            return res < 0;
        }
    };
    std::map<mpz_t, mpz_t, mpzCmp> m;

    mpz_t a, b, apb, amb, n, oddKey;
    mpz_inits(a, b, apb, amb, n, oddKey, nullptr);

    for (uint64_t i = min; i < max; i++)
    {
        CopPair(i / 2, i % 2 == 0, a, b);
        mpz_add(apb, a, b);
        mpz_sub(amb, a, b);
        OddFirmScm(a, b, apb, amb, oddKey);

        mpz_set_ui(n, 1);
        mpz_mul(n, n, a);
        mpz_mul(n, n, b);
        mpz_mul(n, n, apb);
        mpz_mul(n, n, amb);
        std::cout << "a: " << a << ";  b: " << b << std::endl;

        std::cout << "n: " << n << ";  oddKey: " << oddKey << std::endl;

        if (mpz_divisible_p(n, oddKey) != 0)
        {
            mpz_divexact(n, n, oddKey);
            if (mpz_perfect_square_p(n))
            {
                mpz_t oddKeyM, nM; //serve perchè oddKey è un puntatore, quindi cambiando il suo valore si cambierebbe anche il valore della chiave, stessa cosa per n
                mpz_init_set(oddKeyM, oddKey);
                mpz_init_set(nM, n);
                if (m.find(oddKeyM) == m.end())
                {
                    mpz_init_set(m[oddKeyM], nM);
                }
                else
                {
                    mpz_mul(m[oddKeyM], m[oddKeyM], nM);
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
    mpz_clears(a, b, apb, amb, n, oddKey, nullptr);
    //TODO:bisognerebbe pulire le entry e chiavi della mappa
}