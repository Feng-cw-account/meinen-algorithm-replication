#include <vector>
#include <functional>
#include <stdexcept>
template<typename T>
using vec = std::vector<T>;

// CompleteTree wraps an array-based complete binary tree container.
// We'll use a power-of-two base for leaf indices (1-based tree array at indices [1..2*base))
template <typename T>
class CompleteTree {
private:
    vec<T> self; // 1-indexed array (we will use size = 2*base)
    int base = 0; // number of leaves (power of two)

public:
    CompleteTree() = default;

    explicit CompleteTree(int n) {
        // compute next power of two >= n
        base = 1;
        while (base < n) base <<= 1;
        self.assign(base << 1, T{}); // indices 0..2*base-1, we will ignore index 0
    }

    // Build from raw vector (0-based), filling leaves at [base .. base+n-1]
    CompleteTree(const vec<T>& raw) {
        int n = static_cast<int>(raw.size());
        base = 1;
        while (base < n) base <<= 1;
        self.assign(base << 1, T{});
        for (int i = 0; i < n; ++i) self[base + i] = raw[i];
    }

    int leafBase() const { return base; }
    int size() const { return static_cast<int>(self.size()); }

    // access by index
    T& operator[](int idx) { return self[idx]; }
    const T& operator[](int idx) const { return self[idx]; }
};

template <typename T>
class SegmentTree {
private:
    CompleteTree<T> tree;
    std::function<T(const T&, const T&)> merge;
    T identity{};
    int nLeaves = 0; // original input size

public:
    // ctor using merge function (default sum) and identity element for merge
    SegmentTree(const vec<T>& raw,
                std::function<T(const T&, const T&)> mergeFn = [](const T& a, const T& b){ return a + b; },
                T id = T{}) : merge(mergeFn), identity(id) {
        nLeaves = static_cast<int>(raw.size());
        tree = CompleteTree<T>(raw);
        build();
    }

    void build() {
        int base = tree.leafBase();
        // build internal nodes from base-1 down to 1
        for (int i = base - 1; i >= 1; --i) {
            tree[i] = merge(tree[i << 1], tree[i << 1 | 1]);
        }
    }

    // point update: set index (0-based) to value
    void set(int idx0, T value) {
        if (idx0 < 0 || idx0 >= nLeaves) throw std::out_of_range("index out of range");
        int i = tree.leafBase() + idx0;
        tree[i] = value;
        for (i >>= 1; i >= 1; i >>= 1) tree[i] = merge(tree[i << 1], tree[i << 1 | 1]);
    }

    // point add: add delta to index (0-based) -- requires operator+
    void add(int idx0, T delta) {
        if (idx0 < 0 || idx0 >= nLeaves) throw std::out_of_range("index out of range");
        // implement add in terms of update to keep a single point-update path
        update(idx0, [delta](const T& old) { return old + delta; });
    }

    // generic point update: apply updater(old_value) -> new_value, supporting any merge func
    void update(int idx0, std::function<T(const T&)> updater) {
        if (idx0 < 0 || idx0 >= nLeaves) throw std::out_of_range("index out of range");
        int i = tree.leafBase() + idx0;
        tree[i] = updater(tree[i]);
        for (i >>= 1; i >= 1; i >>= 1) tree[i] = merge(tree[i << 1], tree[i << 1 | 1]);
    }

    // query [l, r], inclusive, 0-based
    T query(int l0, int r0) const {
        if (l0 > r0) return identity;
        if (l0 < 0) l0 = 0;
        if (r0 >= nLeaves) r0 = nLeaves - 1;
        int base = tree.leafBase();
        int l = base + l0;
        int r = base + r0;
        T resl = identity, resr = identity;
        while (l <= r) {
            if (l & 1) resl = merge(resl, tree[l++]);
            if (!(r & 1)) resr = merge(tree[r--], resr);
            l >>= 1; r >>= 1;
        }
        return merge(resl, resr);
    }

    // get single value at idx (0-based)
    T get(int idx0) const {
        if (idx0 < 0 || idx0 >= nLeaves) throw std::out_of_range("index out of range");
        return tree[tree.leafBase() + idx0];
    }
};
