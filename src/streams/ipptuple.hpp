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
} // namespace IPP_Tuple

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
