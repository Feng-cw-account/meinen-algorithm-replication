#include <iostream>
#include <cassert>
#include <deque>
#include <random>
#include "linear/mqueue_single_queue.cpp"

int main() {
    using namespace std;
    MQueue2<int> q;
    deque<int> ref;

    // Basic test
    bool threw=false;
    try { q.head(); } catch(...) { threw = true; }
    assert(threw);

    q.add(3);
    assert(q.head() == 3);
    assert(q.min() == 3);
    q.add(1);
    q.add(2);
    assert(q.head() == 3);
    assert(q.min() == 1);
    q.pop();
    assert(q.head() == 1);
    assert(q.min() == 1);
    q.pop();
    assert(q.head() == 2);
    assert(q.min() == 2);
    q.pop();

    // randomized test
    MQueue2<int> q2;
    deque<int> ref2;
    mt19937 rng(123);
    uniform_int_distribution<int> op(0, 2);
    uniform_int_distribution<int> val(-1000, 1000);
    for (int i = 0; i < 5000; ++i) {
        int o = op(rng);
        if (o == 0) { // add
            int v = val(rng);
            q2.add(v);
            ref2.push_back(v);
        } else if (o == 1) { // pop
            if (!ref2.empty()) { q2.pop(); ref2.pop_front(); }
            else { try { q2.pop(); } catch(...) { } }
        } else { // query
            if (!ref2.empty()) {
                assert(q2.head() == ref2.front());
                int mv = ref2.front(); for (int x : ref2) mv = min(mv, x);
                assert(q2.min() == mv);
            } else { try { q2.head(); } catch(...) { } }
        }
    }

    cout << "MQueue2 single-queue tests passed" << endl;
    return 0;
}
