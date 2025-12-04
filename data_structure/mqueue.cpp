#include<vector>

template<typename T>
using vec = std::vector<T>;

template <typename T> 
class MQueue{
private:
    vec self;
public:
    void push(T x);
    T first();
    void pop();

};

template <typename T>
void MQueue<T>::push(T x) {
    self.push_back(x);
}
