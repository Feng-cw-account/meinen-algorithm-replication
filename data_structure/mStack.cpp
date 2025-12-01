#include <stack>
#include <utility>
#include <algorithm>
#include <iostream>
#include <memory>

template<typename T>
using mStack = std::stack<std::pair<T, T>>;

using maxStack = mStack<int>;
using node = std::pair<int, int>;

class MaxStack {
public:
MaxStack() = default;
void push(int x);
int top();
int max();
void pop();
private:
maxStack self;
};
void MaxStack::push(int x) {
    node nx;
    if (self.empty()) {
        nx = node(x, x);

    } else {
        int old_max = self.top().first;
        nx = node(std::max(x, old_max),x);
    }

    self.push(nx);
}

int MaxStack::top() {
    return self.top().second;
}

int MaxStack::max() {
    return self.top().first;
}

void MaxStack::pop() {
    self.pop();
}

