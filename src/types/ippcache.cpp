#include "ippcache.h"

#include "ipptypes.hpp"

#include <algorithm>

using namespace std;

IPPCache::~IPPCache()
{
    for(const auto& section : _sections)
    {
        auto next = section.second.getFirstParameter();
        while(next != nullptr)
        {
            auto toRemove = next;
            next = next->nextParameter; // ++ iterator
            delete toRemove;
        }
    }
}

bool
IPPCache::addParameter(IPP_Types::Parameter* parameter,
                       const string& section)
{
    auto ret = _sections.insert({section, IPPSection(parameter)});

    // If there was no such section add it to sequence
    if(ret.second)
    {
        if(section.empty())
        {
            _sequence.push_front(section);
        }
        else
        {
            _sequence.push_back(section);
        }

        return true;
    }

    return ret.first->second.addParameter(parameter);;
}

bool
IPPCache::replaceParameter(IPP_Types::Parameter* parameter,
                           const string& section)
{
    auto search = _sections.find(section);

    if(search != _sections.end())
    {
        search->second.replaceParameter(parameter);

        return true;
    }

    return false;
}

bool
IPPCache::rmParameter(IPP_Types::Parameter* parameter,
                      const string& section)
{
    auto search = _sections.find(section);

    if(search != _sections.end())
    {
        search->second.rmParameter(parameter->name);

        if(search->first.empty())
        {
            _sections.erase(search);
            _sequence.erase(remove(_sequence.begin(), _sequence.end(), section),
                           _sequence.end());
        }

        return true;
    }

    return false;
}

IPP_Types::Parameter*
IPPCache::getParameter(const string& parameter,
                       const string& section) const
{
    auto search = _sections.find(section);

    if(search != _sections.end())
    {
        return search->second.getParameter(parameter);
    }

    return nullptr;
}

IPPSection*
IPPCache::getSection(const string& section)
{
    auto search = _sections.find(section);

    if(search != _sections.end())
    {
        return &search->second;
    }

    return nullptr;
}

deque<string>*
IPPCache::getSequence()
{
    return &_sequence;
}
