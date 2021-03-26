#include "address.h"

int main()
{
    initMMU(1024*4,4,1024 * 4,2,32);

    Vector<int> vector1(1024 * 10,0x10f92160);
    Vector<int> vector2(1024 * 10,vector1.end_address());

    int total = 0;
    for(int i = 0; i < 10000; i++)
    {
        total = vector1[i] + vector2[i];
    }

    mmu->performance_metrics();
}
