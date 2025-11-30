#include <stack>
#include <utility>
#include <algorithm>
#include <iostream>
#include <stdexcept>

template<typename T>
using mStack = std::stack<std::pair<T, T>>;

using minStack = mStack<int>;
using node = std::pair<int, int>;

// 最小栈：每个元素存储 (当前值, 栈内最小值)
class MinStack {
public:
    MinStack() = default;

    void push(int x) {
        if (self.empty()) {
            self.push({x, x});
        } else {
            self.push({x, std::min(x, self.top().second)});
        }
    }

    void pop() {
        if (self.empty()) {
            throw std::runtime_error("Stack is empty");
        }
        self.pop();
    }

    int top() {
        if (self.empty()) {
            throw std::runtime_error("Stack is empty");
        }
        return self.top().first;
    }

    int min() {
        if (self.empty()) {
            throw std::runtime_error("Stack is empty");
        }
        return self.top().second;
    }

    bool empty() const {
        return self.empty();
    }

    size_t size() const {
        return self.size();
    }

private:
    minStack self;
};

// 最小值队列：使用两个最小栈模拟队列，支持 O(1) 查询最小值
class MinQueue {
public:
    MinQueue() = default;

    void push(int x) {
        stack_in.push(x);
    }

    void pop() {
        if (empty()) {
            throw std::runtime_error("Queue is empty");
        }
        if (stack_out.empty()) {
            transfer();
        }
        stack_out.pop();
    }

    int front() {
        if (empty()) {
            throw std::runtime_error("Queue is empty");
        }
        if (stack_out.empty()) {
            transfer();
        }
        return stack_out.top();
    }

    int min() {
        if (empty()) {
            throw std::runtime_error("Queue is empty");
        }
        if (stack_in.empty()) {
            return stack_out.min();
        }
        if (stack_out.empty()) {
            return stack_in.min();
        }
        return std::min(stack_in.min(), stack_out.min());
    }

    bool empty() const {
        return stack_in.empty() && stack_out.empty();
    }

    size_t size() const {
        return stack_in.size() + stack_out.size();
    }

private:
    MinStack stack_in;   // 入队栈
    MinStack stack_out;  // 出队栈

    void transfer() {
        while (!stack_in.empty()) {
            stack_out.push(stack_in.top());
            stack_in.pop();
        }
    }
};

int main() {
    MinQueue q;

    // 测试入队
    q.push(3);
    q.push(1);
    q.push(4);
    q.push(1);
    q.push(5);

    std::cout << "队列最小值: " << q.min() << std::endl;  // 输出 1
    std::cout << "队首元素: " << q.front() << std::endl;  // 输出 3

    q.pop();
    std::cout << "出队后队首: " << q.front() << std::endl;  // 输出 1
    std::cout << "出队后最小值: " << q.min() << std::endl;  // 输出 1

    q.pop();
    q.pop();
    std::cout << "再出队两次后最小值: " << q.min() << std::endl;  // 输出 1

    q.pop();
    std::cout << "再出队后最小值: " << q.min() << std::endl;  // 输出 5

    std::cout << "队列大小: " << q.size() << std::endl;  // 输出 1

    return 0;
}
