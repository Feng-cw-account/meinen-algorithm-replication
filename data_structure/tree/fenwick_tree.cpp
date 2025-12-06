#include <vector>
#include <stdexcept>

template <typename T>
using vec = std::vector<T>;

template <typename T>
class FenwickTree {
private:
    vec<T> tree; // Internal array for Fenwick tree, 1-indexed
    int n;       // Size of the array being represented

    // Helper function: returns the lowest set bit in x
    inline int lowbit(int x) {
        return x & (-x);
    }

public:
    // Cons5tructor 1: Initializes an empty Fenwick tree of a given size
    FenwickTree(int size) : n(size) {
        tree.assign(n + 1, T{}); // Initialize with default value of T (e.g., 0 for numeric types)
    }

    // Constructor 2: Initializes Fenwick tree from a raw 0-indexed array
    FenwickTree(const vec<T>& raw) : n(raw.size()) {
        tree.assign(n + 1, T{}); // Initialize with default value
        for (int i = 0; i < n; ++i) {
            add(i + 1, raw[i]); // Fenwick tree is 1-indexed, add elements one by one
        }
    }

    // Adds 'delta' to the element at index 'i' (1-indexed)
    // Time complexity: O(log n)
    void add(int i, T delta) {
        while (i <= n) {
            tree[i] += delta;
            i += lowbit(i); // Move to the next parent node
        }
    }

    // Returns the value at index i (1-indexed). O(log n)
    // For the point-update, prefix-sum variant of Fenwick tree, the
    // original array value is query(i) - query(i-1).
    T get(int i) {
        if (i < 1 || i > n) throw std::out_of_range("FenwickTree::get: index out of range");
        return query(i) - query(i - 1);
    }

    // Queries the prefix sum from index 1 to 'i' (1-indexed)
    // Time complexity: O(log n)
    T query(int i) {
        T sum = T{}; // Initialize sum with default value of T
        while (i > 0) {
            sum += tree[i];
            i -= lowbit(i); // Move to the previous relevant segment
        }
        return sum;
    }

    // Queries the sum of elements in the range [left, right] (1-indexed)
    // Time complexity: O(log n)
    T queryRange(int left, int right) {
        return query(right) - query(left - 1);
    }
};
