#include <iostream>
#include <vector>
#include <list>
using namespace std;

class Cache
{
    int hitCount = 0;
    int lookupCount = 0;

    public:

    virtual bool i_lookup(int block) = 0;
    virtual void print() = 0;

    bool lookup(int block)
    {
        lookupCount++;
        bool hit = i_lookup(block);
        hitCount += hit;
        return hit;
    }

    int getHitCount() const { return hitCount; }
    int getLookupCount() const { return lookupCount; }
};
class NWayCache : public Cache
{
    vector<list<int>> cache;
    int size;
    int ways;

    public:
    NWayCache(int _size,int _ways) : size(_size),ways(_ways),cache(_size) { }

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

    virtual void print() override
    {
        cerr << "=================================" << endl;
        for(int i = 0; i < size; i++)
        {
            cerr << i << ": ";
            for(int b : cache[i]) cerr << b << " | ";
            cerr << endl;
        }
        cerr << "=================================" << endl;
        cerr << endl;
    }
};

class VictimCache : public Cache
{
    vector<int> cache;
    list<int> victim;

    int size;
    int victim_size;

    public:
    VictimCache(int _size,int _victim_size) : size(_size), victim_size(_victim_size), cache(_size,-1) { }

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

    virtual void print() override
    {
        cerr << "=================================" << endl;
        for(int i = 0; i < size; ++i)
        {
            cerr << i << ": " << ((cache[i] != -1) ? to_string(cache[i]) : " ") << endl;
        }
        cerr << "Victim cache: ";
        for(auto b : victim) cerr << b << " | ";
        cerr << endl;
        cerr << "=================================" << endl;
        cerr << endl;
    }
};

void simulate(Cache* cache)
{
    int n;
    while(cin >> n)
    {
        cerr << "Querying for block " << n << endl;
        cerr << "Cache hit:";
        cout << " " << cache->lookup(n);
        cerr << endl;
        cache->print();
    }

    cerr << "===[Perfomance metrics]==========" << endl;
    int lookupCount = cache->getLookupCount();
    int hitCount = cache->getHitCount();

    cerr << "LookupCount: " << lookupCount << endl;
    cerr << "HitCount: " << hitCount << endl;

    cerr << "Perfomance: " << hitCount / double(lookupCount) << endl;
}
int main()
{
    int type;
    cerr << "Choose following cache types: " << endl;
    cerr << "0) NWayCache " << endl;
    cerr << "1) VictimCache " << endl;

    cin >> type;
    cerr << "Give size and ways/victim slots" << endl;
    int size,ways;
    cin >> size >> ways;
    switch(type)
    {
        case 0: simulate(new NWayCache(size,ways)); break;
        case 1: simulate(new VictimCache(size,ways)); break;
        default:
        cerr << "Invalid option" << endl;
        return 1;
    }
}
