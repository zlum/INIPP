#ifndef INIPP_SECTION_H
#define INIPP_SECTION_H

#include <string>
#include <unordered_map>

namespace IPP_Types
{
    struct Parameter;
} // namespace IPP_Types

class IPPSection
{
public:
    explicit IPPSection();
    explicit IPPSection(IPP_Types::Parameter* parameter);
    virtual ~IPPSection() = default;

    bool addParameter(IPP_Types::Parameter* parameter);
    bool replaceParameter(IPP_Types::Parameter* parameter);
    bool rmParameter(const std::string& parameter);

    IPP_Types::Parameter* getParameter(const std::string& parameter) const;
    IPP_Types::Parameter* getFirstParameter() const;
    IPP_Types::Parameter* getLastParameter() const;

    std::unordered_map<std::string, IPP_Types::Parameter*>* getParameters();

private:
    std::unordered_map<std::string, IPP_Types::Parameter*> _parameters;
    IPP_Types::Parameter* _firstParameter;
    IPP_Types::Parameter* _lastParameter;
};

#endif // INIPP_SECTION_H
