#include <iostream>
#include <vector>
#include "cache.h"
using namespace std;

struct MMU
{

    Cache *cache;
    size_t frameSize;
    NWayCache tlbCache;

    size_t blockSize;
    size_t hit;
    size_t total;

    MMU(size_t _frameSize,size_t tlbWays,size_t _blockSize,Cache * _cache) : cache(_cache), frameSize(_frameSize), blockSize(_blockSize), tlbCache(1,tlbWays)
    {
        hit = 0;
        total = 0;
    }

    bool access(size_t address)
    {
        size_t frameAddress = address / frameSize;
        bool tlbFound = tlbCache.lookup(frameAddress);

//size_t lowerBound = address % frameSize;
        size_t lowerBound = address;
        bool cacheFound = cache->lookup(lowerBound / blockSize);


        hit += cacheFound && tlbFound;
        total += 1;

        return cacheFound && tlbFound;
    }
    void performance_metrics()
    {
        cerr << "====[Cache performance]=====" << endl;
        cache->print();
        cache->performance_metrics();
        cerr << "====[TLB performance]=====" << endl;
        tlbCache.print();
        tlbCache.performance_metrics();

        cerr << "====[MMU performance]=====" << endl;
        cerr << "Total: " << total << endl;
        cerr << "Hits: " << hit << endl;
        cerr << "Miss: " << total - hit << endl;
        cerr << "Perfomance: " << double(hit)/double(total) << endl;
    }

    ~MMU()
    {
        delete cache;
    }
};
MMU* mmu;

void initMMU(size_t frameSize,size_t tlbWays,size_t cacheSize,size_t cacheSets,size_t blockSize)
{
    mmu = new MMU(frameSize,tlbWays,blockSize,new NWayCache(cacheSize / (cacheSets * blockSize),cacheSets));
}

template <typename T>
struct Vector : public vector<T>
{
    size_t base_address = -1;
    Vector(size_t size,size_t _base_address = -1) : vector<T>(size) 
    {
        if (_base_address != -1)
        {
            base_address = _base_address;
        }
    }
    T& operator[](size_t i)
    {
        T& object = vector<T>::at(i);
        size_t address;
        if (base_address == -1) address = size_t(&object);
        else address = ((i * sizeof(T)) + base_address);

        mmu->access(address);
        return object;
    }

    size_t end_address()
    {
        return base_address + vector<T>::size() * sizeof(T);
    }

    ~Vector()
    {
        cerr << "Used sizeof " << sizeof(T) << " of a total elements of " << vector<T>::size() << endl;
    }
};
