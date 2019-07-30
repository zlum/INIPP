#ifndef INIPP_TYPES_HPP
#define INIPP_TYPES_HPP

#include <algorithm>
#include <cstring>
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
} // namespace IPP_Types

#endif // INIPP_TYPES_HPP
