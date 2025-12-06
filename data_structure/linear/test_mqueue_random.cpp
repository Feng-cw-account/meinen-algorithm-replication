#include <iostream>
#include <random>
#include <deque>
#include <cassert>
#include "mqueue.cpp"

int main() {
    using namespace std;
    MQueue<int> q;
    deque<int> ref;
    std::mt19937 rng(12345);
    std::uniform_int_distribution<int> op(0, 2); // 0 add, 1 pop, 2 query
    std::uniform_int_distribution<int> val(-1000, 1000);

    const int N = 5000;
    for (int i = 0; i < N; ++i) {
        int o = op(rng);
        if (o == 0) { // add
            int v = val(rng);
            q.add(v);
            ref.push_back(v);
        } else if (o == 1) { // pop
            if (!ref.empty()) {
                q.pop();
                ref.pop_front();
            } else {
                // ensure that pop on empty throws
                bool threw = false;
                try { q.pop(); } catch(...) { threw = true; }
                assert(threw);
            }
        } else { // query
            if (!ref.empty()) {
                assert(q.head() == ref.front());
                // compute min in ref
                int minv = ref.front();
                for (auto &x : ref) minv = std::min(minv, x);
                assert(q.min() == minv);
            } else {
                bool threw = false;
                try { q.head(); } catch(...) { threw = true; }
                assert(threw);
            }
        }
    }

    cout << "Randomized MQueue test passed" << endl;
    return 0;
}
