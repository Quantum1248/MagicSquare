#include "../include/MSKContainer.h"

MSKContainer::MSKContainer(uint64_t min, uint64_t max):min(min),max(max)
{
}

MSKContainer::~MSKContainer()
{
    for (auto i = m.begin(); i != m.end(); i++)
    {
        for (size_t j = 0; j < i->second.size(); j++)
        {
            //delete i->second[j];
        }
    }
}

void MSKContainer::Insert(const mpz_class &oddKey, const mpz_class &evenKey, const mpz_class &a, const mpz_class &b)
{
    m[oddKey].push_back(EvenKey(evenKey, a, b));
}

void MSKContainer::Save(bool clean) const
{
    std::ofstream f;
    f.open("/run/media/manuel/29703f9d-5247-4b9b-8a98-bc007a395130/"+std::to_string(min) + "-" + std::to_string(max) + ".txt", std::ios::trunc);
    for (auto i = m.begin(); i != m.end(); ++i)
    {
        if (!clean || i->second.size() > 1)
        {
            MSKey msk(i->first,i->second);
            /*for (size_t j = 0; j < i->second.size(); j++)
            {
                msk.evenKeys.push_back(*(i->second[j]));
            }*/
            f << msk.ToString()<<std::endl;
        }
    }
    f.close();
}

void MSKContainer::Load(std::vector<std::string> paths, bool clean)
{
    size_t pos = 0;
    for (size_t i = 0; i < paths.size(); i++)
    {
        pos = 0;
        uint64_t tmpMin, tmpMax;
        tmpMin = NextNumber(paths[i], pos).get_ui();
        tmpMax = NextNumber(paths[i], pos).get_ui();
        if (tmpMin < min)
            min = tmpMin;
        if (tmpMax > max)
            max = tmpMax;
    }

    MSKey msk;
    std::string line="";
    for (size_t i = 0; i < paths.size(); i++)
    {
        std::ifstream file;
        file.open(paths[i], std::ios::in);
        while (getline(file,line))
        {
            msk.FromString(line);
            for (size_t j = 0; j < msk.evenKeys.size(); j++)
            {
                m[msk.oddKey].push_back(msk.evenKeys[j]);
            }
        }
        file.close();
        if (clean)
            std::remove(paths[i].c_str());
    }
}
/*
void MSKContainer::Merge(std::vector<std::string> paths, bool cleanOldFile, bool cleanResult) const
{

    size_t pos = 0;
    mpz_class min = this->min, max = this->max;
    std::vector<std::ifstream> files(paths.size());
    for (size_t i = 0; i < paths.size(); i++)
    {
        pos = 0;
        files[i].open(paths[i], std::ios::in);
        mpz_class tmpMin, tmpMax;
        tmpMin = ExtractNextNumber(paths[i], pos);
        tmpMax = ExtractNextNumber(paths[i], pos);
        if (tmpMin < min)
            min = tmpMin;
        if (tmpMax > max)
            max = tmpMax;
    }
    std::ofstream resFile;
    resFile.open(min.get_str() + "-" + max.get_str() + ".txt");

    std::vector<std::string> lines(files.size());
    std::vector<LineObj> currentLines(lines.size());
    for (size_t i = 0; i < files.size(); i++)
    {
        files[i]>>lines[i];
        currentLines[i] = LineObj::FromString(lines[i]);
    }

    do
    {
            
    } while (lines.size() > 0);
   
}
 */