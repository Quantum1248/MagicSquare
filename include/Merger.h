#ifndef MERGER_H
#define MERGER_H

#include <vector>
#include <string>
#include <fstream>
#include <gmp.h>
#include <gmpxx.h>

#include "MSKey.h"
#include "EvenKey.h"
#include "StringMan.h"


void Merge(std::vector<std::string> paths, std::string resPath, bool clean);

std::string SubMerge(std::vector<std::string> paths, std::string resPath, bool clean);

#endif