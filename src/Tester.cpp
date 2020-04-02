#include "../include/Tester.h"

void test(std::string path)
{
    std::ifstream f;
    f.open(path, std::ios::in);
    if (!f.is_open())
        std::cout << "Failed to open res input file." << std::endl;

    std::vector<std::ofstream> resFile(9);
    for (size_t i = 0; i < resFile.size(); i++)
    {
        resFile[i].open(std::to_string(i+1) + "-sol.txt",std::ios::trunc);
        if (!resFile[i].is_open())
            std::cout << "Failed to open res output file." << std::endl;
    }

    MSKey n;
    std::string line;
    int kl1= 0;
    while (std::getline(f, line))
    {
        n.FromString(line);
        if (n.evenKeys.size() > 1)
        {
            //std::cout << n.oddKey << std::endl;
            n.Normalize();
            for (size_t i = 0; i < n.evenKeys.size(); i++)
            {
                for (size_t j = i+1; j < n.evenKeys.size(); j++)
                {
                    mpz_class l1 = (n.oddKey / n.evenKeys[i].a) - n.evenKeys[i].a + 1;
                    mpz_class L1 = (n.oddKey / n.evenKeys[i].a) + n.evenKeys[i].a - 1;
                    mpz_class l2 = (n.oddKey / n.evenKeys[j].a) + n.evenKeys[j].a - 1;
                    mpz_class c = DispSum(l1, L1);
                    mpz_class e = DispSum(l1, l2);
                    mpz_class D = DispSum(1, l1 - 2);
                    
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
                        if (A + B + C != key)
                            std::cout << "error" << std::endl;
                    }
                    if(counter<1)
                        std::cout << n.ToString() << std::endl;
                    else
                    {
                        resFile[counter - 1] << "A= " << A << std::endl;
                        resFile[counter - 1] << "B= " << B << std::endl;
                        resFile[counter - 1] << "C= " << C << std::endl;
                        resFile[counter - 1] << "D= " << D << std::endl;
                        resFile[counter - 1] << "E= " << E << std::endl;
                        resFile[counter - 1] << "F= " << F << std::endl;
                        resFile[counter - 1] << "G= " << G << std::endl;
                        resFile[counter - 1] << "H= " << H << std::endl;
                        resFile[counter - 1] << "I= " << I << std::endl;
                        resFile[counter - 1] << std::endl;
                        mpz_class key = 3 * E;
                    }
                    
                   
                }
            }
        }
    }


    f.close();
    for (size_t i = 0; i < resFile.size(); i++)
    {
        resFile[i].close();
    }
}

mpz_class DispSum(mpz_class l, mpz_class L)
{
    return ((L + l) * (L - l + 2)) / 4;
}