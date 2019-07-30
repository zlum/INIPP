#include "ipptypes.hpp"

using namespace std;

static const ctype<char>::mask* tokensTableFull =
        IPP_Types::Tokens::createTable({' ', '=', ',', ';', '[', ']'});

static const ctype<char>::mask* tokensTableDelimiters =
        IPP_Types::Tokens::createTable({','});

static const ctype<char>::mask* tokensTableSection =
        IPP_Types::Tokens::createTable({'[', ']'});

IPP_Types::Tokens::Tokens(const ctype_base::mask* mask):
    ctype<char>(mask)
{
}

const ctype_base::mask*
IPP_Types::Tokens::getTableFull()
{
    return tokensTableFull;
}

const ctype_base::mask*
IPP_Types::Tokens::getTableDelimiters()
{
    return tokensTableDelimiters;
}

const ctype_base::mask*
IPP_Types::Tokens::getTableSection()
{
    return tokensTableSection;
}

const ctype_base::mask*
IPP_Types::Tokens::createTable(initializer_list<char> delims) noexcept
{
    static const ctype::mask* const_rc = ctype::classic_table();
    auto rc = new ctype::mask[ctype::table_size];
    memcpy(rc, const_rc, ctype::table_size * sizeof(ctype::mask));

    rc[static_cast<unsigned char>(' ')] = ctype_base::alpha;

    for(const auto& next : delims)
    {
        rc[static_cast<unsigned char>(next)] = ctype_base::space;
    }

    return rc;
}

IPP_Types::SyncData::SyncData():
    mutexParCounter(new mutex),
    mutexSeqCounter(new mutex),
    mutexParallel(new mutex),
    mutexSequential(new mutex),
    parallelLock(nullptr),
    sequentialLock(nullptr),
    parallelCounter(new uint64_t(0)),
    sequentialCounter(new uint64_t(0)),
    workersCounter(new uint64_t(0))
{
}

IPP_Types::SyncData::~SyncData()
{
    // Locks must not be deleted here
    delete mutexParCounter;
    delete mutexSeqCounter;
    delete mutexParallel;
    delete mutexSequential;
    delete parallelCounter;
    delete sequentialCounter;
    delete workersCounter;
}

const string
IPP_Types::ParameterVector::getValueAsString() const
{
    string values;

    for(const auto& next : value)
    {
        if(!values.empty()) values += ", ";
        values.append(next);
    }

    return values;
}

const vector<string>
IPP_Types::ParameterVector::getValueAsVector() const
{
    return value;
}
