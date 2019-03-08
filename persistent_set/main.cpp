#include <iostream>
#include "persistent_set.h"
#include "linked_ptr.h"
#include <vector>

using std::cout;

int main()
{
    persistent_set<int> ps1;
    ps1.insert(5);
    persistent_set<int> ps2 = ps1;
    ps1.insert(6);
    persistent_set<int> ps3 = ps1;
    ps2.insert(7);
    persistent_set<int> ps4 = ps3;
    ps4.insert(8);
    for(auto it = ps1.begin(); it != ps1.end(); it++) {
        cout << *it << " ";
    }
    cout << "\n";
    for(auto it = ps2.begin(); it != ps2.end(); it++) {
        cout << *it << " ";
    }
    cout << "\n";
    for(auto it = ps3.begin(); it != ps3.end(); it++) {
        cout << *it << " ";
    }
    cout << "\n";
    for(auto it = ps4.begin(); it != ps4.end(); it++) {
        cout << *it << " ";
    }
    cout << "\n";
    std::vector<persistent_set<int>> v = {persistent_set<int>()};
    for(int i = 1; i < 100; i++) {
        int x = rand();
        v.push_back(v[i - 1]);
        v[i].insert(x);
    }
    for(auto ps : v) {
        for(persistent_set<int>::iterator it = ps.begin(); it != ps.end(); it++) {
            std::cout << *it << " ";
        }
        std::cout << "\n";
    }
    return 0;
}

