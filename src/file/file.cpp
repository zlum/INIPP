#include "file.h"

#include "ippreader.h"
#include "src/streams/streams.hpp"
#include "src/core/ipputility.hpp"

#include <sys/stat.h>

using namespace std;

File::File(const string& fileName):
    fileName(fileName)
{
    // Создаёт файл, если его нет
    struct stat buffer{};

    if(stat(fileName.c_str(), &buffer) != 0)
    {
        ofstream outfile(fileName);
        if(!outfile.is_open())
        {
            throw runtime_error("IPP unable to create file");
        }
    }
}

IPPCache*
File::readFile()
{
    ChainLock sync(fileName, IPP_Types::LockType::parallel);
    ifstream textFile(fileName);

    if(!textFile.is_open())
    {
        throw runtime_error("IPP unable to open file");
    }

    string line;
    auto parameters = new IPPCache{};
    _currentSection.clear();

    while(getline(textFile, line))
    {
        if(line.empty())
        {
            continue;
        }

        if(line.at(0) == ';')
        {
            continue;
        }

        if(line.at(0) == '[')
        {
            // Lines with section names in a file begin with '['
            _currentSection = IPP_Reader::readSectionName(line);
        }
        else
        {
            auto parameter = IPP_Reader::createParameter(line);

            if(!parameters->addParameter(parameter, _currentSection))
            {
                delete parameter;
            }
        }
    }

    return parameters;
}

bool
File::appendFile(IPP_Types::Parameter* parameter, const string& section)
{
    ChainLock sync(fileName, IPP_Types::LockType::sequential);

    if(section != _currentSection)
    {
        return false;
    }

    ofstream textFileOut(fileName, ios::app);
    if(!textFileOut.is_open())
    {
        throw runtime_error("IPP unable to append file");
    }

    textFileOut << *parameter;

    return true;
}

bool
File::writeFile(IPPCache* parameters)
{
    if(parameters == nullptr) return true;

    ChainLock sync(fileName, IPP_Types::LockType::sequential);
    ofstream textFileOut(fileName);
    if(!textFileOut.is_open())
    {
        throw runtime_error("IPP unable to write file");
    }

    stringstream text;

    for(const auto& next : *parameters->getSequence())
    {
        _currentSection = next;

        auto section = parameters->getSection(_currentSection);

        if(!_currentSection.empty())
        {
            if(next != *parameters->getSequence()->begin())
            {
                text << endl;
            }

            text << '[' << _currentSection << ']' << endl;
        }

        auto parameter = section->getFirstParameter();
        while(parameter != nullptr)
        {
            text << *parameter;
            parameter = parameter->nextParameter; // ++ iterator
        }
    }

    textFileOut << text.str();

    return true;
}
