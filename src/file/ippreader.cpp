#include "ippreader.h"

#include "src/types/ipptypes.hpp"

using namespace std;

IPP_Types::ParameterVector*
IPP_Reader::createParameter(const string& str)
{
    istringstream is{str};
    string valuesString;
    IPP_Types::ParameterVector* parameter;

    getline(is, valuesString, ';');
    istringstream values{valuesString};

    parameter = new IPP_Types::ParameterVector{};

    values.imbue(locale(locale(),
                        new IPP_Types::Tokens(
                            IPP_Types::Tokens::getTableFull())));

    values >> parameter->name;
    parameter->value = toVector(values);
    parameter->comment = readComment(is);

    return parameter;
}

string
IPP_Reader::readComment(istringstream& is)
{
    string comment;

    is >> comment;

    if(!is.eof())
    {
        is.seekg(unsigned(unsigned(is.tellg()) - comment.size()));
        getline(is, comment);
    }

    return comment;
}

string
IPP_Reader::readSectionName(const string& str)
{
    istringstream is(str);
    string sectionName;

    is.imbue(locale(locale(),
                    new IPP_Types::Tokens(
                        IPP_Types::Tokens::getTableSection())));

    is >> sectionName;

    return sectionName;
}

vector<string>
IPP_Reader::toVector(istream& is)
{
    vector<string> values;
    string value;

    while(!is.eof())
    {
        is >> value;

        if(!is.eof())
        {
            is.seekg(unsigned(unsigned(is.tellg()) - value.size()));
            getline(is, value, ',');
        }

        values.push_back(value);
    }

    return values;
}
