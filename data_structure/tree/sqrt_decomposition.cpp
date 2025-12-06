#include <vector>
#include <cmath>
#include <stdexcept>
#include <algorithm>

template <typename T>
using vec = std::vector<T>;

template <typename T>
class SqrtDecomposition {
private:
int num_raw;
int block_size;
int num_block;
vec<T> raw;
vec<T> blocks;

// utils:
    // compute block size (B), default floor(sqrt(n)), at least 1
    int compute_block_size(int n) const {
        if (n <= 0) return 1;
        int b = static_cast<int>(std::floor(std::sqrt(static_cast<double>(n))));
        return (b > 0) ? b : 1;
    }

    // integer ceil division a/b
    int ceilingDivision(int a, int b) const {
        if (b <= 0) throw std::invalid_argument("ceilingDivision: b must be positive");
        return (a + (b - 1)) / b;
    }

public:

    SqrtDecomposition(const vec<T>& arr) {
        raw = arr;
        num_raw = static_cast<int>(raw.size());
        block_size = compute_block_size(num_raw);
        num_block = ceilingDivision(num_raw, block_size);
        blocks.assign(num_block, T{});
        // compute block sums
        for (int i = 0; i < num_raw; ++i) {
            blocks[i / block_size] += raw[i];
        }
    }

    // convenience constructor - size n with default-initialized elements
    SqrtDecomposition(int n = 0) {
        num_raw = n;
        block_size = compute_block_size(num_raw);
        num_block = ceilingDivision(num_raw, block_size);
        raw.assign(num_raw, T{});
        blocks.assign(num_block, T{});
    }

    // Return number of elements
    int size() const { return num_raw; }

    // point add: add delta at index idx (0-indexed)
    void add(int idx, T delta) {
        if (idx < 0 || idx >= num_raw) throw std::out_of_range("SqrtDecomposition::add: index out of range");
        raw[idx] += delta;
        blocks[idx / block_size] += delta;
    }

    // point set: assign new value to index idx (0-indexed)
    void set(int idx, T value) {
        if (idx < 0 || idx >= num_raw) throw std::out_of_range("SqrtDecomposition::set: index out of range");
        T diff = value - raw[idx];
        raw[idx] = value;
        blocks[idx / block_size] += diff;
    }

    // range sum query [l, r] inclusive (0-indexed)
    T query(int l, int r) const {
        if (l < 0 || r < 0 || l >= num_raw || r >= num_raw || l > r) throw std::out_of_range("SqrtDecomposition::query: invalid range");
        T res = T{};
        int lb = l / block_size;
        int rb = r / block_size;
        if (lb == rb) {
            for (int i = l; i <= r; ++i) res += raw[i];
            return res;
        }
        int endLb = (lb + 1) * block_size - 1;
        for (int i = l; i <= std::min(endLb, num_raw - 1); ++i) res += raw[i];
        for (int b = lb + 1; b <= rb - 1; ++b) res += blocks[b];
        for (int i = rb * block_size; i <= r; ++i) res += raw[i];
        return res;
    }
    
};