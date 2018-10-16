//
// Created by Vishal Shitole on 9/29/18.
//

#include <iostream>
#include <math.h>
#include "Cache.h"

using namespace std;

Cache::Cache(
        unsigned long int size,
        unsigned long int associativity,
        unsigned long int blockSize
){
    this->size = size;
    this->associativity = associativity;
    this->blockSize = blockSize;
    if(size == 0)
        return ;

    this->setSets();
    this->setBits();
    this->assignArrayValues();
}

void Cache::assignArrayValues() {
    unsigned long int maxLRUValue = this->associativity-1;
    this->tags.resize(this->sets);
    for (int set = 0; set < this->sets; ++set) {
        this->tags[set].resize(this->associativity);
        for (int assoc = 0; assoc < this->associativity; ++assoc) {
            this->tags[set][assoc].leastRecentlyUsed = maxLRUValue;
        }
    }
}


void Cache::setSets(void) {
    this->sets = (unsigned int)(this->size / (this->associativity * this->blockSize));
 }

void Cache::displayValues() {
    cout << "My Values are : " << this->size << " \t" << this->associativity << "\t" << this->blockSize << endl;
    cout << "Number of sets : " << this->sets << endl;
    cout << "Bits of Blocks : " << "Tag Bits: " << this->bitsOffset.tagBits << " \t" <<"Index Bits: " << this->bitsOffset.indexBits << " \t" <<"Block Offset Bits: " << this->bitsOffset.blockOffsetBits << endl;
}

void Cache::setBits() {
    this->bitsOffset.blockOffsetBits = (unsigned int)(log2(this->blockSize));
    this->bitsOffset.indexBits = (unsigned int)(log2(this->sets)); /** number of index bits  **/
    this->bitsOffset.tagBits = ADDRESS_BITS - this->bitsOffset.blockOffsetBits - this->bitsOffset.indexBits; /** for reference **/
}

unsigned long Cache::findLRUIndex(unsigned long int setIndex) {
    unsigned long assocIndex = 0;
    unsigned long int assoc = 0;
    while (assoc < this->associativity){
        if(this->tags[setIndex][assoc].leastRecentlyUsed == this->associativity - 1){
            assocIndex = assoc;
            break;
        }
        assoc++;
    }

    return assocIndex;
}

bool Cache::lookForMatchInSet(memory_address * memory_address1) {
    bool match = false;
    for (unsigned  long int assoc = 0; assoc < this->associativity; assoc++){
        if((this->tags[(*memory_address1).setIndex][assoc].tag == (*memory_address1).tag) && (this->tags[(*memory_address1).setIndex][assoc].validBit)){
            match = true; // cache hit
            (*memory_address1).assocIndex = assoc;
        }
    }

    return match;
}

void Cache::updateRecencyNumber(unsigned long int setIndex, unsigned long int assocIndex) {
    for (unsigned long int assoc = 0; assoc < this->associativity; assoc++){
        if(this->tags[setIndex][assoc].leastRecentlyUsed < this->tags[setIndex][assocIndex].leastRecentlyUsed){
            this->tags[setIndex][assoc].leastRecentlyUsed++;
        }

    }
    this->tags[setIndex][assocIndex].leastRecentlyUsed = 0; /** Update the MRU Block Recency Number **/

}

const PerformanceParameters &Cache::getPerformanceParameters() const {
    return performanceParameters;
}

/**
 *
 */
void Cache::displayTags() {
    unsigned long int temp = 0;
    bool dirtyBit = 0;
    for(unsigned long int setIndex = 0; setIndex < this->tags.size(); setIndex++){
        cout << "set  " << setIndex << ": ";
        for (int assoc = 0; assoc < this->tags[setIndex].size(); ++assoc) {
             cout << hex << this->tags[setIndex][assoc].tag << " " <<  this->tags[setIndex][assoc].dirtyBit << " "  ;
        }
        cout << dec <<endl;
    }
}

/**
 *
 * @param memory_address1
 * @return
 */
unsigned long int Cache::makeAddress(memory_address memory_address1) {
    unsigned long int address;
    address = memory_address1.tag;
    address = (address << this->bitsOffset.indexBits) + memory_address1.setIndex ;
    address = (address << this->bitsOffset.blockOffsetBits) + (memory_address1.blockOffset);
    return address;
}

