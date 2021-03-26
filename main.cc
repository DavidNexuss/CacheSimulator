#include "cache.h"
#include <iostream>
#include <vector>
#include <list>
using namespace std;
bool silent;

void simulate(Cache* cache)
{
    int n;
    while(cin >> n)
    {
        if (!silent)
        {
            cerr << "Querying for block " << n << endl;
            cerr << "Cache hit:";
            cout << " " << cache->lookup(n);
            cerr << endl;
            cache->print();
        } else cache->lookup(n);
    }

    cache->performance_metrics();
}
int main(int argc,char** argv)
{
    silent = argc > 1 && string(argv[1]) == "-s";

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
