#include "ippsection.h"

#include "ipptypes.hpp"

using namespace std;

IPPSection::IPPSection():
    _firstParameter(nullptr),
    _lastParameter(nullptr)
{
}

IPPSection::IPPSection(IPP_Types::Parameter* parameter):
    _parameters{make_pair(parameter->name, parameter)},
    _firstParameter(parameter),
    _lastParameter(parameter)
{
}

bool
IPPSection::addParameter(IPP_Types::Parameter* parameter)
{
    auto ret = _parameters.insert({parameter->name, parameter});

    // If there was no such section add it to sequence
    if(ret.second)
    {
        _lastParameter->nextParameter = parameter;
        _lastParameter = parameter;

        if(_parameters.size() == 1)
        {
            _firstParameter = parameter;
        }

        return true;
    }

    return false;
}

bool
IPPSection::replaceParameter(IPP_Types::Parameter* parameter)
{
    auto search = _parameters.find(parameter->name);

    if(search != _parameters.end())
    {
        if(_firstParameter == search->second)
        {
            parameter->nextParameter = _firstParameter->nextParameter;
            _firstParameter = parameter;
        }
        else
        {
            auto next = _firstParameter;
            while(next != nullptr)
            {
                if(next->nextParameter == search->second)
                {
                    if(_lastParameter == search->second)
                    {
                        _lastParameter = parameter;
                    }

                    parameter->nextParameter =
                            next->nextParameter->nextParameter;
                    next->nextParameter = parameter;

                    break;
                }

                next = next->nextParameter; // ++ iterator
            }
        }

        delete search->second;
        search->second = parameter;

        return true;
    }

    return false;
}

bool
IPPSection::rmParameter(const string& parameter)
{
    auto search = _parameters.find(parameter);

    if(search != _parameters.end())
    {
        // Удаление указателя на элемент из последовательности для записи
        auto next = _firstParameter;
        while(next != nullptr)
        {
            if(next == search->second)
            {
                if(next == _firstParameter)
                {
                    _firstParameter = next->nextParameter;
                }
                else
                if(next->nextParameter == _lastParameter)
                {
                    _lastParameter = next;
                }

                next->nextParameter = next->nextParameter->nextParameter;

                break;
            }

            next = next->nextParameter; // ++ iterator
        }

        _parameters.erase(search);
        delete search->second;

        return true;
    }

    return false;
}

IPP_Types::Parameter*
IPPSection::getParameter(const string& parameter) const
{
    auto search = _parameters.find(parameter);

    if(search != _parameters.end())
    {
        return search->second;
    }

    return nullptr;
}

IPP_Types::Parameter*
IPPSection::getFirstParameter() const
{
    return _firstParameter;
}

IPP_Types::Parameter*
IPPSection::getLastParameter() const
{
    return _lastParameter;
}
