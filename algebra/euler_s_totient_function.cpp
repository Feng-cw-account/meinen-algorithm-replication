int totient(int n) {
    // handle trivial case
    if (n == 1) return 1;

    int result = n;
    
    for (int base=2; base <= n / base; ++base) {
        if (n % base == 0) {
            // change state of n
            while (n % base == 0) n = n / base;
            // change result
            result = result - result / base;
        }
    }

    // handle left-out factor
    if (n != 1) result = result - result / n;

    return result;
}