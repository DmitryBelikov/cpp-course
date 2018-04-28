#include <vector>
#include <cassert>
#include <iostream>
#include "list.h"

using std::cout;
using std::endl;

int main() {
    list<int> l;
    l.push_back(1);
    l.push_back(2);
    l.push_back(3);
    list<int>::const_iterator it = l.begin();
    *it = 5;
    for(auto it = l.begin(); it != l.end(); it++)
        cout << *it;
    return 0;
}
