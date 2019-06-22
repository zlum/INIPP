#ifndef INIPP_HPP
#define INIPP_HPP

#include "src/core/ippcore.hpp"

class IniPP:
        private IPPCore
{
    /*
     * On constructor call (ex: IniPP configFile("test.txt");)
     * the existence of a file with the specified name will be checked.
     * If there is no such file, it will be created.
     * The default file name is "example.txt".
     * File operating mode can be provided as a second argument
     * (ex: IniPP configFile("test.txt", IPP::Mode::cache);).
     * File operating mode are available in the IPP :: Mode enumeration.
     * The default mode is direct.
     * In direct mode file be read or written on each call of
     * corresponding function.
     * In cache mode file will be read once the constructor is called
     * and recorded once the destructor called and data was modified.
     *
     * bool setParameter(name, value...)
     * Writes parameter "name" with value of "value".
     * If there was such parameter, then its value will be changed.
     *
     * bool setDefaultParameter(name, value...)
     * If there was no parameter "name" set, then
     * writes parameter "name" with value of "value".
     *
     * bool getParameter(name, value...)
     * Reads value of parameter "name" to variable "value".
     *
     * bool getParameterForced(name, value...)
     * Reads value of parameter "name" to variable "value".
     * If there was no such parameter set,
     * then it will be added with value of "value".
     *
     * There are methods to work with comments
     * by the same scheme as parameters.
     *
     * Every methods above returns value of boolean type,
     * where true is successful execution.
     *
     * string getSection()
     * Returns name of current section.
     * By default is empty.
     *
     * void setSection(name)
     * Set name of current section.
     * Parameters from other sections will not be read or written.
    */

public:
    inline explicit IniPP(const std::string& fileName = "example.txt",
                          IPP::Mode mode = IPP::Mode::cache);
    inline virtual ~IniPP() = default;

    inline virtual std::string getSection() const override;
    inline virtual void setSection(const std::string& name) override;

    inline bool getComment(const std::string& name,
                           std::string& comment);
    inline bool setComment(const std::string& name,
                           const std::string& comment);
    inline bool getCommentForced(const std::string& name,
                                 std::string& comment);
    inline bool setDefaultComment(const std::string& name,
                                  const std::string& comment);

    template<typename ...Ts>
    bool getParameter(const std::string& name, Ts&... values);

    template<typename ...Ts>
    bool setParameter(const std::string& name, const Ts... values);

    template<typename ...Ts>
    bool getParameterForced(const std::string& name, Ts&... value);

    template<typename ...Ts>
    bool setDefaultParameter(const std::string& name, const Ts... value);
};

inline IniPP::IniPP(const std::string& fileName, IPP::Mode mode):
    IPPCore(fileName, mode)
{
}

inline std::string
IniPP::getSection() const
{
    return IPPCore::getSection();
}

inline void
IniPP::setSection(const std::string& name)
{
    IPPCore::setSection(name);
}

inline bool
IniPP::getComment(const std::string& name, std::string& comment)
{
    IPP_Types::Parameter* parameter = findParameter(name, getSection());

    if(parameter == nullptr) return false;

    comment = parameter->comment;

    return true;
}

inline bool
IniPP::setComment(const std::string& name, const std::string& comment)
{
    IPP_Types::Parameter* parameter = findParameter(name, getSection());

    return IPPCore::setComment(parameter, comment);
}

inline bool
IniPP::getCommentForced(const std::string& name, std::string& comment)
{
    IPP_Types::Parameter* parameter = findParameter(name, getSection());

    if(parameter == nullptr) return false;

    if(parameter->comment.empty())
    {
        if(!IPPCore::setComment(parameter, comment)) return false;
    }
    else
    {
        comment = parameter->comment;
    }

    return true;
}

inline bool
IniPP::setDefaultComment(const std::string& name, const std::string& comment)
{
    IPP_Types::Parameter* parameter = findParameter(name, getSection());

    if(parameter == nullptr) return false;

    if(parameter->comment.empty())
    {
        return IPPCore::setComment(parameter, comment);
    }

    return true;
}

template<typename ...Ts>
bool
IniPP::getParameter(const std::string& name, Ts&... values)
{
    IPP_Types::Parameter* parameter = findParameter(name, getSection());

    return IPPCore::getParameter(parameter, values...);
}

template<typename ...Ts>
bool
IniPP::setParameter(const std::string& name, const Ts... values)
{
    IPP_Types::Parameter* parameter = findParameter(name, getSection());

    return IPPCore::setParameter(&parameter, name, values...);
}

template<typename ...Ts>
bool
IniPP::getParameterForced(const std::string& name,
                          Ts&... value)
{
    IPP_Types::Parameter* parameter = findParameter(name, getSection());

    if(parameter == nullptr)
    {
        if(!IPPCore::setParameter(&parameter, name, value...)) return false;
    }

    return IPPCore::getParameter(parameter, value...);
}

template<typename ...Ts>
bool
IniPP::setDefaultParameter(const std::string& name,
                           const Ts... value)
{
    IPP_Types::Parameter* parameter = findParameter(name, getSection());

    if(parameter != nullptr) return true;

    return IPPCore::setParameter(&parameter, name, value...);
}

#endif // INIPP_HPP
