#include <iostream>
#include "mqueue.cpp"

int main() {
    Mqueue<int> q;
    q.push(1);
    q.push(2);
    std::cout << q.first() << "\n"; // 1
    q.pop();
    std::cout << q.first() << "\n"; // 2

    q.pop();
    try {
        q.first();
        std::cerr << "Error: should have thrown on empty queue" << std::endl;
        return 2;
    } catch (const std::out_of_range &e) {
        std::cout << "caught expected exception: " << e.what() << '\n';
    }

    std::cout << "Mqueue tests passed" << std::endl;
    return 0;
}
