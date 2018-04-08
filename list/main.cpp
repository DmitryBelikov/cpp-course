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
    const list<int> r = l;
    return 0;
}
