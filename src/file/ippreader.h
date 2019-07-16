#ifndef INIPP_READER_HPP
#define INIPP_READER_HPP

#include <iostream>
#include <vector>

namespace IPP_Types
{
    struct ParameterVector;
} // namespace IPP_Types

namespace IPP_Reader
{
    IPP_Types::ParameterVector* createParameter(const std::string& str);
    std::string readComment(std::istringstream& is);
    std::string readSectionName(const std::string& str);
    std::vector<std::string> toVector(std::istream& is);
} // namespace IPP_Reader

#endif // INIPP_READER_HPP
