#include "../include/MagicMath.h"

void CoprimePair(uint64_t i, mpz_class &m, mpz_class &n)
{
    if (i % 2 == 0)
    {
        m= 2;
        n = 1;
    }
    else
    {
        m = 3;
        n = 1;
    }
    i /= 2;
    if (i == 0)
    {
        return;
    }
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

void FindOddFirm(mpz_class &res)
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

//n1,n2,n3,n4 devono(e sono nel mio caso) essere coprimi, a meno di n3 e n4 che possono avere solo 2 in comune
void FindOddFirmTris(mpz_class n1, mpz_class n2, mpz_class n3, mpz_class n4, mpz_class &res)
{
    FindOddFirm(n1);
    FindOddFirm(n2);
    if(n3%2==0 && n4%2==0 )
    {
        n3 /= 2;
        n4 /= 2;
    }
    mpz_class g = gcd(n3, n4);
    if(g!=1)
        g = g;
    FindOddFirm(n3);
    FindOddFirm(n4);
    res = n1 * n2 * n3 * n4;
}

void FMK(mpz_class k, std::vector<mpz_class> &res, std::vector<uint64_t> &exp)
{
    mpz_class i=2;
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
    int ik = nIndex - 1; // se si mette size_t poi fa in overflow negativo nel ciclo sotto
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
            in--;
        }
    }

    size_t i = 1;
    while (res[0] != 2)
    {
        if(res[i]==2)
        {
            mpz_class rtmp = res[0];
            uint64_t etmp = exp[0];
            res[0] = res[i];
            exp[0] = exp[i];
            res[i] = rtmp;
            exp[i] = etmp;
        }
        i++;
    }
}

void FindAllDivisors(std::vector<mpz_class> &factors, std::vector<uint64_t> &exp, mpz_class n, std::vector<mpz_class> &divisors)
{
    std::vector<uint64_t> weight;
    size_t n_div = 1;
    for (size_t i = 0; i < exp.size();i++)
    {
        weight.push_back(0);
        n_div *= exp[i] + 1;
    }
    divisors.reserve(n_div);

    mpz_class div = 1;
    divisors.push_back(div);

    while (div < n)
    {
        weight[0]++;
        for (size_t i = 0; weight[i] > exp[i];)
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
}

void FactorizeMemoized(mpz_class key_odd, mpz_class key_even, std::vector<mpz_class> &res, std::vector<uint64_t> &exp)
{
    FMK(key_odd, res, exp);     //fattorizza la chiave
    size_t nIndex = res.size(); //indice al quale inizieranno i fattori di n
    FMN(key_even, res, exp);    //fattorizza il coefficiente quadratico

    NormalizePrimeFactorList(res, exp, nIndex);
}

void FilterDivisors(mpz_class& n, std::vector<mpz_class>& div)
{
    mpz_class root=sqrt(n);
    for (size_t i = 0; i < div.size(); i++)
    {
        if(div[i]>root)
            div.resize(i);
    }
}

void memoizedCenter(mpz_class key_odd, mpz_class key_even, std::vector<mpz_class> &centers)
{
    mpz_class n = key_odd * key_even;
    std::vector<mpz_class> factors;
    std::vector<uint64_t> exp;

    FactorizeMemoized(key_odd, key_even, factors, exp);


    std::vector<mpz_class> divisors;

    FindAllDivisors(factors, exp, n, divisors);
    //std::vector<mpz_class> v = {1, 2, 3, 4, 6, 7, 8, 12, 14, 16, 21, 24, 28, 32, 42, 48, 56, 64, 67, 84, 96, 112, 128, 134, 168, 192, 201, 224, 268, 336, 384, 402, 448, 469, 536, 672, 804, 896, 938, 1072, 1344, 1407, 1608, 1876, 2144, 2688, 2814, 3216, 3752, 4288, 5628, 6432, 7504, 8576, 11256, 12864, 15008, 22512, 25728, 30016, 45024, 60032, 90048, 180096};
    //for (size_t i = 0; i < v.size(); i++)
    //{
//  if(v[i]!=divisors[i] || v.size()!=divisors.size())
    //        std::cout << "stop" << std::endl;
    //}

    //PrintArr(divisors);
    /*n *= 4;
    for (size_t i = 0; i < divisors.size(); i++)
    {
        divisors[i] *= 2;
    }*/

    FilterDivisors(n, divisors);
    int c = 0;
    for (auto a = divisors.begin(); a != divisors.end(); ++a)
    {
        mpz_class det = n * n + 6 * n * (*a) * (*a) + (*a) * (*a) * (*a) * (*a); //n^2+6na^2+a^4
        if (mpz_perfect_square_p(det.get_mpz_t()))
        {
            det = sqrt(det);
            det = n + (*a) * (*a) - det;
            if (det % (2 * (*a)) == 0)
            {
                det /= -2 * (*a);
                for (auto b = divisors.begin(); b != a && (*b) <= det; ++b)
                {
                    if ((*b) == det)
                    {
                        centers.push_back(*a);
                        centers.push_back(*b);
                        c++;
                    }
                }
            }
        }
    }
}

void reverseCenter(std::vector<mpz_class>& c)
{
    mpz_class tmp;
    for (size_t i = 0; i < c.size()/2; i++)
    {
        tmp = c[i];
        c[i] = c[c.size() - 1 - i];
        c[c.size() - 1 - i] = tmp;
    }
    for (size_t i = 0; i < c.size(); i+=2)
    {
        tmp = c[i];
        c[i] = c[i + 1];
        c[i + 1] = tmp;
    }
}

int cmax = 0;
int check(mpz_class D, mpz_class e, mpz_class c)
{
    mpz_class A = e + c + D;
    mpz_class B = 2 * e - 2 * c + D;
    mpz_class C = c + D;
    mpz_class E = e + D;
    mpz_class F = 2 * e + D;
    mpz_class G = 2 * e - c + D;
    mpz_class H = 2 * c + D;
    mpz_class I = e - c + D;

    int counter = 0;
    if (mpz_perfect_square_p(A.get_mpz_t()))
    {
        counter++;
    }
    if (mpz_perfect_square_p(B.get_mpz_t()))
    {
        counter++;
    }
    if (mpz_perfect_square_p(C.get_mpz_t()))
    {
        counter++;
    }
    if (mpz_perfect_square_p(D.get_mpz_t()))
    {
        counter++;
    }
    if (mpz_perfect_square_p(E.get_mpz_t()))
    {
        counter++;
    }
    if (mpz_perfect_square_p(F.get_mpz_t()))
    {
        counter++;
    }
    if (mpz_perfect_square_p(G.get_mpz_t()))
    {
        counter++;
    }
    if (mpz_perfect_square_p(H.get_mpz_t()))
    {
        counter++;
    }
    if (mpz_perfect_square_p(I.get_mpz_t()))
    {
        counter++;
    }

    if(counter>cmax)
        cmax = counter;
    if (counter > 6)
    {
        std::cout << counter << std::endl;
        std::cout << "A= " << A << std::endl;
        std::cout << "B= " << B << std::endl;
        std::cout << "C= " << C << std::endl;
        std::cout << "D= " << D << std::endl;
        std::cout << "E= " << E << std::endl;
        std::cout << "F= " << F << std::endl;
        std::cout << "G= " << G << std::endl;
        std::cout << "H= " << H << std::endl;
        std::cout << "I= " << I << std::endl;
        mpz_class key = 3 * E;
        if(A+B+C!=key)
            std::cout << "error" << std::endl;
    }

    return counter;
}
