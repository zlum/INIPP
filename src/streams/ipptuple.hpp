#ifndef TUPLE_H
#define TUPLE_H

#include "src/types/ipptypes.hpp"

#include <iostream>

namespace IPP_Tuple
{
    template<typename T>
    bool
    istreamToValue(std::istream& is, T& value);

    template<typename Tuple, std::size_t... I>
    void
    istreamToTuple(std::istream& is, Tuple& values);

    template<std::size_t> struct int_ {};

    template<typename Tuple, size_t Pos>
    std::ostream&
    print(std::ostream& os, const Tuple& values, int_<Pos> pos);

    template<typename Tuple>
    std::ostream&
    print(std::ostream& os, const Tuple& values, int_<1> pos);

    template<std::size_t I = 0, typename... Tp>
    typename std::enable_if<I == sizeof...(Tp), void>::type
    tupleToVector(const std::tuple<Tp...>& /*t*/,
                  std::vector<std::string>* /*vec*/)
    {
    }

    template<std::size_t I = 0, typename... Tp>
    typename std::enable_if<I < sizeof...(Tp), void>::type
    tupleToVector(const std::tuple<Tp...>& t,
                  std::vector<std::string>* vec)
    {
        std::string val;
        std::stringstream ss;

        ss.imbue(std::locale(std::locale(),
                             new IPP_Types::Tokens(
                                 IPP_Types::Tokens::getTableDelimiters())));

        ss << std::get<I>(t);
        ss >> val;
        vec->push_back(val);
        tupleToVector<I + 1, Tp...>(t, vec);
    }
} // namespace IPP_Tuple

template<typename ...Ts>
const std::vector<std::string>
IPP_Types::ParameterTuple<Ts...>::getValueAsVector() const
{
    std::vector<std::string> vec;
    IPP_Tuple::tupleToVector(value, &vec);

    return vec;
}

template<typename T>
bool
IPP_Tuple::istreamToValue(std::istream& is, T& value)
{
    is.imbue(std::locale(std::locale(),
                         new IPP_Types::Tokens(
                             IPP_Types::Tokens::getTableDelimiters())));

    is >> value;

    return true;
}

template<typename Tuple, std::size_t... I>
void
IPP_Tuple::istreamToTuple(std::istream& is, Tuple& values)
{
    for(auto& next : {values})
    {
        istreamToValue(is, next);
    }
}

template<typename Tuple, size_t Pos>
std::ostream&
IPP_Tuple::print(std::ostream& os, const Tuple& values,
                 int_<Pos> /*pos*/)
{
    os << std::get<std::tuple_size<Tuple>::value - Pos>(values) << ", ";
    return print(os, values, int_<Pos - 1>());
}

template<typename Tuple>
std::ostream&
IPP_Tuple::print(std::ostream& os, const Tuple& values,
                 int_<1> /*pos*/)
{
    return os << std::get<std::tuple_size<Tuple>::value - 1>(values);
}

#endif // TUPLE_H