/**
 *
 * @param memory_address1
 * @param rw
 * @return
 */
bool Cache::cacheHit(memory_address memory_address1,  char rw) {

    this->performanceParameters.cacheHits++;
    if(rw == 'r') this->performanceParameters.cacheReadHits++;
    else if(rw == 'w') this->performanceParameters.cacheWriteHits++;

    this->tags[memory_address1.setIndex][memory_address1.assocIndex].tag = memory_address1.tag; /** update the new tag with new memory address**/
    this->tags[memory_address1.setIndex][memory_address1.assocIndex].validBit = true;

    if((rw == 'w') && (!this->tags[memory_address1.setIndex][memory_address1.assocIndex].dirtyBit)){ // Write Operation Successful
        this->tags[memory_address1.setIndex][memory_address1.assocIndex].dirtyBit = true;
    }

    updateRecencyNumber(memory_address1.setIndex,memory_address1.assocIndex);

    return true;
}

/**
 *
 * @param memory_address1
 * @param rw
 * @return
 */
cache_interface Cache::evictLRU(memory_address memory_address1,  char rw) {

    performanceParameters.cacheMiss++;
    if(rw == 'r') performanceParameters.cacheReadMiss++;
    else if(rw == 'w') performanceParameters.cacheWriteMiss++;

    cache_interface cacheInterface1 = cache_interface();

    memory_address1.assocIndex = findLRUIndex(memory_address1.setIndex); /** find the Least Recently Used index **/

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

    if((rw == 'w') && (!this->tags[memory_address1.setIndex][memory_address1.assocIndex].dirtyBit)){ // Write Operation Successful
        this->tags[memory_address1.setIndex][memory_address1.assocIndex].dirtyBit = true;
    }

    updateRecencyNumber(memory_address1.setIndex,memory_address1.assocIndex);

    return cacheInterface1;
}

cache_interface Cache::rwCache(unsigned long int memoryAddress, char rw) {

    this->countCacheEntry = this->countCacheEntry+1;
//    cout << "#" << this->countCacheEntry << ": " << rw << hex << memoryAddress << dec << endl;
    cache_interface cacheInterface1;
    memory_address mem_add_split;

    mem_add_split.blockOffset = ((1 << this->bitsOffset.blockOffsetBits) - 1) & memoryAddress;
    mem_add_split.setIndex = ((1 << this->bitsOffset.indexBits) - 1) & (memoryAddress >> this->bitsOffset.blockOffsetBits);
    mem_add_split.tag =  (memoryAddress >> (this->bitsOffset.blockOffsetBits + this->bitsOffset.indexBits));

    if(lookForMatchInSet(&mem_add_split)){
        /** We got a cache hit**/
        cacheInterface1.cacheHit = this->cacheHit(mem_add_split,  rw);
        return cacheInterface1;
    }

    /** No Cache Hit we have to replace the LRU so get the LRU Index **/
    cacheInterface1 = this->evictLRU(mem_add_split, rw);


    return cacheInterface1;
}

bool Cache::swapData(cache_interface cacheInterface1, char rw) {

    if(!cacheInterface1.swapFlag)
        return false;

    memory_address mem_add_split;

    mem_add_split.blockOffset = ((1 << this->bitsOffset.blockOffsetBits) - 1) & cacheInterface1.swapMemoryAddress;
    mem_add_split.setIndex = ((1 << this->bitsOffset.indexBits) - 1) & (cacheInterface1.swapMemoryAddress >> this->bitsOffset.blockOffsetBits);
    mem_add_split.tag =  (cacheInterface1.swapMemoryAddress >> (this->bitsOffset.blockOffsetBits + this->bitsOffset.indexBits));

    if(lookForMatchInSet(&mem_add_split)) {
        /** We got a cache hit**/
        this->tags[mem_add_split.setIndex][mem_add_split.assocIndex].tag = mem_add_split.tag; /** update the new tag with new memory address**/
        this->tags[mem_add_split.setIndex][mem_add_split.assocIndex].dirtyBit = cacheInterface1.swapDirtyBit;
        if( rw == 'w')
            this->tags[mem_add_split.setIndex][mem_add_split.assocIndex].dirtyBit = true;

    }

    return true;
}

char Cache::dirtyBit2Char(bool dirtyBit) {
    if(dirtyBit)
        return 'w';

    return 'r';
}


