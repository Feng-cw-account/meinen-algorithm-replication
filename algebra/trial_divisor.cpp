#include <vector>
#include <utility>
#include <cmath>

using p = std::pair<int, int>;
using vec_p = std::vector<p>;

vec_p find_all_prime_divisor(int n) {

    auto result = vec_p();

    // for trivial case:
    if (n <= 1) return result;
    
    // for normal cases:

    //**note**: $base * base \le  n$ ensure $2\times 2$ included
    //**note**: we can ensure for most 1 factor bigger then $n^{\frac{1}{2}}$ will be left over.
    //**note**: for small to big range make sure all factor be **prime**
    // i.e. there is a non-prime factor 4, then we know that it can be
    // break down into **smaller factor** that we have checked. 
    // so **None of non-prime** included.
    for(int base = 2; base <= n / base; base++){
        if (n % base == 0) {
            int exponent = 0;
            while (n % base == 0) {
                exponent++;
                n = n / base;
            }

            result.push_back(p(base, exponent));
        }
    }

    // handel left over factor.
    if (n != 1) {
        result.push_back(p(n, 1));
    }
	return result;
}