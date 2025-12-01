#include <iostream>

template<typename T>
struct BinaryOperation {
    virtual T operator()(T a, T b) const = 0;
    virtual ~BinaryOperation() = default;
};

template<typename T>
T power(T base, int exponent, T identity, const BinaryOperation<T>& operation) {
    if (exponent <= 0) {
        throw std::invalid_argument("Exponent must be non-negative");
    }

    T weight = base;
    T result = identity;

    while(exponent > 0) {
        if (exponent & 1){
            result = operation(result, weight);
        }
        // over  calculation
        weight = operation(weight, weight);
        exponent >>= 1;
    }

    return result;
}

struct Multiply : BinaryOperation<int> {
    int operator()(int a, int b) const override {
        return a * b;
    }
};

int main() {
    int base = 5;
    int exponent = 10;
    int identity = 1; // Identity for multiplication
    Multiply multiplyOp;
    int result = power(base, exponent, identity, multiplyOp);
    std::cout << base << " raised to the power of " << exponent << " is " << result << std::endl;
    return 0;
   
}