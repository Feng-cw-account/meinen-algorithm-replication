#include <vector>
#include <stdexcept>

template <typename T>
using vec = std::vector<T>;

template <typename T>
class Fenwick {
private:
    vec<T> self;
    int n = 0;
    int lowbit(int x) {
        return x & -x;
    }

public:

    Fenwick() = default;
    Fenwick(vec<T> f) {
        n = static_cast<int>(f.size());
        self.assign(n + 1, T{});
        for (int i = 1; i <= n; ++i) self[i] = f[i - 1];
        // build BIT in O(n)
        for (int i = 1; i <= n; ++i) {
            int j = i + lowbit(i);
            if (j <= n) self[j] += self[i];
        }
    }

    
};