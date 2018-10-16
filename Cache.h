//
// Created by Vishal Shitole on 9/29/18.
//

#ifndef ECE_563_PROJECT_CACHE_H
#define ECE_563_PROJECT_CACHE_H

#include "BitsOffset.h"
#include "PerformanceParameters.h"
#include <vector>

using namespace std;

#define ADDRESS_BITS 32

typedef struct cacheInterface{
    bool evictionFlag = false;
    bool cacheHit = false;
    bool dirtyBit = false;
    unsigned long int evictedMemoryAddress;
    bool swapFlag = false;
    bool swapDirtyBit = false;
    unsigned long int swapMemoryAddress;
    char rw;
}cache_interface;


typedef struct cacheElements{
    unsigned long int tag = 0;
    unsigned long int leastRecentlyUsed = 0;
    bool validBit = false;
    bool dirtyBit = false;
}cache_elements;

typedef struct memoryAddress{
    unsigned long int tag = 0;
    unsigned long int setIndex = 0;
    unsigned long int assocIndex = 0;
    unsigned long int blockOffset = 0;
}memory_address;


class Cache {
public:
    unsigned long int countCacheEntry;
private:
    /** get the structure of cache by realising the following parameters **/
    unsigned long int size;
    unsigned long int blockSize;
    unsigned long int sets;

protected:
    /** get the number of bits of the following parameters to synthesize the structure of cache **/
    BitsOffset bitsOffset;

    /**Record Performance Parameters**/
    PerformanceParameters performanceParameters;

    bool lookForMatchInSet(memory_address * memory_address1);
    unsigned long findLRUIndex(unsigned long int setIndex);
    unsigned long int associativity;
    /** generate the arrays required to route the memory addresses in the cache **/
    vector<vector <cache_elements>> tags;

    void updateRecencyNumber(unsigned long int setIndex, unsigned long int assocIndex);

    bool cacheHit(memory_address memory_address1, char rw);
    cache_interface evictLRU(memory_address memory_address1,  char rw);
public:
    const PerformanceParameters &getPerformanceParameters() const;

public:
    Cache(
            unsigned long int size,
            unsigned long int associativity,
            unsigned long int blockSize
            );
    void displayValues();
    void displayTags();

    cache_interface rwCache(unsigned long int memoryAddress, char rw = 'r');
    unsigned long int makeAddress(memory_address memory_address1);
    bool swapData(cache_interface cacheInterface1, char rw);
    char dirtyBit2Char(bool dirtyBit);

private:
    void setSets(void);
    void assignArrayValues(void);
    void setBits(void);
};


#endif //ECE_563_PROJECT_CACHE_H
