#include "../include/Merger.h"

void Merge(std::vector<std::string> paths, std::string resPath, bool clean)
{
    int buffSize = 67108864; //64MB
    std::vector<std::ifstream *> files(paths.size());
    for (size_t i = 0; i < paths.size(); i++)
    {
        files[i] = new std::ifstream();
        files[i]->rdbuf()->pubsetbuf(new char[buffSize], buffSize);
        (*files[i]).open(paths[i], std::ios::in);
        if(!(*files[i]).is_open())
            std::cout << "Failed to open input file." << std::endl;
    }

    std::ofstream resFile;
    resFile.rdbuf()->pubsetbuf(new char[buffSize], buffSize);
    resFile.open(resPath, std::ios::trunc);
    if(!resFile.is_open())
        std::cout << "Failed to open output file." << std::endl;

    std::vector<std::string> lines(files.size());
    std::vector<MSKey> MSKeys(lines.size());
    for (size_t i = 0; i < files.size();)
    {
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
            if (clean)
                std::remove(paths[i].c_str());
            paths[i] = paths[paths.size() - 1];
            paths.resize(paths.size() - 1);

            MSKeys[i] = MSKeys[MSKeys.size() - 1];
            MSKeys.resize(MSKeys.size() - 1);
        }
    }

    mpz_class min;

    while (MSKeys.size() > 0)
    {
        MSKey res;
        min = MSKeys[0].oddKey;
        for (size_t i = 0; i < MSKeys.size(); i++)
        {
            if (MSKeys[i].oddKey < min)
            {
                min = MSKeys[i].oddKey;
            }
        }
        res.oddKey = min;
        int c = 0;
        for (size_t i = 0; i < MSKeys.size();)
        {
            if (MSKeys[i].oddKey == min)
            {
                c++;
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

                    if (clean)
                        std::remove(paths[i].c_str());
                    paths[i] = paths[paths.size() - 1];
                    paths.resize(paths.size() - 1);

                    MSKeys[i] = MSKeys[MSKeys.size() - 1];
                    MSKeys.resize(MSKeys.size() - 1);
                }
            }
            else
            {
                i++;
            }
        }
        if(c>1)
        {
            std::cout << "Trovato nuovo numero: " << res.ToString() << std::endl;
        }
        resFile << res.ToString() << '\n';
    }
    resFile.close();
}
