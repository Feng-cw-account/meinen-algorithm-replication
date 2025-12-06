# std::vector 常用操作清单

本文档列举并说明 C++ 标准库 `std::vector` 的常见操作、函数与用法，包含示例、复杂度与注意事项，方便快速参考。

---

## 目录
- 创建与构造
- 访问元素
- 容量/尺寸管理
- 修改操作（增删改）
- 迭代器与遍历
- 算法与常见用法
- 性能与内存/迭代器失效
- 例子
- 常见陷阱与建议

---

## 创建与构造
- 默认构造: `std::vector<T> v;`
- 指定大小且初始化: `std::vector<T> v(n);` 或 `std::vector<T> v(n, value);`
- 使用 initializer list: `std::vector<int> v = {1,2,3};`
- 拷贝构造: `std::vector<T> v2(v1);`
- 移动构造: `std::vector<T> v2(std::move(v1));`
- 从区间构造: `std::vector<T> v(it_begin, it_end);`

**复杂度**: 构造通常为 O(n)（除默认构造），移动构造为 O(1)（实现内存转移）。

---

## 访问元素
- `v[i]`：无界检查，常数时间 O(1)
- `v.at(i)`：带范围检查，会抛出 `std::out_of_range`，常用在调试时
- `v.front()` / `v.back()`：访问首尾元素
- `v.data()`：获取底层连续内存指针（C 风格数组）

示例：
```cpp
std::vector<int> v = {10, 20, 30};
int x = v[1];        // 20
int y = v.at(2);     // 30, 检查越界
int &r = v.front();  // 引用到第一个元素
int *p = v.data();   // 指向底层数组的指针
```

---

## 容量 / 尺寸管理
- `v.size()`：当前元素数量
- `v.capacity()`：当前已分配容量
- `v.empty()`：是否为空
- `v.reserve(n)`：确保至少有 n 的容量（可减少扩容次数）
- `v.shrink_to_fit()`：尝试减小 capacity 至 size
- `v.resize(n)`：调整 size（若变大使用默认或给定值填充）
- `v.max_size()`：能分配的最大元素数量（实现相关）

示例：
```cpp
std::vector<int> v;
v.reserve(100);     // 减少后续扩容
v.resize(5);        // size = 5
v.shrink_to_fit();  // 通常把 capacity=5
```

---

## 修改操作（增删改）
- `push_back(value)`：尾部插入（拷贝/移动），摧毁迭代器可能失效
- `emplace_back(args...)`：构造元素到尾部，避免额外拷贝
- `pop_back()`：移除最后一个元素
- `insert(pos, value)`：在迭代器 `pos` 处插入
- `insert(pos, n, value)`：在 `pos` 处插入 n 个 value
- `insert(pos, first, last)`：在 `pos` 处插入 [first,last) 区间
- `emplace(pos, args...)`：在位置构造元素
- `erase(pos)`：移除某位置元素并返回下一个元素迭代器
- `erase(first, last)`：移除区间元素
- `clear()`：移除所有元素（size=0，但不改变 capacity）
- `assign(count, value)` 或 `assign(first, last)`：重新赋值
- `swap(v2)`：交换两个 vector 的内容（常数时间）

示例：
```cpp
std::vector<int> v;
v.push_back(1);
v.emplace_back(2);
v.insert(v.begin() + 1, 42);
v.erase(v.begin());
v.clear();
```

---

## 迭代器与遍历
- `begin()`、`end()`：正向迭代器
- `cbegin()`、`cend()`：const 迭代器
- `rbegin()`、`rend()`：反向迭代器
- `c` 前缀代表 const 版本（如 `crbegin()`）
- 注意：某些操作（如插入/删除、push_back 在扩容时）会导致迭代器失效。

遍历示例：
```cpp
for (int x : v) { /* range-based loop */ }
for (auto it = v.begin(); it != v.end(); ++it) { /* classic iterator */ }
for (auto rit = v.rbegin(); rit != v.rend(); ++rit) { /* reverse iterate */ }
```

