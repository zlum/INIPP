#ifndef INIPP_CHAINLOCK_H
#define INIPP_CHAINLOCK_H

#include "src/types/ipptypes.hpp"

#include <string>
#include <mutex>
#include <unordered_map>

class ChainLock
{
public:
    explicit ChainLock(const std::string& name,
                       IPP_Types::LockType lockType);
    virtual ~ChainLock();

    static void beginParallel(IPP_Types::SyncData* syncData);
    static void beginSequential(IPP_Types::SyncData* syncData);
    static void endParallel(IPP_Types::SyncData* syncData);
    static void endSequential(IPP_Types::SyncData* syncData);

private:
    static IPP_Types::SyncData* getSyncData(const std::string& name);
    static bool tryRmSyncData(const std::string& name);

private:
    static std::unordered_map<std::string, IPP_Types::SyncData*> _syncMap;
    static std::mutex _syncMapMutex;
    std::string _name;
    IPP_Types::LockType _lockType;
};

#endif // INIPP_CHAINLOCK_H
