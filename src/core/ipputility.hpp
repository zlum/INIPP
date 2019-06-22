#ifndef INIPP_UTILITY_HPP
#define INIPP_UTILITY_HPP

#include "src/streams/streams.hpp"

namespace IPP_Utility
{
    template<typename ...Ts>
    void
    istreamToArgs(std::istream& is, Ts&&... values);

    template<typename Tuple, typename ...Ts>
    void
    tupleToArgs(const Tuple& tuple, Ts&&... values);

    template<typename T>
    bool
    stringToValue(const std::string& string, T& value);

    template<typename T>
    bool
    vectorToArgs(unsigned i,
                 const std::vector<std::string>& vec, T& value);

    template<typename ...Ts, typename T>
    bool
    vectorToArgs(unsigned i, // TODO: Beautify function call
                 const std::vector<std::string>& vec, T& value, Ts&... values);
} // namespace IPP_Utility

template<typename T>
bool
IPP_Utility::stringToValue(const std::string& string, T& value)
{
    std::istringstream isstream(string);
    isstream.imbue(std::locale(std::locale(),
                               new IPP_Types::Tokens(
                                   IPP_Types::Tokens::getTableDelimiters())));

    isstream >> value;

    return !isstream.fail();
}

template<typename T>
bool
IPP_Utility::vectorToArgs(unsigned i,
                          const std::vector<std::string>& vec, T& value)
{
    return stringToValue(vec.at(i), value);
}

template<typename ...Ts, typename T>
bool
IPP_Utility::vectorToArgs(unsigned i,
                          const std::vector<std::string>& vec,
                          T& value, Ts&... values)
{
    if(!stringToValue(vec.at(i++), value)) return false;

    return vectorToArgs(i, vec, values...);
}

template<typename ...Ts>
void
IPP_Utility::istreamToArgs(std::istream& is, Ts&&... values)
{
    std::tuple<Ts...> tuple =
            std::forward_as_tuple(std::forward<Ts>(values)...);
    IPP_Tuple::istreamToTuple(is, tuple,
                              std::make_index_sequence<sizeof...(Ts)>{});
}

template<typename Tuple, typename ...Ts>
void
IPP_Utility::tupleToArgs(const Tuple& tuple, Ts&&... values)
{
    std::tuple<Ts...> args =
            std::forward_as_tuple(std::forward<Ts>(values)...);
    args = tuple;
}

#endif // INIPP_UTILITY_HPP
