#include <iostream>
#include <cassert>
#include "segment_tree/basic.cpp"

int main(){
    using namespace std;
    vec<int> a = {1,2,3,4,5};
    SegmentTree<int> seg(a);
    assert(seg.query(0,0) == 1);
    assert(seg.query(0,1) == 3);
    assert(seg.query(1,3) == 9);
    assert(seg.query(0,4) == 15);

    seg.set(2, 10); // set a[2]=10
    assert(seg.get(2) == 10);
    assert(seg.query(0,4) == 22);

    seg.add(2, 5); // a[2]+=5 -> 15
    assert(seg.get(2) == 15);
    assert(seg.query(0,4) == 27);

    cout << "SegmentTree tests passed" << endl;
    return 0;
}
