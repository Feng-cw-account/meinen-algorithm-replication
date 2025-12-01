#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

typedef long long ll;

ll K;
ll ans = 1e18;
vector<ll> primes;

bool is_prime(ll n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (ll i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

void get_divisors(ll n, vector<ll>& divs) {
    for (ll i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            divs.push_back(i);
            if (i * i != n) {
                divs.push_back(n / i);
            }
        }
    }
}

// Generate primes up to a reasonable limit
void generate_primes(vector<ll>& primes) {
    for (ll i = 2; i <= 100000; ++i) {
        if (is_prime(i)) {
            primes.push_back(i);
        }
    }
}

// DFS to find all factorizations of K
// exponents: list of (exponent+1) values
// idx: current index in exponents
// result: current N being constructed
// prime_idx: current prime index to use
void dfs(vector<ll>& exponents, int idx, ll result, int prime_idx) {
    if (idx == exponents.size()) {
        ans = min(ans, result);
        return;
    }
    
    if (ans <= result) return; // Pruning
    if (prime_idx >= primes.size()) return; // Out of primes
    
    ll exp = exponents[idx];
    ll p = primes[prime_idx];
    
    // Calculate p^exp and check for overflow
    ll term = 1;
    bool overflow = false;
    for (ll i = 0; i < exp; ++i) {
        if (term > ans / p) {
            overflow = true;
            break;
        }
        term *= p;
    }
    
    if (overflow) return;
    
    if (result > ans / term) return; // Would overflow or exceed ans
    
    dfs(exponents, idx + 1, result * term, prime_idx + 1);
}

// Factorize K and find all ways to express K as product of positive integers
void factorize_and_search(ll k, vector<ll>& current_factors, int min_divisor) {
    if (k == 1) {
        // Sort in descending order, larger exponents should use smaller primes
        vector<ll> exponents = current_factors;
        sort(exponents.begin(), exponents.end(), greater<ll>());
        
        dfs(exponents, 0, 1, 0);
        return;
    }
    
    // Try all divisors >= min_divisor
    for (ll d = min_divisor; d * d <= k; ++d) {
        if (k % d == 0) {
            current_factors.push_back(d);
            factorize_and_search(k / d, current_factors, d);
            current_factors.pop_back();
        }
    }
    
    // k itself as a factor
    current_factors.push_back(k);
    dfs(current_factors, 0, 1, 0);
    current_factors.pop_back();
}

int main() {
    if (!(cin >> K)) return 0;

    if (K == 1) {
        cout << 1 << endl;
        return 0;
    }
    
    generate_primes(primes);
    
    vector<ll> factors;
    factorize_and_search(K, factors, 2);
    
    if (ans == 1e18) {
        cout << 0 << endl;
    } else {
        cout << ans << endl;
    }

    return 0;
}
