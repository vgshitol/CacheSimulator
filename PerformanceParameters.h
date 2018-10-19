//
// Created by Vishal Shitole on 10/1/18.
//

#ifndef ECE_563_PROJECT_PERFORMANCEPARAMETERS_H
#define ECE_563_PROJECT_PERFORMANCEPARAMETERS_H


class PerformanceParameters {

public:
      unsigned int cacheMiss = 0;
      unsigned int cacheHits = 0;
      unsigned int cacheReadMiss = 0;
      unsigned int cacheReadHits = 0;
      unsigned int cacheWriteMiss = 0;
      unsigned int cacheWriteHits = 0;
      unsigned int swapRequests = 0;
      unsigned int swaps = 0;
      unsigned int evictions = 0;
        unsigned int writeBack = 0;
};


#endif //ECE_563_PROJECT_PERFORMANCEPARAMETERS_H
