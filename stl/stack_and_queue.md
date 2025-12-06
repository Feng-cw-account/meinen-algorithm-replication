# 衍生类型（容器适配器） —— stack / queue / priority_queue

容器适配器（container adaptor）是基于已有的序列容器（如 `deque`、`vector`、`list` 等）提供受限接口的容器类型。
它把底层容器的操作包装成一种更高层的抽象（例如栈/队列/优先队列），隐藏底层实现细节并提供固定的行为语义。

本文以 `std::stack`、`std::queue` 和 `std::priority_queue` 为例，说明它们的用途、API、复杂度和常见注意点。

---

- ## 概要与共同特性

- 容器适配器通常不提供迭代器接口（purposefully）——它们只提供受限的访问方式（LIFO / FIFO / heap）。
- 默认底层容器：`std::deque`（`std::stack` 和 `std::queue`）; `std::vector`（`std::priority_queue` 的默认容器为 `std::vector`，但 `deque` 也是可行的）。
- 可以指定自定义底层容器以及在 priority_queue 中自定义比较器。
- 适配器通过模板参数传递底层容器和比较器（对于 `priority_queue`）。

---

- ## 1) std::stack（LIFO）

- 用法：先进后出（Last In First Out）
- 常用成员函数：
  - `empty()`、`size()`
  - `push(const T&)`、`push(T&&)`、`emplace(args...)`
  - `pop()`（返回 void）
  - `top()`（返回引用，访问栈顶元素）
- 复杂度：`push` / `pop` / `top` 都是 O(1) 平均时间复杂度（底层容器的对应操作通常是 O(1)）。
- 底层容器约束：需要支持 `back()`, `push_back()`, `pop_back()`（例如 `deque`、`vector`、`list` 都可用）。

示例：

```cpp
#include <stack>
#include <iostream>

  int main() {
    std::stack<int> s; // 默认使用 deque
    s.push(10);
    s.emplace(20);
    std::cout << s.top() << '\n'; // 20
    s.pop();
    if (!s.empty()) std::cout << s.top() << '\n';
}
```

注意：`pop()` 只移除元素，不返回元素的副本；如果你需要值，先使用 `top()` 再 `pop()`。

---

- ## 2) std::queue（FIFO）

- 用法：先进先出（First In First Out）
- 常用成员函数：
  - `empty()`, `size()`
  - `push()` / `emplace()`（在尾部插入）
  - `pop()`（移除头部）
  - `front()` / `back()`（访问头/尾元素）
- 复杂度：`push` / `pop` / `front` / `back` 均为 O(1)（由底层容器保证）。
- 底层容器约束：需要支持 `push_back()`, `pop_front()`, `front()`, `back()`（典型是 `deque`；`list` 也可，但 `vector` 不行因为没有 pop_front）。

示例：

```cpp
#include <queue>
#include <string>
#include <iostream>

  int main() {
    std::queue<std::string> q;
    q.push("a");
    q.emplace("b");
    std::cout << q.front() << ' ' << q.back() << '\n'; // a b
    q.pop();
    std::cout << q.front() << '\n'; // b
}
```

注意：`pop()` 也不返回值，需配合 `front()` 读取数据。

---

- ## 3) std::priority_queue（优先队列 / 堆）

- 用法：基于比较器维护堆，`top()` 返回优先级最高（默认是最大元素）的值。
- 常用模板参数：`std::priority_queue<T, Container, Compare>`
  - `T`：元素类型
  - `Container`：底层容器（必须提供 RandomAccessIterator，默认 `std::vector<T>`，`std::deque<T>` 也可用）
  - `Compare`：比较器，保持堆属性（默认 `std::less<T>` -> 最大堆）
- 常用成员：`empty()`, `size()`, `top()`, `push()`, `emplace()`, `pop()`。
- 复杂度：`push` 和 `pop` 为 O(log n)，`top` 为 O(1)。

示例（默认最大堆）：

```cpp
#include <queue>
#include <vector>
#include <iostream>

  int main() {
    std::priority_queue<int> pq; // max-heap
    pq.push(5);
    pq.push(1);
    pq.push(3);
    std::cout << pq.top() << '\n'; // 5
    pq.pop();
    std::cout << pq.top() << '\n'; // 3
}
```

示例（最小堆）：

