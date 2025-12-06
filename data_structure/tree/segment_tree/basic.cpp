#include <vector>
#include <functional>
#include <stdexcept>
template<typename T>
using vec = std::vector<T>;

template <typename T>
class SegmentTree {
private:
    vec<T> t;         // tree array (size = 2*base)
    int n;            // number of leaves (original array size)
    int base;         // power-of-two base
    T identity;
    std::function<T(const T&, const T&)> merge;

    static int next_power_of_two(int x) {
        int p = 1;
        while (p < x) p <<= 1;
        return p;
    }

public:
    SegmentTree() = default;
    // default: sum merge and identity T{}
    SegmentTree(const vec<T>& raw,
                std::function<T(const T&, const T&)> mergeFn = [](const T& a, const T& b){ return a + b; },
                T id = T{}) : n(static_cast<int>(raw.size())), identity(id), merge(mergeFn) {
        base = next_power_of_two(n == 0 ? 1 : n);
        t.assign(base << 1, identity);
        for (int i = 0; i < n; ++i) t[base + i] = raw[i];
        for (int i = base - 1; i >= 1; --i) t[i] = merge(t[i << 1], t[i << 1 | 1]);
    }

    // set value at index (0-based)
    void set(int idx, const T& value) {
        if (idx < 0 || idx >= n) throw std::out_of_range("index out of range");
        int i = base + idx;
        t[i] = value;
        for (i >>= 1; i >= 1; i >>= 1) t[i] = merge(t[i << 1], t[i << 1 | 1]);
    }

    // apply function to a single element
    void update(int idx, const std::function<T(const T&)>& f) {
        if (idx < 0 || idx >= n) throw std::out_of_range("index out of range");
        int i = base + idx;
        t[i] = f(t[i]);
        for (i >>= 1; i >= 1; i >>= 1) t[i] = merge(t[i << 1], t[i << 1 | 1]);
    }

    // add (convenience) -- uses operator+
    void add(int idx, const T& delta) {
        update(idx, [delta](const T& old){ return old + delta; });
    }

    // get value at index
    T get(int idx) const {
        if (idx < 0 || idx >= n) throw std::out_of_range("index out of range");
        return t[base + idx];
    }

    // query [l, r] inclusive
    T query(int l, int r) const {
        if (l > r) return identity;
        if (l < 0) l = 0;
        if (r >= n) r = n - 1;
        int L = l + base;
        int R = r + base;
        T resl = identity, resr = identity;
        while (L <= R) {
            if (L & 1) resl = merge(resl, t[L++]);
            if (!(R & 1)) resr = merge(t[R--], resr);
            L >>= 1; R >>= 1;
        }
        return merge(resl, resr);
    }
};
