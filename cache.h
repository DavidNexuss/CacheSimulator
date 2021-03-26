#include <iostream>
#include <vector>
#include <list>
using namespace std;

class Cache
{
    int hitCount = 0;
    int lookupCount = 0;

    virtual bool i_lookup(int block) = 0;

    public:

    bool lookup(int block)
    {
        lookupCount++;
        bool hit = i_lookup(block);
        hitCount += hit;
        return hit;
    }

    int getHitCount() const { return hitCount; }
    int getLookupCount() const { return lookupCount; }
    
    virtual void i_print() = 0;
    void print()
    {
        cerr << "=================================" << endl;
        i_print();
        cerr << "=================================" << endl;
        cerr << endl;
    }

    void performance_metrics()
    {
        cerr << "===[Perfomance metrics]==========" << endl;
        int lookupCount = getLookupCount();
        int hitCount = getHitCount();

        cerr << "LookupCount: " << lookupCount << endl;
        cerr << "HitCount: " << hitCount << endl;

        cerr << "Perfomance: " << hitCount / double(lookupCount) << endl;
    }
};
class NWayCache : public Cache
{
    vector<list<int>> cache;
    int size;
    int ways;

    void insert(int block)
    {
        if(cache[block % size].size() == ways) cache[block % size].pop_back();
        cache[block % size].push_front(block);
    }

    virtual bool i_lookup(int block) override
    {
        int idx = block % size;
        for(auto it = cache[idx].begin(); it != cache[idx].end(); ++it)
        {
            if (*it == block)
            {
                cache[idx].erase(it);
                insert(block);
                return true;
            }
        }
        insert(block);
        return false;
    }

    public:
    NWayCache(int _size,int _ways) : size(_size),ways(_ways),cache(_size) { }

    virtual void i_print() override
    {
        for(int i = 0; i < size; i++)
        {
            cerr << i << ": ";
            for(int b : cache[i]) cerr << b << " | ";
            cerr << endl;
        }
    }
};

class VictimCache : public Cache
{
    vector<int> cache;
    list<int> victim;

    int size;
    int victim_size;


    void push_victim(int block)
    {
        if(victim.size() == victim_size) victim.pop_back();
        victim.push_front(cache[block % size]);
        cache[block % size] = block;
    }
    virtual bool i_lookup(int block) override
    {
        if (cache[block % size] == -1)
        {
            cache[block % size] = block;
            return false;
        }
        if(cache[block % size] == block) return true;
        for(auto it = victim.begin(); it != victim.end(); ++it)
        {
            if (*it == block)
            {
                victim.erase(it);
                push_victim(block);
                return true;
            }
        }
        push_victim(block);
        return false;
    }

    public:
    VictimCache(int _size,int _victim_size) : size(_size), victim_size(_victim_size), cache(_size,-1) { }

    virtual void i_print() override
    {
        for(int i = 0; i < size; ++i)
        {
            cerr << i << ": " << ((cache[i] != -1) ? to_string(cache[i]) : " ") << endl;
        }
        cerr << "Victim cache: ";
        for(auto b : victim) cerr << b << " | ";
    }
};