```cpp
#include <queue>
#include <vector>
#include <functional>
#include <iostream>

  int main() {
    std::priority_queue<int, std::vector<int>, std::greater<int>> minheap; // min-heap
    minheap.push(5);
    minheap.push(1);
    minheap.push(3);
    std::cout << minheap.top() << '\n'; // 1
}
```

示例（自定义类型和比较器）：

```cpp
#include <queue>
#include <vector>
#include <iostream>

using P = std::pair<int, int>;
struct Cmp {
  bool operator()(const P &a, const P &b) const {
    return a.first > b.first; // 小顶堆按 first
  }
};

int main() {
    std::priority_queue<P, std::vector<P>, Cmp> pq;
    pq.emplace(10, 1);
    pq.emplace(5, 2);
    std::cout << pq.top().first << '\n'; // 5
}
```

注意：`Compare` 必须定义为严格弱序（strict weak ordering），不然堆行为未定义或不符合预期。

---

- ## 常见用法与示例场景

- `std::stack`：函数调用/递归展开、DFS（图遍历的显式栈）、撤销/恢复（undo/redo）等
- `std::queue`：广度优先搜索（BFS）、任务队列、消息缓冲
- `std::priority_queue`：任务调度（最大/最小优先级）、Dijkstra 算法（最短路径）、A* 中的 open set、Top-K 问题

示例：Top-K 小值（使用大顶堆 / 小顶堆策略的差异）：

```cpp
// Top-K 小值，用大小为 k 的 max-heap（默认 pq）来维护
#include <queue>
#include <vector>
using namespace std;

  vector<int> topk_small(const vector<int>& a, int k) {
    priority_queue<int> pq; // max-heap
    for (int x : a) {
      pq.push(x);
      if ((int)pq.size() > k) pq.pop();
    }
    vector<int> res;
    while (!pq.empty()) { res.push_back(pq.top()); pq.pop(); }
    return res; // 注意：按降序返回
}
```

---

- ## 底层容器选择与性能考虑

- 默认 `deque`（stack/queue）的优势是两端 O(1) 的插入/删除，并且支持随机访问（对 deque 而言，不过随机访问成本和 vector 类似）。
- `vector` 更节省内存并且内存局部性更好，但不支持 `pop_front()`，因此不适合作为 `queue` 的底层容器。
- 对于 `priority_queue`，默认 `vector` 是比较合适的：堆操作依赖随机访问迭代器且频繁重排，`vector` 的内存局部性会更好。
- `list` 作为底层容器适用于极少数场景（例如特殊内存布局或实时性），但 `list` 没有随机访问，不适用于 `priority_queue`。

---

- ## 迭代器與底层容器访问（高级技巧）

- 容器适配器刻意不提供迭代器。如果确实需要遍历或访问底层容器内容：
  - 通过底层容器构造 `std::vector` 等可迭代类型后再遍历（需要复制/移动成本）；
  - （不推荐）继承 `std::stack` 并访问受保护的 `c` 成员；但这种方式依赖实现细节且违反封装，不建议用于生产代码。

示例（不推荐但可用于调试）:

```cpp
// 访问 std::stack 的底层容器（仅调试/测试）
#include <stack>
#include <deque>
#include <iostream>

template <typename T, typename Container = std::deque<T>>
struct DebugStack : std::stack<T, Container> {
  const Container &underlying() const { return this->c; } // c 是受保护成员
};

int main() {
  DebugStack<int> ds;
  ds.push(1); ds.push(2); ds.push(3);
  for (int x : ds.underlying()) std::cout << x << ' '; // 1 2 3
}
```

---

- ## 常见陷阱与建议

- 注意容器适配器没有迭代器 —— 若需要遍历请使用底层容器或其他容器类型。
- `pop()` 不返回值：如果需要取出元素值，请先 `top()`/`front()` 再 `pop()`。
- 在性能敏感情形中，选择合适的底层容器（默认选项通常合适，但了解不同实现的开销能帮助调优）。
- `priority_queue` 的比较器方向常容易出错（默认 `less` = max-heap，若要 min-heap 使用 `greater`）。
- 线程安全：容器本身不是线程安全的；在多线程场景需显式同步。

---

## 参考与进一步阅读

- cppreference: [container adaptors](https://en.cppreference.com/w/cpp/container)

