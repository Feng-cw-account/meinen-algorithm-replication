#include <iostream>
#include <cassert>
#include "mqueue.cpp"

int main() {
    using namespace std;
    MQueue<int> q;
    // empty queue head()/min()/pop() should throw
    bool threw = false;
    try { q.head(); } catch(...) { threw = true; }
    assert(threw);
    threw = false;
    try { q.min(); } catch(...) { threw = true; }
    assert(threw);

    q.add(3);
    assert(q.head() == 3);
    assert(q.min() == 3);

    q.add(1);
    q.add(2);
    // queue now: 3,1,2
    assert(q.head() == 3);
    assert(q.min() == 1);

    q.pop(); // now 1,2
    assert(q.head() == 1);
    assert(q.min() == 1);

    q.pop(); // now 2
    assert(q.head() == 2);
    assert(q.min() == 2);

    q.pop(); // now empty
    threw = false; try { q.head(); } catch(...) { threw = true; }
    assert(threw);

    cout << "All MQueue tests passed" << endl;
    return 0;
}
