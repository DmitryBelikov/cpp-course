#include <iostream>
//#include "persistent_set.h"
#include "linked_ptr.h"
//#include "shared_ptr.h";

struct A
{
    int x;
    void print()
    {
        std::cout << "HI" << std::endl;
    }
};

int main()
{
    //int x = 5;

    linked_ptr<int> a = new int(5);
    std::cout << *a;

    linked_ptr<A> b = new A;
    b->print();

    return 0;
}

