#include "ippcore.hpp"

using namespace std;

IPPCore::IPPCore(const string& filename, IPP::Mode mode):
    File(filename),
    _cache(nullptr),
    _ioMode(mode),
    _isModified(false)
{
    if(mode == IPP::Mode::cache)
    {
        _cache = readFile();
    }
}

IPPCore::~IPPCore()
{
    if(_ioMode == IPP::Mode::cache && _isModified)
    {
        try
        {
            writeFile(_cache);
        }
        catch(const runtime_error&)
        {
            cerr << "IPP unable to write file" << endl;
        }
    }

    delete _cache;
}

IPPCache*
IPPCore::getCache() const
{
    return _cache;
}

string
IPPCore::getSection() const
{
    return _currentSection;
}

void
IPPCore::setSection(const string& name)
{
    _currentSection = name;
}

bool IPPCore::setComment(IPP_Types::Parameter* parameter,
                         const string& comment)
{
    if(parameter == nullptr) return false;

    parameter->comment = comment;

    if(_ioMode == IPP::Mode::direct)
    {
        try
        {
            return writeFile(_cache);
        }
        catch(const std::runtime_error&)
        {
            std::cerr << "IPP unable to write file" << std::endl;

            return false;
        }
    }

    return true;
}

IPP_Types::Parameter*
IPPCore::findParameter(const string& name,
                       const string& section)
{
    if((_cache == nullptr) || (_ioMode != IPP::Mode::cache))
    {
        delete _cache;

        // В режиме НЕ cache переменная _cache
        // будет использована для записи файла
        _cache = readFile();
    }

    if(_cache == nullptr)
    {
        return nullptr;
    }

    return _cache->getParameter(name, section);
}

