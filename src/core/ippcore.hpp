#ifndef INIPP_CORE_HPP
#define INIPP_CORE_HPP

#include "ipputility.hpp"
#include "src/file/file.h"
#include "src/types/ippcache.h"
#include "src/streams/streams.hpp"

namespace IPP
{
    enum class Mode
    {
        direct,
        cache
    };
} // namespace IPP

class IPPCore:
        private File
{
public:
    explicit IPPCore(const std::string& filename = "example.txt",
                     IPP::Mode mode = IPP::Mode::cache);
    virtual ~IPPCore();

protected:
    virtual std::string getSection() const;
    virtual void setSection(const std::string& name);

    bool setComment(IPP_Types::Parameter* parameter,
                    const std::string& comment);

    template<typename ...Ts>
    bool getParameter(IPP_Types::Parameter* parameter, Ts&... values) const;

    template<typename ...Ts>
    bool setParameter(IPP_Types::Parameter** parameter, const std::string& name,
                      const Ts... values);

    IPP_Types::Parameter* findParameter(const std::string& name,
                                        const std::string& section);

private:
    IPPCache* _cache;
    std::string _currentSection;
    IPP::Mode _ioMode;
    bool _isModified;
};

template<typename ...Ts>
bool
IPPCore::getParameter(IPP_Types::Parameter* parameter, Ts&... values) const
{
    if(parameter == nullptr) return false;

    // Try to guess type

    // Has parameter just came from file?
    {
        auto cast =
                dynamic_cast<IPP_Types::ParameterVector*>(parameter);

        if(cast != nullptr)
        {
            try
            {
                return IPP_Utility::vectorToArgs(0, cast->value, values...);
            }
            catch(...)
            {
            }

            return false;
        }
    }

    // Are types smilar?
    {
        auto cast =
                dynamic_cast<IPP_Types::ParameterTuple<Ts...>*>(parameter);

        if(cast != nullptr)
        {
            IPP_Utility::tupleToArgs(cast->value, values...);

            return true;
        }
    }

    // Types are definitely different
    return IPP_Utility::vectorToArgs(0, parameter->getValueAsVector(),
                                     values...);
}

template<typename ...Ts>
bool
IPPCore::setParameter(IPP_Types::Parameter** parameter, const std::string& name,
                      const Ts... values)
{
    IPP_Types::ParameterTuple<Ts...>* paramTuple = nullptr;

    _isModified = true;

    // Type of parameter and args are same
    paramTuple = dynamic_cast<IPP_Types::ParameterTuple<Ts...>*>(*parameter);

    if(paramTuple != nullptr)
    {
        paramTuple->value = std::tie(values...);

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

    // Create new parameter
    paramTuple = new IPP_Types::ParameterTuple<Ts...>;
    paramTuple->name = name;
    paramTuple->value = std::tie(values...);

    if(*parameter == nullptr)
    {
        // Parameter wasn`t found
        *parameter = paramTuple;
        _cache->addParameter(paramTuple, getSection());

        if(_ioMode == IPP::Mode::direct)
        {
            try
            {
                if(!appendFile(paramTuple, getSection()))
                {
                    return writeFile(_cache);
                }
            }
            catch(const std::runtime_error&)
            {
                std::cerr << "IPP unable to write file" << std::endl;

                return false;
            }
        }
    }
    else
    {
        // Parameter with different type was found
        paramTuple->comment = (*parameter)->comment;
        *parameter = paramTuple;
        _cache->replaceParameter(paramTuple, getSection());

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
    }

    return true;
}

#endif // INIPP_CORE_HPP
