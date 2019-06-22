#ifndef INIPP_STREAMS_HPP
#define INIPP_STREAMS_HPP

#include "ipptuple.hpp"
#include "src/types/ipptypes.hpp"

template<typename ...Ts>
inline std::istream& operator >>(std::istream& is,
                                 const IPP_Types::ParameterTuple<Ts...>& parameter);
inline std::ostream& operator <<(std::ostream& os,
                                 const IPP_Types::Parameter& parameter);
template<typename ...Ts>
inline std::ostream& operator <<(std::ostream& out, const std::tuple<Ts...>& t);

// Interpret int8_t & uint8_t as numbers
inline std::istream& operator >>(std::istream& is, int8_t& value);
inline std::istream& operator >>(std::istream& is, uint8_t& value);
inline std::ostream& operator <<(std::ostream& os, const int8_t& value);
inline std::ostream& operator <<(std::ostream& os, const uint8_t& value);

template<typename ...Ts>
inline std::istream& operator >>(std::istream& is,
                                 const IPP_Types::ParameterTuple<Ts...>& parameter)
{
    is.exceptions(is.exceptions() | std::ios_base::badbit);
    is.imbue(std::locale(std::locale(),
                         new IPP_Types::Tokens(
                             IPP_Types::Tokens::getTableFull())));

    is >> parameter.name;
    IPP_Tuple::istreamToTuple(is, parameter.value,
                              std::make_index_sequence<sizeof...(Ts)>{});

    return is;
}

inline std::ostream& operator <<(std::ostream& os,
                                 const IPP_Types::Parameter& parameter)
{
    os.exceptions(os.exceptions() | std::ios_base::badbit);

    os << parameter.name << " = " <<
          parameter.getValueAsString();

    if(!parameter.comment.empty())
    {
        os << "; " <<
              parameter.comment;
    }

    os << std::endl;

    return os;
}

template<typename ...Ts>
inline std::ostream& operator <<(std::ostream& out, const std::tuple<Ts...>& t)
{
    return IPP_Tuple::print(out, t, IPP_Tuple::int_<sizeof...(Ts)>());
}

// Типы int8_t и uint8_t по умолчанию работают с
// потоками как signed char и unsigned char соответственно.
// Следующие четыре функции изменяют поведение потоков для
// этих типов на соответствующее работе с int16_t и uint16_t.
inline std::istream& operator >>(std::istream& is, int8_t& value)
{
    is.exceptions(is.exceptions() | std::ios_base::badbit);

    int16_t tmp;
    is >> tmp;

    if((tmp < INT8_MIN) || (tmp > INT8_MAX))
    {
        is.clear(std::ios_base::failbit);
        return is;
    }

    value = int8_t(tmp);

    return is;
}

inline std::istream& operator >>(std::istream& is, uint8_t& value)
{
    is.exceptions(is.exceptions() | std::ios_base::badbit);

    uint16_t tmp;
    is >> tmp;

    if(tmp > UINT8_MAX)
    {
        is.clear(std::ios_base::failbit);
        return is;
    }

    value = uint8_t(tmp);

    return is;
}

inline std::ostream& operator <<(std::ostream& os, const int8_t& value)
{
    os.exceptions(os.exceptions() | std::ios_base::badbit);

    return os << static_cast<int16_t>(value);
}

inline std::ostream& operator <<(std::ostream& os, const uint8_t& value)
{
    os.exceptions(os.exceptions() | std::ios_base::badbit);

    return os << static_cast<uint16_t>(value);
}

#endif // INIPP_STREAMS_HPP
