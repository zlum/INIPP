﻿#ifndef INIPP_TYPES_HPP
#define INIPP_TYPES_HPP

#include <algorithm>
#include <cstring>
#include <deque>
#include <mutex>
#include <string>
#include <sstream>
#include <vector>

namespace IPP_Types
{
    enum class LockType
    {
        parallel,
        sequential
    };

    struct Parameter
    {
        virtual ~Parameter() = default;

        virtual const std::string getValueAsString() const = 0;
        virtual const std::vector<std::string> getValueAsVector() const = 0;

    public:
        std::string name;
        std::string comment;
        Parameter* nextParameter = nullptr;
    };

    template<typename ...Ts>
    struct ParameterTuple:
            public Parameter
    {
        virtual const std::string getValueAsString() const final;
        virtual const std::vector<std::string> getValueAsVector() const final;

    public:
        std::tuple<Ts...> value;
    };

    struct ParameterVector:
            public Parameter
    {
        virtual const std::string getValueAsString() const final;
        virtual const std::vector<std::string> getValueAsVector() const final;

    public:
        std::vector<std::string> value;
    };

    struct Tokens:
            public std::ctype<char>
    {
        explicit Tokens(const std::ctype_base::mask* mask);

        static const std::ctype_base::mask* getTableFull();
        static const std::ctype_base::mask* getTableDelimiters();
        static const std::ctype_base::mask* getTableSection();
        static const std::ctype_base::mask* createTable(
                std::initializer_list<char> delims) noexcept;
    };

    struct SyncData
    {
        explicit SyncData();
        virtual ~SyncData();

    public:
        std::mutex* mutexParCounter;
        std::mutex* mutexSeqCounter;
        std::mutex* mutexParallel;
        std::mutex* mutexSequential;
        std::unique_lock<std::mutex>* parallelLock;
        std::unique_lock<std::mutex>* sequentialLock;
        uint64_t* parallelCounter;
        uint64_t* sequentialCounter;
        uint64_t* workersCounter;
    };
} // namespace IPP_Types

template<typename ...Ts>
const std::string
IPP_Types::ParameterTuple<Ts...>::getValueAsString() const
{
    std::stringstream stream;
    std::string line;

    stream << value;

    line = stream.str();

    line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
    line.erase(std::remove(line.begin(), line.end(), '\n'), line.end());

    return line;
}

template<typename Func, typename ...Ts, std::size_t ...Is>
void
forEachInTuple(const std::tuple<Ts...>& tuple, Func func,
               std::index_sequence<Is...> /*seq*/)
{
    using expander = int[];
    (void)expander {0, ((void)func(std::get<Is>(tuple)), 0)...};
}

template<typename Func, typename ...Ts>
void
forEachInTuple(const std::tuple<Ts...>& tuple, Func func)
{
    forEachInTuple(tuple, func, std::make_index_sequence<sizeof...(Ts)>());
}

template<typename ...Ts>
const std::vector<std::string>
IPP_Types::ParameterTuple<Ts...>::getValueAsVector() const
{
    std::vector<std::string> vec;

    forEachInTuple(value, [&](const auto& x)
    {
        std::stringstream ss;
        ss << x;
        vec.push_back(ss.str());
    });

    return vec;
}

#endif // INIPP_TYPES_HPP