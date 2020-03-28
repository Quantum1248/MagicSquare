#ifndef STRINGMAN_H
#define STRINGMAN_H

#include <iostream>
#include <string>
#include <gmp.h>
#include <gmpxx.h>

bool is_number(const std::string &s);
mpz_class NextNumber(std::string s, size_t &pos);

#endif