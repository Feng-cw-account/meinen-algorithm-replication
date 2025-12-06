#include <iostream>
#include <cassert>
#include <limits>
#include "segment_tree/basic.cpp"

int main() {
    using namespace std;
    // Test 1: Default sum merge
    vec<int> a = {1,2,3,4,5};
    SegmentTree<int> seg(a);
    seg.update(2, [](const int &old){ return old + 5; }); // a[2] becomes 8
    assert(seg.get(2) == 8);
    assert(seg.query(0,4) == 1+2+8+4+5);

    // Test 2: max merge using std::function
    vec<int> b = {5,1,3,2,4};
    SegmentTree<int> segmax(b, [](const int &x, const int &y){ return x > y ? x : y; }, numeric_limits<int>::min());
    assert(segmax.query(0,4) == 5);
    segmax.update(4, [](const int &old){ return 10; }); // set a[4] = 10
    assert(segmax.get(4) == 10);
    assert(segmax.query(0,4) == 10);

    cout << "SegmentTree update tests passed" << endl;
    return 0;
}
