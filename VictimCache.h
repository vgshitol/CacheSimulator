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
    VictimCache(unsigned long size, unsigned long associativity, unsigned long blockSize) : Cache(size,
                                                                                                  associativity,
                                                                                                  blockSize) {

    }

    cache_interface swap(memory_address memory_address1, bool dirtyBit){

        this->performanceParameters.cacheHits++;

        /**
         * Store the Swap Data in the Memory Address Details Structure
         */
        cache_interface cacheInterface1;

        memory_address memory_address2; // The address existing in the Victim Cache at the moment
        memory_address2 = memory_address1;
        memory_address2.tag = this->tags[memory_address1.setIndex][memory_address1.assocIndex].tag;

        cacheInterface1.swapMemoryAddress = this->makeAddress(memory_address2);
        cacheInterface1.swapDirtyBit = this->tags[memory_address1.setIndex][memory_address1.assocIndex].dirtyBit;
        cacheInterface1.swapFlag = true;

        this->tags[memory_address1.setIndex][memory_address1.assocIndex].tag = 0;
        this->tags[memory_address1.setIndex][memory_address1.assocIndex].validBit = false;
        this->tags[memory_address1.setIndex][memory_address1.assocIndex].dirtyBit = false;
        /**
         * Memory Address Structure Details To be Loaded in the Cache
         */
        this->tags[memory_address1.setIndex][memory_address1.assocIndex].tag = memory_address1.tag; /** update the new tag with new memory address**/
        this->tags[memory_address1.setIndex][memory_address1.assocIndex].validBit = true;

        this->tags[memory_address1.setIndex][memory_address1.assocIndex].dirtyBit = dirtyBit;

        updateRecencyNumber(memory_address1.setIndex,memory_address1.assocIndex);

        return cacheInterface1;
    }

    cache_interface rwCache(unsigned long int currentMemoryAddress, unsigned long int evictedMemoryAddress, bool evictedDirtyBit) {

        memory_address mem_add_split, evicted_mem_add_split;
        cache_interface cacheInterface1;

        mem_add_split.blockOffset = ((1 << this->bitsOffset.blockOffsetBits) - 1) & currentMemoryAddress;
        mem_add_split.setIndex = ((1 << this->bitsOffset.indexBits) - 1) & (currentMemoryAddress >> this->bitsOffset.blockOffsetBits);
        mem_add_split.tag =  (currentMemoryAddress >> (this->bitsOffset.blockOffsetBits + this->bitsOffset.indexBits));

        evicted_mem_add_split.blockOffset = ((1 << this->bitsOffset.blockOffsetBits) - 1) & evictedMemoryAddress;
        evicted_mem_add_split.setIndex = ((1 << this->bitsOffset.indexBits) - 1) & (evictedMemoryAddress >> this->bitsOffset.blockOffsetBits);
        evicted_mem_add_split.tag =  (evictedMemoryAddress >> (this->bitsOffset.blockOffsetBits + this->bitsOffset.indexBits));
        performanceParameters.swapRequests++;

        if(lookForMatchInSet(&mem_add_split)){
            performanceParameters.swaps++;
            evicted_mem_add_split.assocIndex = mem_add_split.assocIndex;
            /** We got a cache hit**/
            return this->swap(evicted_mem_add_split, evictedDirtyBit);
        }

        /** No Cache Hit we have to replace the LRU so get the LRU Index **/
        cacheInterface1 = this->evictLRU(evicted_mem_add_split, evictedDirtyBit);

        return cacheInterface1;
    }

    cache_interface evictLRU(memory_address memory_address1, bool evictedDirtyBit) {

        cache_interface cacheInterface1 = cache_interface();

        memory_address1.assocIndex = findLRUIndex(memory_address1.setIndex); /** find the Least Recently Used index **/
        cacheInterface1.evictionFlag = false;

        /**
         * Check if all the blocks in a set are filled . Even if a single block is empty
         * fill in that spot with the recent address
         */
        bool isValidBitZero = false;
        for (unsigned long int assoc = 0; assoc < this->associativity; assoc++){
            if(!this->tags[memory_address1.setIndex][assoc].validBit){
                isValidBitZero = true;
                break;
            }
        }

        if((!isValidBitZero)){
            performanceParameters.evictions++;

            cacheInterface1.evictionFlag = true;
            memory_address memory_address2;
            memory_address2 = memory_address1;
            memory_address2.tag = this->tags[memory_address1.setIndex][memory_address1.assocIndex].tag;

            cacheInterface1.evictedMemoryAddress = this->makeAddress(memory_address2);
            cacheInterface1.dirtyBit = this->tags[memory_address1.setIndex][memory_address1.assocIndex].dirtyBit;
            cacheInterface1.rw = dirtyBit2Char(cacheInterface1.dirtyBit);

            this->tags[memory_address1.setIndex][memory_address1.assocIndex].dirtyBit = false; /** Set the Dirty Bit false as you are evicting this tag and a new
             tag will be replaced who will have its own dirty bit policy  **/

        }

        this->tags[memory_address1.setIndex][memory_address1.assocIndex].tag = memory_address1.tag; /** update the new tag with new memory address**/
        this->tags[memory_address1.setIndex][memory_address1.assocIndex].validBit = true;
        this->tags[memory_address1.setIndex][memory_address1.assocIndex].dirtyBit = evictedDirtyBit;


        updateRecencyNumber(memory_address1.setIndex,memory_address1.assocIndex);

        return cacheInterface1;
    }
};


#endif //ECE_563_PROJECT_VICTIMCACHE_H
