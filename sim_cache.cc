#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "sim_cache.h"
#include "Cache.h"
#include "VictimCache.h"
#include <typeinfo>
#include <iomanip>
#include <fstream>

using namespace std;
/*  argc holds the number of command line arguments
    argv[] holds the commands themselves

    Example:-
    sim_cache 32 8192 4 7 262144 8 gcc_trace.txt
    argc = 8
    argv[0] = "sim_cache"
    argv[1] = "32"
    argv[2] = "8192"
    ... and so on
*/

enum cacheStructure {
    NOVICTIMCACHENOL2,
    VICTIMCACHEL1NOL2,
    L2NOVICTIMCACHE,
    VICTIMCACHEL1ANDL2
};

int main (int argc, char* argv[])
{
    FILE *FP;               // File handler
    char *trace_file;       // Variable that holds trace file name;
    cache_params params;    // look at sim_cache.h header file for the the definition of struct cache_params
    char rw;                // variable holds read/write type read from input file. The array size is 2 because it holds 'r' or 'w' and '\0'. Make sure to adapt in future projects
    unsigned long int addr; // Variable holds the address read from input file

    enum cacheStructure cacheStructure1 = NOVICTIMCACHENOL2;

    if(argc != 8)           // Checks if correct number of inputs have been given. Throw error and exit if wrong
    {
        printf("Error: Expected inputs:7 Given inputs:%d\n", argc-1);
        exit(EXIT_FAILURE);
    }
    
    // strtoul() converts char* to unsigned long. It is included in <stdlib.h>
    params.block_size       = strtoul(argv[1], NULL, 10);
    params.l1_size          = strtoul(argv[2], NULL, 10);
    params.l1_assoc         = strtoul(argv[3], NULL, 10);
    params.vc_num_blocks    = strtoul(argv[4], NULL, 10);
    params.l2_size          = strtoul(argv[5], NULL, 10);
    params.l2_assoc         = strtoul(argv[6], NULL, 10);
    trace_file              = argv[7];

    // Open trace_file in read mode
    FP = fopen(trace_file, "r");
    if(FP == NULL)
    {
        // Throw error and exit if fopen() failed
        printf("Error: Unable to open file %s\n", trace_file);
        exit(EXIT_FAILURE);
    }

    // Print params
    printf( "===== Simulator configuration =====\n"
                               "  L1_BLOCKSIZE:                     %lu\n"
                               "  L1_SIZE:                          %lu\n"
                               "  L1_ASSOC:                         %lu\n"
                               "  VC_NUM_BLOCKS:                    %lu\n"
                               "  L2_SIZE:                          %lu\n"
                               "  L2_ASSOC:                         %lu\n"
                               "  trace_file:                       %s\n"
                               "\n", params.block_size, params.l1_size, params.l1_assoc, params.vc_num_blocks, params.l2_size, params.l2_assoc, trace_file);




    char str[2];

    if(params.l2_size == 0 && params.vc_num_blocks == 0){
        cacheStructure1 = NOVICTIMCACHENOL2;
    }

    else if(params.l2_size != 0 && params.vc_num_blocks == 0){
        cacheStructure1 = L2NOVICTIMCACHE;
    }

    else if(params.l2_size == 0 && params.vc_num_blocks != 0){
        cacheStructure1 = VICTIMCACHEL1NOL2;
    }

    else if(params.l2_size != 0 && params.vc_num_blocks != 0){
        cacheStructure1 = VICTIMCACHEL1ANDL2;
    }

    Cache l1(params.l1_size, params.l1_assoc, params.block_size);
    VictimCache vc1(params.vc_num_blocks*params.block_size, params.vc_num_blocks, params.block_size);
    Cache l2(params.l2_size, params.l2_assoc, params.block_size);

    while(fscanf(FP, "%s %lx", str, &addr) != EOF)
    {
        cache_interface cacheInterface1, cacheInterface2;
        cache_interface victim_cache_interface;
        rw = str[0];
        if (rw == 'r'){
//            printf("%s %lx\n", "read", addr);           // Print and test if file is read correctly
            cacheInterface1 = l1.rwCache(addr, rw);
        }
        else if (rw == 'w'){
//            printf("%s %lx\n", "write", addr);          // Print and test if file is read correctly
            cacheInterface1 = l1.rwCache(addr, rw);
        }

        switch (cacheStructure1){
            case VICTIMCACHEL1NOL2: {
                if(cacheInterface1.evictionFlag) {
//                    cout << "Address Evicted from Cache: " << hex << cacheInterface1.evictedMemoryAddress << dec <<  endl;
                        victim_cache_interface = vc1.rwCache(addr, cacheInterface1.evictedMemoryAddress, cacheInterface1.dirtyBit);
                        l1.swapData(victim_cache_interface, rw);
                }
                break;
            }
            case L2NOVICTIMCACHE:
            {
                if((cacheInterface1.evictionFlag) && (cacheInterface1.dirtyBit)){
                    l2.rwCache(cacheInterface1.evictedMemoryAddress, 'w');
                }
                if(!cacheInterface1.cacheHit) {
                    l2.rwCache(addr, 'r');
                }
                break;
               }

            case VICTIMCACHEL1ANDL2:
            {

                if(cacheInterface1.evictionFlag) {
                    victim_cache_interface = vc1.rwCache(addr,cacheInterface1.evictedMemoryAddress, cacheInterface1.dirtyBit);
                    cacheInterface2.cacheHit = l1.swapData(victim_cache_interface, rw);
                }


                if((victim_cache_interface.evictionFlag) && (victim_cache_interface.dirtyBit)){
                    l2.rwCache(victim_cache_interface.evictedMemoryAddress, 'w');
                }


                if(!cacheInterface2.cacheHit && !cacheInterface1.cacheHit) {
                    l2.rwCache(addr, 'r');
                }

                break;
            }
            default: break;
        }

    }

    unsigned int totalReadsL1 = l1.getPerformanceParameters().cacheReadHits + l1.getPerformanceParameters().cacheReadMiss;
    unsigned int totalWritesL1 = l1.getPerformanceParameters().cacheWriteHits + l1.getPerformanceParameters().cacheWriteMiss;
    unsigned int totalRequests = totalReadsL1 + totalWritesL1;
    float srr = 0;
    float  mrL1VC = (l1.getPerformanceParameters().cacheReadMiss + l1.getPerformanceParameters().cacheWriteMiss)/ (float)totalRequests ;
    float mrL2 = 0;
    unsigned int wbL1VC = l1.getPerformanceParameters().writeBack;
    unsigned int totalReadsL2 = 0;
    unsigned int  totalWritesL2 = 0;
    unsigned int totalRequestsL2 = 0;
    unsigned int totalMemoryTraffic = l1.getPerformanceParameters().cacheReadMiss + l1.getPerformanceParameters().cacheWriteMiss + l1.getPerformanceParameters().writeBack;
    switch (cacheStructure1){
        case VICTIMCACHEL1NOL2: {
            srr = l1.getPerformanceParameters().evictions / (float)totalRequests;
            mrL1VC = (l1.getPerformanceParameters().cacheReadMiss + l1.getPerformanceParameters().cacheWriteMiss -  vc1.getPerformanceParameters().swaps)/ (float)totalRequests ;
            wbL1VC = vc1.getPerformanceParameters().writeBack;
            totalMemoryTraffic = totalMemoryTraffic - vc1.getPerformanceParameters().swaps - l1.getPerformanceParameters().writeBack + vc1.getPerformanceParameters().writeBack;
            break;
        }
        case L2NOVICTIMCACHE:
        {
            totalReadsL2 = l2.getPerformanceParameters().cacheReadHits + l2.getPerformanceParameters().cacheReadMiss;
            totalWritesL2 = l2.getPerformanceParameters().cacheWriteHits + l2.getPerformanceParameters().cacheWriteMiss;
            totalRequestsL2 = totalReadsL2 + totalWritesL2;
            mrL2 =  l2.getPerformanceParameters().cacheReadMiss / (float)totalReadsL2;
            totalMemoryTraffic = l2.getPerformanceParameters().cacheReadMiss + l2.getPerformanceParameters().cacheWriteMiss + l2.getPerformanceParameters().writeBack;
            break;
        }

        case VICTIMCACHEL1ANDL2:
        {
            srr = vc1.getPerformanceParameters().swapRequests / (float)totalRequests;
            wbL1VC = vc1.getPerformanceParameters().writeBack;
            mrL1VC = (l1.getPerformanceParameters().cacheReadMiss + l1.getPerformanceParameters().cacheWriteMiss -  vc1.getPerformanceParameters().swaps)/ (float)totalRequests ;
            totalReadsL2 = l2.getPerformanceParameters().cacheReadHits + l2.getPerformanceParameters().cacheReadMiss;
            totalWritesL2 = l2.getPerformanceParameters().cacheWriteHits + l2.getPerformanceParameters().cacheWriteMiss;
            totalRequestsL2 = totalReadsL2 + totalWritesL2;
            mrL2 =  l2.getPerformanceParameters().cacheReadMiss / (float)totalReadsL2;
            totalMemoryTraffic = l2.getPerformanceParameters().cacheReadMiss + l2.getPerformanceParameters().cacheWriteMiss + l2.getPerformanceParameters().writeBack;
            break;
        }
        default: break;
    }

    ofstream simulationResults;
    simulationResults.open("simulation.txt", ios::out);

  simulationResults << "===== Simulator configuration ====="
          "\n  BLOCKSIZE:                     "<< params.block_size <<
          "\n  L1_SIZE:                          "<< params.l1_size <<
          "\n  L1_ASSOC:                         "<<params.l1_assoc <<
          "\n  VC_NUM_BLOCKS:                    " <<params.vc_num_blocks <<
          "\n  L2_SIZE:                          " <<params.l2_size <<
          "\n  L2_ASSOC:                         " <<params.l2_assoc <<
          "\n  trace_file:                       " <<trace_file <<
          "\n" << endl;

    simulationResults << "===== L1 contents =====" << endl;
    l1.reorderTags();
    l1.displayTags(&simulationResults);
    simulationResults << endl;
    simulationResults << "===== VC contents =====" << endl;
    vc1.reorderTags();
    vc1.displayTags(&simulationResults);
    simulationResults << endl;
    simulationResults << "===== L2 contents =====" << endl;
    l2.reorderTags();
    l2.displayTags(&simulationResults);

    simulationResults << endl;
    simulationResults << "===== Simulation results =====" << endl;
    simulationResults << left << setw(42) << "  a. number of L1 reads:" << right << setw(10)<< totalReadsL1 << endl;
    simulationResults << left << setw(42) << "  b. number of L1 read misses:"<< right << setw(10) << l1.getPerformanceParameters().cacheReadMiss << endl;
    simulationResults << left << setw(42) << "  c. number of L1 writes:" << right << setw(10)<< totalWritesL1 << endl;
    simulationResults << left << setw(42) << "  d. number of L1 write misses:" << right << setw(10)<< l1.getPerformanceParameters().cacheWriteMiss << endl;
    simulationResults << left << setw(42) << "  e. number of swap requests:" << right << setw(10)<< vc1.getPerformanceParameters().swapRequests << endl;
    simulationResults<<fixed<<setprecision(4);
    simulationResults << left << setw(42) << "  f. swap request rate:" << right << setw(10)<< srr << endl;
    simulationResults << left << setw(42) << "  g. number of swaps:" << right << setw(10)<< vc1.getPerformanceParameters().swaps << endl;
    simulationResults << left << setw(42) << "  h. combined L1+VC miss rate:" << right << setw(10)<< mrL1VC << endl;
    simulationResults << left << setw(42) << "  i. number writebacks from L1/VC:" << right << setw(10)<< wbL1VC << endl;

    simulationResults << left << setw(42) << "  j. number of L2 reads:" << right << setw(10)<< totalReadsL2 << endl;
    simulationResults << left << setw(42) << "  k. number of L2 read misses:" << right << setw(10)<< l2.getPerformanceParameters().cacheReadMiss << endl;
    simulationResults << left << setw(42) << "  l. number of L2 writes:" << right << setw(10)<< totalWritesL2<< endl;
    simulationResults << left << setw(42) << "  m. number of L2 write misses:" << right << setw(10)<< l2.getPerformanceParameters().cacheWriteMiss << endl;

    simulationResults << left << setw(42) << "  n. L2 miss rate:"<< right << setw(10) << mrL2 << endl;
    simulationResults << left << setw(42) << "  o. number of writebacks from L2:" << right << setw(10)<< l2.getPerformanceParameters().writeBack << endl;
    simulationResults << left << setw(42) << "  p. total memory traffic:" << right << setw(10)<< totalMemoryTraffic << endl;

//    simulationResults << "Writing this to a file.\n";
    simulationResults.close();

    return 0;
}