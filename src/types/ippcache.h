#ifndef INIPP_CACHE_H
#define INIPP_CACHE_H

#include "ippsection.h"

#include <deque>
#include <string>
#include <unordered_map>

class IPPCache
{
public:
    explicit IPPCache() = default;
    virtual ~IPPCache();

    bool addParameter(IPP_Types::Parameter* parameter,
                      const std::string& section);
    bool replaceParameter(IPP_Types::Parameter* parameter,
                          const std::string& section);
    bool rmParameter(IPP_Types::Parameter* parameter,
                     const std::string& section);

    IPP_Types::Parameter* getParameter(const std::string& parameter,
                                       const std::string& section) const;

    IPPSection* getSection(const std::string& section);
    std::unordered_map<std::string, IPPSection>* getSections();

    std::deque<std::string>* getSequence();

private:
    std::unordered_map<std::string, IPPSection> _sections;
    std::deque<std::string> _sequence;
};

#endif // INIPP_CACHE_H
