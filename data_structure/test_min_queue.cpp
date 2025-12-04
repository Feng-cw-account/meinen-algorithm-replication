#include <iostream>
#include <random>
#include <cassert>
#include "min_queue.hpp"

int main() {
    MinQueue<int> q;
    assert(q.empty());

    q.push(3);
    q.push(1);
    q.push(2);

    assert(q.get_min() == 1);
    assert(q.front() == 3);

    q.pop(); // pop 3
    assert(q.front() == 1);
    assert(q.get_min() == 1);

    q.pop(); // pop 1
    assert(q.get_min() == 2);
    assert(q.front() == 2);

    q.pop();
    assert(q.empty());

    // test with repeated numbers
    q.push(5);
    q.push(5);
    q.push(3);
    q.push(5);
    assert(q.get_min() == 3);

    // test monotonic increasing
    while(!q.empty()) q.pop();
    for (int i = 0; i < 100; ++i) q.push(i);
    assert(q.get_min() == 0);
    for (int i = 0; i < 100; ++i) {
        assert(q.get_min() == i);
        q.pop();
    }

    // test random sequence against brute force
    std::mt19937 rng(123);
    std::uniform_int_distribution<int> dist(-100, 100);

    while (!q.empty()) ; // ensure empty

    std::vector<int> brute;
    for (int t = 0; t < 1000; ++t) {
        int x = dist(rng);
        q.push(x);
        brute.push_back(x);
        if (q.get_min() != *std::min_element(brute.begin(), brute.end())) {
            std::cerr << "Mismatch after push at t=" << t << "\n";
            return 2;
        }
        if ((t & 7) == 0) { // occasionally pop
            int front_v = q.front();
            q.pop();
            brute.erase(brute.begin());
            if (!brute.empty()) {
                if (q.get_min() != *std::min_element(brute.begin(), brute.end())) {
                    std::cerr << "Mismatch after pop at t=" << t << "\n";
                    return 3;
                }
            }
        }
    }

    std::cout << "MinQueue tests passed\n";
    return 0;
}
