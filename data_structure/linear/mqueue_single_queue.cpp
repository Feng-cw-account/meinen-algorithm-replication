#include <queue>
#include <deque>
#include <algorithm>
#include <stdexcept>

// MQueue2: queue that supports O(1) amortized min query using an auxiliary monotonic deque
template <typename T>
class MQueue2 {
private:
    std::queue<T> self;        // main queue maintaining FIFO order
    std::deque<T> minDeque;    // monotonic (non-decreasing) deque of candidate minima
public:
    bool empty() const { return self.empty(); }
    size_t size() const { return self.size(); }

    void add(const T &v) {
        // push into main queue
        self.push(v);
        // maintain monotonic property of minDeque
        while (!minDeque.empty() && minDeque.back() > v) {
            minDeque.pop_back();
        }
        minDeque.push_back(v);
    }

    const T head() const {
        if (self.empty()) throw std::runtime_error("MQueue2: head() on empty queue");
        return self.front();
    }

    const T min() const {
        if (self.empty()) throw std::runtime_error("MQueue2: min() on empty queue");
        return minDeque.front();
    }

    void pop() {
        if (self.empty()) throw std::runtime_error("MQueue2: pop() on empty queue");
        const T &frontVal = self.front();
        if (!minDeque.empty() && minDeque.front() == frontVal) {
            minDeque.pop_front();
        }
        self.pop();
    }
};