---

## 算法与常见用法
- 排序: `std::sort(v.begin(), v.end())`
- 查找: `std::find(v.begin(), v.end(), value)`
- 去重: `v.erase(std::unique(v.begin(), v.end()), v.end())`（先排序或按需）
- 删除-擦除（remove-erase）惯用法:
	- `v.erase(std::remove(v.begin(), v.end(), value), v.end());`
- 插入排序/相同行为: `std::lower_bound` 可与 `insert` 结合
- `std::move` 与 vector: 将元素移动到另一个容器

示例：
```cpp
std::sort(v.begin(), v.end());
auto it = std::lower_bound(v.begin(), v.end(), 10);
v.insert(it, 10);
// 删除所有为 0 的元素
v.erase(std::remove(v.begin(), v.end(), 0), v.end());
```

---

## 性能与内存 / 迭代器失效
- 插入（push_back 和 insert）在触发扩容时会重新分配内存 => 使所有指向元素的指针、引用和迭代器失效
- `push_back` 平均摊复杂度为 O(1)，在扩容时为 O(n)
- `insert` 在中间插入的复杂度是 O(n)
- `erase` 删除中间元素复杂度为 O(n)，删除末尾元素为 O(1)
- `reserve` 可以减少重复分配次数
- `shrink_to_fit` 只是一种非强制请求；实现可能忽略

迭代器失效规则（常见）:
- `push_back`/`emplace_back` 在 capacity 不足时会触发重分配 -> 所有迭代器失效
- `insert` 或 `erase` 会使得之后位置的迭代器失效（取决于实现和操作位置）
- `clear()` 将使元素无效，但不保证 capacity 减少
- `swap`、移动构造/移动赋值通常不会引起大量拷贝

---

## 与 C 风格数组 / API 交互
- `v.data()` 返回连续内存的 `T*`，方便传递指针给 C API
- 修改 `v`（如 `push_back` 导致重分配）之后，`data()` 返回的指针可能失效

---

## 进阶/实用技巧
- 使用 `reserve` 提前分配内存以提升性能
- 使用 `emplace_back` 避免不必要的拷贝或移动
- 用 `std::move` 将元素移动到另一容器以避免拷贝
- 使用 `swap` 或者 `vector<T>().swap(v)` 实现 shrink-to-fit 的 portable trick（旧技巧，C++11 后 `shrink_to_fit` 推荐）

---

## 示例（完整）
```cpp
#include <vector>
#include <algorithm>
#include <iostream>

int main() {
		std::vector<int> v = {3, 1, 4, 1, 5, 9};
		// push_back / emplace_back
		v.push_back(2);
		v.emplace_back(6);

		// iterate
		for (int x : v) std::cout << x << ' ';
		std::cout << '\n';

		// sort + unique + erase (去重示例)
		std::sort(v.begin(), v.end());
		v.erase(std::unique(v.begin(), v.end()), v.end());

		// remove-erase: 删除所有为 1 的元素
		v.erase(std::remove(v.begin(), v.end(), 1), v.end());

		// use data() to call a C API that expects int* and size
		int *arr = v.data(); // 注意：push_back 后可能失效
		size_t n = v.size();

		// reserve/resize
		v.reserve(100);
		v.resize(10, 0);
}
```

---

## 常见陷阱与建议
- 切记迭代器失效规则：在保留指针/引用/迭代器的同时修改容器时要小心
- `operator[]` 不做边界检查，调试阶段建议用 `at()` 或添加断言
- 使用 `reserve` 可以避免频繁扩容带来的性能损耗
- `shrink_to_fit()` 并非强制，不能保证容量一定会减少
- 大量头尾插入/删除应考虑 `deque` 或 `list`（但 `list` 失去连续内存优势）

---

## 参考
- C++ 标准文档（容器章节）
- cppreference: https://en.cppreference.com/w/cpp/container/vector


