#include "../include/Merger.h"

void Merge(std::vector<std::string> paths, std::string resPath)
{
    size_t pos = 0;
    mpz_class minN = 0, maxN = 0;
    std::vector<std::ifstream*> files(paths.size());
    for (size_t i = 0; i < paths.size(); i++)
    {
        pos = 0;
        files[i] = new std::ifstream();
        (*files[i]).open(paths[i], std::ios::in);
        mpz_class tmpMin, tmpMax;
        tmpMin = NextNumber(paths[i], pos);
        tmpMax = NextNumber(paths[i], pos);
        if (tmpMin < minN)
            minN = tmpMin;
        if (tmpMax > maxN)
            maxN = tmpMax;
    }
    std::ofstream resFile;
    resFile.open(minN.get_str() + "-" + maxN.get_str() + ".txt");

    std::vector<std::string> lines(files.size());
    std::vector<MSKey> MSKeys(lines.size());
    for (size_t i = 0; i < files.size();)
    {
        getline(*files[i], lines[i]);
        if (lines[i] == "")
        {
            (*files[i]).close();
            delete files[i];
            files[i] = files[files.size() - 1];
            files.resize(files.size() - 1);

            MSKeys[i] = MSKeys[MSKeys.size() - 1];
            MSKeys.resize(MSKeys.size() - 1);
        }
        else
        {
            MSKeys[i].FromString(lines[i]);
            i++;
        }
    }

    mpz_class min;
    size_t minIndex = 0;
   
    while (MSKeys.size() > 0)
    {
        MSKey res;
        min = MSKeys[0].oddKey;
        minIndex = 0;
        for (size_t i = 0; i < MSKeys.size(); i++)
        {
            if (MSKeys[i].oddKey < min)
            {
                min = MSKeys[i].oddKey;
                minIndex = i;
            }
        }
        res.oddKey = min;
        for (size_t i = 0; i < MSKeys.size(); )
        {
            if (MSKeys[i].oddKey == min)
            {
                res.evenKeys.insert(res.evenKeys.end(), MSKeys[i].evenKeys.begin(), MSKeys[i].evenKeys.end());
                if (getline(*files[i], lines[i]))
                {
                    MSKeys[i].FromString(lines[i]);
                    i++;
                }
                else
                {
                    (*files[i]).close();
                    delete files[i];
                    files[i] = files[files.size() - 1];
                    files.resize(files.size() - 1);

                    MSKeys[i] = MSKeys[MSKeys.size() - 1];
                    MSKeys.resize(MSKeys.size() - 1);
                }
            }
            else
            {
                i++;
            }
        }
        resFile << res.ToString() << std::endl;
    }
    resFile.close();
}