#include <vector>
#include <utility>
#include <stdexcept>
#include <algorithm>

template <typename T>
using paar = std::pair<T,T>;
template<typename T>
using vec = std::vector< paar<T> >;


template <typename T>
class MStack{
private:
vec<T> self;
public:
bool empty() const {
    return self.empty();
}

void push(const T &elem) {
    paar<T> new_pair;
    if (self.empty()){
        new_pair = paar<T>(elem, elem);
    } else {
        // compute new min value as min(elem, previous_min)
        new_pair = paar<T>(elem, std::min(elem, self.back().second));
    }

    self.push_back(new_pair);
}

T top() const {
    if (self.empty()) {
        throw std::runtime_error("MStack: top() called on empty stack");
    }
    return self.back().first;
}
T min() const {
    if (self.empty()) {
        throw std::runtime_error("MStack: min() called on empty stack");
    }
    return self.back().second;
}

void pop() {
    if (self.empty()) {
        throw std::runtime_error("MStack: pop() called on empty stack");
    }
    self.pop_back();
}
};

template <typename T> 
class MQueue{
private:
    MStack<T> in;
    MStack<T> out;
    const paar<T> _get_head_elem();
public:
    void add(T);
    const T head();
    const T min();
    void pop();
};

// NOTE: framework-only, TODO: implement algorithm logic
template <typename T>
void MQueue<T>::add(T elem) {
    in.push(elem);
}

template <typename T>
const paar<T> MQueue<T>::_get_head_elem() {
    if (out.empty()) {
        if (in.empty()) throw std::runtime_error("don't read empty queue!");
        while (! in.empty()) {
            out.push(in.top());
            in.pop();
        }
    }
    if (in.empty()) return paar<T>(out.top(), out.min());
    return paar<T>(out.top(), std::min(in.min(), out.min()));
}

template <typename T>
const T MQueue<T>::head() {
    return _get_head_elem().first;
}
template <typename T>
const T MQueue<T>::min() {
    return _get_head_elem().second;
}

template <typename T>
void MQueue<T>::pop() {
    _get_head_elem();
    out.pop();
}