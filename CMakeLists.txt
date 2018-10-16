cmake_minimum_required(VERSION 3.12)
project(ECE_563_Project)

set(CMAKE_CXX_STANDARD 14)

add_executable(ECE_563_Project sim_cache.cc Cache.cpp Cache.h BitsOffset.cpp BitsOffset.h PerformanceParameters.cpp PerformanceParameters.h CacheElements.cpp CacheElements.h VictimCache.cpp VictimCache.h)