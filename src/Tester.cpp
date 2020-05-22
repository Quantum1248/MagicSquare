#include "../include/Tester.h"

void test(std::string path)
{
    std::vector<std::vector<mpz_class>> quadDup;
    std::ifstream f;
    f.open(path, std::ios::in);
    if (!f.is_open())
        std::cout << "Failed to open res input file." << std::endl;

    std::vector<std::ofstream> resFile(9);
    for (size_t i = 0; i < resFile.size(); i++)
    {
        resFile[i].open(std::to_string(i + 1) + "-sol.txt", std::ios::trunc);
        if (!resFile[i].is_open())
            std::cout << "Failed to open res output file." << std::endl;
    }

    MSKey n;
    std::string line;
    int kl1 = 0;
    while (std::getline(f, line))
    {
        n.FromString(line);
        if (n.evenKeys.size() > 1)
        {
            //std::cout << n.oddKey << std::endl;
            n.Normalize();
            for (size_t i = 0; i < n.evenKeys.size(); i++)
            {
                for (size_t j = i + 1; j < n.evenKeys.size(); j++)
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
                    if (counter < 1)
                        std::cout << n.ToString() << std::endl;
                    else
                    {
                        mpz_class g = gcd(A, B);
                        g = gcd(g, C);
                        g = gcd(g, D);
                        g = gcd(g, E);
                        g = gcd(g, F);
                        g = gcd(g, G);
                        g = gcd(g, H);
                        g = gcd(g, I);
                        A /= g;
                        B /= g;
                        C /= g;
                        D /= g;
                        E /= g;
                        F /= g;
                        G /= g;
                        H /= g;
                        I /= g;

                        std::vector<mpz_class> &v = *(new std::vector<mpz_class>());
                        v.push_back(A);
                        v.push_back(B);
                        v.push_back(C);
                        if (std::find(quadDup.begin(), quadDup.end(), v) != quadDup.end())
                        {
                            std::cout << A << "," << B << "," << C << " dup" << std::endl;
                        }
                        quadDup.push_back(v);

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

void test2(std::string path)
{
    std::ifstream f;
    f.open(path, std::ios::in);
    if (!f.is_open())
        std::cout << "Failed to open res input file." << std::endl;

    std::vector<std::ofstream> resFile(9);
    for (size_t i = 0; i < resFile.size(); i++)
    {
        resFile[i].open(std::to_string(i + 1) + "-sol.txt", std::ios::app);
        if (!resFile[i].is_open())
            std::cout << "Failed to open res output file." << std::endl;
    }

    MSKey n;
    std::string line;
    int kl1 = 0;
    while (std::getline(f, line))
    {
        n.FromString(line);
        if (n.evenKeys.size() > 1)
        {
            //std::cout << n.oddKey << std::endl;
            n.Normalize();
            for (size_t i = 0; i < n.evenKeys.size(); i++)
            {
                for (size_t j = i + 1; j < n.evenKeys.size(); j++)
                {
                    mpz_class l1 = (n.oddKey / n.evenKeys[i].a) - n.evenKeys[i].a + 1;
                    mpz_class L1 = (n.oddKey / n.evenKeys[i].a) + n.evenKeys[i].a - 1;
                    mpz_class L2 = (n.oddKey / n.evenKeys[j].b) + n.evenKeys[j].b - 1;
                    mpz_class c = DispSum(l1, L1);
                    mpz_class e2 = DispSum(l1, L2);
                    mpz_class D = DispSum(1, l1 - 2);
                    mpz_class e = 1;
                    if (e2 % 2 != 0)
                    {
                        e2 = e2;
                    }
                    else
                    {
                        e = e2 / 2;
                    

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
                        if (counter < 1)
                            std::cout << n.ToString() << std::endl;
                        else
                        {
                            mpz_class g = gcd(A, B);
                            g = gcd(g, C);
                            g = gcd(g, D);
                            g = gcd(g, E);
                            g = gcd(g, F);
                            g = gcd(g, G);
                            g = gcd(g, H);
                            g = gcd(g, I);
                            A /= g;
                            B /= g;
                            C /= g;
                            D /= g;
                            E /= g;
                            F /= g;
                            G /= g;
                            H /= g;
                            I /= g;

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
    }

    f.close();
    for (size_t i = 0; i < resFile.size(); i++)
    {
        resFile[i].close();
    }
}

void test3(std::string path)
{
    std::vector<std::vector<mpz_class>> quadDup;
    std::ifstream f;
    f.open(path, std::ios::in);
    if (!f.is_open())
        std::cout << "Failed to open res input file." << std::endl;

    std::vector<std::ofstream> resFile(9);
    for (size_t i = 0; i < resFile.size(); i++)
    {
        resFile[i].open(std::to_string(i + 1) + "-sol.txt", std::ios::trunc);
        if (!resFile[i].is_open())
            std::cout << "Failed to open res output file." << std::endl;
    }

    MSKey n;
    std::string line;
    int kl1 = 0;
    while (std::getline(f, line))
    {
        std::cout << "Linea " << kl1 << std::endl;
        kl1++;
        n.FromString(line);
        n.Normalize();
        for (size_t i = 0; i < n.evenKeys.size(); i++)
        {
            mpz_class l1 = (n.oddKey / n.evenKeys[i].a) - n.evenKeys[i].a + 1;
            mpz_class L1 = (n.oddKey / n.evenKeys[i].a) + n.evenKeys[i].a - 1;
            mpz_class e = DispSum(l1, L1);

            mpz_class D = DispSum(1, l1 - 2);
            mpz_class E = e + D;
            mpz_class F = 2 * e + D;

            mpz_class mSum = 3 * E;
            mpz_class min = sqrt(D)+1;
            mpz_class max = sqrt(E);
            for (mpz_class c = min; c < max;++c)
            {
                mpz_class C = c * c;
                mpz_class G = mSum - (E + C);
                mpz_class A = mSum - (G + D);
                mpz_class I = mSum - (A + E);
                mpz_class H = mSum - (G + I);
                mpz_class B = mSum - (A + C);

                int counter = 4;
                if (mpz_perfect_square_p(A.get_mpz_t()))
                {
                    counter++;
                }
                if (mpz_perfect_square_p(B.get_mpz_t()))
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
                if(counter>5)
                {
                    resFile[counter - 1] << "A= " << A << "\n";
                    resFile[counter - 1] << "B= " << B << "\n";
                    resFile[counter - 1] << "C= " << C << "\n";
                    resFile[counter - 1] << "D= " << D << "\n";
                    resFile[counter - 1] << "E= " << E << "\n";
                    resFile[counter - 1] << "F= " << F << "\n";
                    resFile[counter - 1] << "G= " << G << "\n";
                    resFile[counter - 1] << "H= " << H << "\n";
                    resFile[counter - 1] << "I= " << I << "\n";
                    resFile[counter - 1] << "\n";
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