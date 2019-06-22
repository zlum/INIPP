#include "chainlock.h"

using namespace std;

unordered_map<string, IPP_Types::SyncData*> ChainLock::_syncMap;
mutex ChainLock::_syncMapMutex;

ChainLock::ChainLock(const string& name, IPP_Types::LockType lockType):
    _name(name),
    _lockType(lockType)
{
    IPP_Types::SyncData* syncData = getSyncData(_name);

    switch(_lockType)
    {
    case IPP_Types::LockType::parallel:
        beginParallel(syncData);
        break;
    case IPP_Types::LockType::sequential:
        beginSequential(syncData);
        break;
    }
}

ChainLock::~ChainLock()
{
    IPP_Types::SyncData* syncData = getSyncData(_name);

    switch(_lockType)
    {
    case IPP_Types::LockType::parallel:
        endParallel(syncData);
        break;
    case IPP_Types::LockType::sequential:
        endSequential(syncData);
        break;
    }

    if(*syncData->parallelCounter == 0 && *syncData->sequentialCounter == 0)
    {
        tryRmSyncData(_name);
    }
}

void
ChainLock::beginParallel(IPP_Types::SyncData* syncData)
{
    lock_guard<mutex> lockReadCounter(*syncData->mutexParCounter);
    if(*syncData->parallelCounter == 0)
    {
        syncData->parallelLock =
                new unique_lock<mutex>(*syncData->mutexParallel);
    }

    *syncData->parallelCounter += 1;
}

void
ChainLock::beginSequential(IPP_Types::SyncData* syncData)
{
    syncData->parallelLock =
            new unique_lock<mutex>(*syncData->mutexParallel);
    syncData->sequentialLock =
            new unique_lock<mutex>(*syncData->mutexSequential);
    lock_guard<mutex> lockWriteCounter(*syncData->mutexSeqCounter);

    *syncData->sequentialCounter += 1;
}

void
ChainLock::endParallel(IPP_Types::SyncData* syncData)
{
    lock_guard<mutex> lockReadCounter(*syncData->mutexParCounter);
    if(*syncData->parallelCounter == 1)
    {
        delete syncData->parallelLock;
    }

    *syncData->parallelCounter -= 1;
}

void
ChainLock::endSequential(IPP_Types::SyncData* syncData)
{
    delete syncData->sequentialLock;
    delete syncData->parallelLock;
    lock_guard<mutex> lockWriteCounter(*syncData->mutexSeqCounter);

    *syncData->sequentialCounter -= 1;
}

IPP_Types::SyncData*
ChainLock::getSyncData(const string& name)
{
    lock_guard<mutex> guardMap(_syncMapMutex);

    auto search = _syncMap.find(name);

    if(search != _syncMap.end())
    {
        *search->second->workersCounter += 1;
        return search->second;
    }

    auto sync = new IPP_Types::SyncData();
    _syncMap.emplace(make_pair(name, sync));

    return sync;
}

bool
ChainLock::tryRmSyncData(const string& name)
{
    lock_guard<mutex> guardMap(_syncMapMutex);

    auto search = _syncMap.find(name);

    *search->second->workersCounter -= 1;
    if(search != _syncMap.end())
    {
        if(*search->second->workersCounter == 0)
        {
            delete search->second;
            _syncMap.erase(search);

            return true;
        }
    }

    return false;
}
