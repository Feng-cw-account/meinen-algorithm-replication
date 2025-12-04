#include <iostream>
#include <vector>
#include <numeric> // For std::iota
#include "fenwick_tree.cpp"

using namespace std;

int main() {
    // Test Case 1: Basic functionality with int
    cout << "--- Test Case 1: Basic functionality with int ---" << endl;
    int n_int = 10;
    FenwickTree<int> bit_int(n_int);

    // Simulate array: [0, 1, 2, 3, 4, 5, 0, 0, 0, 0]
    for (int i = 1; i <= 5; i++) {
        bit_int.add(i, i);
    }

    cout << "Prefix sum up to 5: " << bit_int.query(5) << endl; // Expected: 1+2+3+4+5 = 15
    cout << "Range sum [2, 4]: " << bit_int.queryRange(2, 4) << endl; // Expected: 2+3+4 = 9

    // Modify: Add 10 to the 3rd element (value becomes 3 + 10 = 13)
    bit_int.add(3, 10);
    cout << "Prefix sum up to 5 after modification: " << bit_int.query(5) << endl; // Expected: 15 + 10 = 25
    cout << "Range sum [2, 4] after modification: " << bit_int.queryRange(2, 4) << endl; // Expected: (2+13+4) = 19
    // Validate get for index 4
    cout << "Value at index 4 via get: " << bit_int.get(4) << endl; // expected 4
    if (bit_int.get(4) != 4) {
        cerr << "get(4) mismatch: " << bit_int.get(4) << " expected 4\n";
        return 6;
    }

    // Test Case 2: Initialization with raw vector
    cout << "\n--- Test Case 2: Initialization with raw vector ---" << endl;
    vec<long long> initial_data = {10, 20, 30, 40, 50};
    FenwickTree<long long> bit_long_raw(initial_data);

    cout << "Prefix sum up to 3 (raw): " << bit_long_raw.query(3) << endl; // Expected: 10+20+30 = 60
    cout << "Range sum [2, 4] (raw): " << bit_long_raw.queryRange(2, 4) << endl; // Expected: 20+30+40 = 90

    bit_long_raw.add(1, 5); // Add 5 to the first element (10 -> 15)
    cout << "Prefix sum up to 3 after modification (raw): " << bit_long_raw.query(3) << endl; // Expected: 15+20+30 = 65
    // Validate `get` method for index 4 (value should remain 40)
    cout << "Value at index 4 (raw) via get: " << bit_long_raw.get(4) << endl;
    if (bit_long_raw.get(4) != 40) {
        cerr << "get(4) mismatch: " << bit_long_raw.get(4) << " expected 40\n";
        return 5;
    }

    // Test Case 3: Empty tree and edge cases
    cout << "\n--- Test Case 3: Empty tree and edge cases ---" << endl;
    FenwickTree<int> empty_bit(0);
    cout << "Query on empty tree (0): " << empty_bit.query(0) << endl; // Expected: 0
    cout << "Range query on empty tree [0,0]: " << empty_bit.queryRange(0,0) << endl; // Expected: 0

    // Test Case 4: Larger numbers with long long
    cout << "\n--- Test Case 4: Larger numbers with long long ---" << endl;
    int n_long = 1000;
    FenwickTree<long long> bit_large(n_long);
    for (int i = 1; i <= n_long; ++i) {
        bit_large.add(i, 1000LL * i);
    }
    cout << "Prefix sum up to 1000: " << bit_large.query(1000) << endl; // Expected: sum of 1000*i from 1 to 1000
    // (1000 * 1000 * 1001 / 2) = 500500000

    bit_large.add(500, 1000000LL); // Add a large value
    cout << "Prefix sum up to 500 after modification: " << bit_large.query(500) << endl;

    return 0;
}