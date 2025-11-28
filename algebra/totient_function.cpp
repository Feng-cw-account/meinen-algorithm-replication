#include <stdexcept>
#include <vector>

using vec = std::vector<int>;

int totient_single(int n) {
    if (n < 1) {
        throw std::invalid_argument("Input must be a positive integer.");
    }
    if (n == 1) {
        return 1;
    }

    int result = n;
    // edge fator: if there is factor equals to squart of n, its exponent can more than 1, so should be included.
    for (int p = 2; p * p <= n; ++p) {
        // for any non-prime numbers, we know that their prime factors(naturally less then itself) is already checked
        // so we can skip them
        if (n % p == 0) {
            while (n % p == 0) {
                n = n / p;
            }
            result = result - result / p;
        }
    }
    // handle last one
    result = result - result / n; // if n == 1, the logic still works.

    return result;
}

vec totient_range_eratosthenes(int n) {
    vec result;
    
    if (n < 1) throw std::invalid_argument("Input must be a positive integer.");
    
    for (int i = 0; i < n+1; ++i) {
        result.push_back(i);
    }
    if (n == 1) return result;

    for (int possible_factor = 2; possible_factor <= n; ++possible_factor) {
        // check if possible_factor is a prime
        if (result[possible_factor] == possible_factor) {
            for (int affected_x = possible_factor; affected_x <= n; affected_x += possible_factor) {
                result[affected_x] = result[affected_x] - result[affected_x] / possible_factor;
            }   
        }
    }

    return result;
}

vec totient_range_euler(int n) {
    vec result;

    if (n < 1) throw std::invalid_argument("Input must be a positive integer.");

    for(int i = 0; i < n+1; ++i) {
        result.push_back(i);
    }

    if (n == 1) return result;

    vec found_prime;

    // how to ensure k and p's result is finished
    for (int possible_k = 2; possible_k <= n; ++possible_k) {
        if (result[possible_k] == possible_k) {
            found_prime.push_back(possible_k);
            result[possible_k] = possible_k - 1;
        }
        for (int possible_p : found_prime) {
            if (possible_k * possible_p > n) break;
            
            if (possible_k % possible_p == 0) {
                result[possible_k * possible_p] = result[possible_k] * possible_p;
                break;
            } else {
                result[possible_k * possible_p] = result[possible_k] * (possible_p - 1);
            }
        }
    }

    return result;
}
