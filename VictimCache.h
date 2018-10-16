//
// Created by Vishal Shitole on 10/13/18.
//

#ifndef ECE_563_PROJECT_VICTIMCACHE_H
#define ECE_563_PROJECT_VICTIMCACHE_H

#include "Cache.h"
//
//typedef struct victimCacheLink {
//    bool evictionFlag = false;
//    bool cacheHit = false;
//    unsigned long int evictedMemoryAddress;
//    bool swapFlag = false;
//    unsigned long int swapMemoryAddress;
//}victim_cache_link;

class VictimCache: public Cache{

public:
    VictimCache(unsigned long size, unsigned long associativity, unsigned long blockSize);
    cache_interface swap(memory_address memory_address1, bool dirtyBit);
    cache_interface rwCache(unsigned long int memoryAddress, unsigned long int memoryAddress2,  bool dirtyBit);
    cache_interface evictLRU(memory_address memory_address1,  bool evictedDirtyBit);
};


#endif //ECE_563_PROJECT_VICTIMCACHE_H
