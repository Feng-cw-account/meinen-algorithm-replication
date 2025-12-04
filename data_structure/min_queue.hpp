#pragma once

#include <stack>
#include <algorithm>
#include <stdexcept>

// MinQueue<T> implemented using two stacks (in and out) with min tracking
// Each stack stores pairs {value, current_min_in_stack}
// push: O(1)
// pop: amortized O(1) (moving elements from in->out when needed)
// front: amortized O(1)
// get_min: O(1)

template <typename T>
class MinQueue {
private:
    std::stack<std::pair<T, T>> in;   // elements pushed recently, pair = (value, min_so_far_in_stack)
    std::stack<std::pair<T, T>> out;  // elements to pop from, pair = (value, min_so_far_in_stack)

    static T top_min(const std::stack<std::pair<T,T>>& s) {
        return s.empty() ? T() : s.top().second;
    }

public:
    MinQueue() = default;

    bool empty() const { return in.empty() && out.empty(); }

    void push(const T& x) {
        T curmin = in.empty() ? x : std::min(x, in.top().second);
        in.emplace(x, curmin);
    }

    void pop() {
        if (out.empty()) {
            // move everything from in to out recalculating minimums
            while (!in.empty()) {
                T val = in.top().first;
                in.pop();
                T newmin = out.empty() ? val : std::min(val, out.top().second);
                out.emplace(val, newmin);
            }
        }
        if (out.empty()) throw std::out_of_range("MinQueue::pop: empty queue");
        out.pop();
    }

    const T& front() {
        if (out.empty()) {
            while (!in.empty()) {
                T val = in.top().first;
                in.pop();
                T newmin = out.empty() ? val : std::min(val, out.top().second);
                out.emplace(val, newmin);
            }
        }
        if (out.empty()) throw std::out_of_range("MinQueue::front: empty queue");
        return out.top().first;
    }

    T get_min() const {
        if (empty()) throw std::out_of_range("MinQueue::get_min: empty queue");
        if (in.empty()) return out.top().second;
        if (out.empty()) return in.top().second;
        return std::min(in.top().second, out.top().second);
    }
};
