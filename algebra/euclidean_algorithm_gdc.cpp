#include <iostream>

int gcd(int a, int b) {
    if (b == 0) return a;
    return gcd(b, a % b);
}

int main() {
    int a = 48;
    int b = 18;
    std::cout << "GCD of " << a << " and " << b << " is " << gcd(a, b) << std::endl;
    return 0;
}
