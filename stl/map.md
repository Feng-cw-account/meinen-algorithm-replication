
# std::map / std::unordered_map 使用指南

本文档快速介绍 `std::map` / `std::multimap`（有序映射）以及 `std::unordered_map` / `std::unordered_multimap`（无序哈希映射）的常用 API、复杂度、使用建议与示例。

---

## 有序映射（std::map / std::multimap）

`std::map<Key, T, Compare>`：基于平衡二叉树（通常为红黑树），按 key 排序，键唯一；插入/删除/查找 O(log n)。

`std::multimap<Key, T, Compare>`：类似 map，但允许多个相同的键。

常用接口：`insert`, `emplace`, `erase`, `find`, `count`, `lower_bound`, `upper_bound`, `equal_range`。

示例：

```cpp
#include <map>
#include <iostream>

int main(){
    std::map<std::string,int> m;
    m["apple"] = 2;              // 插入或更新
    m.emplace("banana", 3);      // 插入
    auto it = m.find("apple");
    if (it != m.end()) std::cout << it->first << ':' << it->second << '\n';
}
```

注意：`operator[]` 在不存在 key 时会插入默认构造的值，若仅用于查询请用 `find()` 或 `at()`。

---

## 无序映射（std::unordered_map / unordered_multimap）

基于哈希表，平均复杂度 O(1)（最坏 O(n)），适合大量随机查找。常用 API 与 map 类似，但提供哈希相关控制：`reserve`、`rehash`、`load_factor`。

示例：

```cpp
#include <unordered_map>
#include <iostream>

int main(){
    std::unordered_map<std::string,int> um;
    um.reserve(100);             // 减少 rehash
    um.try_emplace("a", 1);
    if (um.find("a") != um.end()) std::cout << um["a"] << '\n';
}
```

---

## 插入与访问对比

- `insert(pair)`：若 key 存在，插入失败且返回失败标志。
- `emplace(args...)`：高效构造并插入。
- `try_emplace(key, args...)`（C++17）：仅当 key 不存在时构造并插入。
- `insert_or_assign(key, value)`（C++17）：存在则赋值，否则插入。
- `operator[]`：用于读写，可能插入默认值（不要用于只查找场合）。

---

## 多重映射与范围查询

`std::multimap` 允许重复 key；`equal_range(key)` 返回构成该 key 的元素区间 [lower_bound, upper_bound)。

示例：

```cpp
#include <map>
int main(){
    std::multimap<int, char> mm;
    mm.emplace(1,'a'); mm.emplace(1,'b');
    auto r = mm.equal_range(1);
    for (auto it=r.first; it!=r.second; ++it) std::cout << it->second << '\n';
}
```

---

## 性能与迭代器失效规则

- `std::map`：插入不会使现有迭代器失效；erase 使被删除迭代器失效。
- `std::unordered_map`：在 `rehash` 或扩容时会使所有迭代器失效；erase 使被删除迭代器失效。

复杂度对比：`map` O(log n)，`unordered_map` 平均 O(1)，但可能受哈希函数质量与负载因子影响。

---

## 高级：自定义 comparator / hash / node handle

- `std::map<Key,T,Compare>`：Compare 必须提供严格弱序。
- `std::unordered_map<Key,T,Hash,KeyEqual>`：自定义哈希与相等比较。
- 节点操作（C++17）：`extract()` 与 `insert(node)` 可安全高效地移动键值节点。

示例（自定义 hash）：

```cpp
#include <unordered_map>
#include <string>
struct MyHash { size_t operator()(const std::string &s) const noexcept { return std::hash<std::string>{}(s); } };
struct MyEq { bool operator()(const std::string &a, const std::string &b) const noexcept { return a==b; } };
int main(){
    std::unordered_map<std::string,int,MyHash,MyEq> um;
    um.emplace("k", 42);
}
```

---

## 何时选择 map 或 unordered_map

- 需要有序遍历 / 范围查询：`map`。
- 性能关键且无需顺序：`unordered_map`（预留容量以避免重哈希）。

---

## 常见陷阱

- `operator[]` 插入默认构造值的副作用。
- 不一致的哈希与等于导致查找失败。
- 在热路径中尽量避免多余的复制，使用 `emplace` / `try_emplace`。

---

## 参考

- cppreference: `std::map` / `std::unordered_map`



# Test Minimal

This is a small placeholder to make sure we can write a clean file.

# std::map / std::unordered_map 使用指南

简介：本文档介绍 `std::map`, `std::multimap`, `std::unordered_map`, `std::unordered_multimap` 的常见用法、复杂度与实践建议。

---

## std::map / std::multimap 概览

`std::map<Key, T, Compare>`：基于有序树（通常是红黑树），按 key 排序、key 唯一，插入/查找/删除复杂度为 O(log n)。

`std::multimap<Key, T, Compare>`：允许重复键的有序关联容器，接口类似 `map`，但 key 可重复。

特点与适用场景：
- 需要按 key 保持顺序/做范围查询使用 `std::map`。
- 需要多个相同 key 的值使用 `std::multimap`（或 `std::map<Key, vector<T>>` 看场景）。

---

## std::unordered_map / unordered_multimap 概览

基于哈希表实现，平均复杂度为 O(1)（最坏 O(n)），适合大量随机查找场景。需要自定义哈希函数或等价判断时，要保证 hash 与 equal 的一致性。

-- API 相关：`reserve()`、`rehash()`、`load_factor()`、`max_load_factor()` 等。

---

## 常用基础操作与差异

- 访问: `operator[]`（会在没有 key 时插入默认值），`at()`（抛出异常），`find()`（返回 iterator）
- 插入: `insert` / `emplace` / `try_emplace`（C++17） / `insert_or_assign`（C++17）
- 删除: `erase(key)` / `erase(it)` / `erase(range)`
- 查询: `lower_bound` / `upper_bound`（仅有序 map） / `equal_range`（multimap）

注意：`operator[]` 可能会产生副作用（插入默认构造的值），对于只读查询请使用 `find()` 或 `contains()`（C++20）。

---

## 插入技巧与性能

- 对于 `unordered_map`，在已知预期大小时先 `reserve(n)` 减少 rehash 的开销。
- `try_emplace(key, args...)`（C++17）比 `operator[]` 或 `emplace` 更高效，可避免值的多次构造/移动。
- `insert_or_assign` 在键存在时直接赋值（避免双查找）。

示例：

```cpp
#include <unordered_map>
#include <iostream>

int main() {
    std::unordered_map<std::string, int> um;
    um.reserve(100);
    um.try_emplace("apple", 3);
    um.insert_or_assign("apple", 4);
    auto it = um.find("apple");
    if (it != um.end()) std::cout << it->first << ':' << it->second << '\n';
}
```

---

## 平均复杂度参考
- `std::map`：O(log n) 查找/插入/删除
- `std::unordered_map`：平均 O(1) 查找/插入；最坏 O(n)

迭代器失效：
- 对于 `std::map`：插入不会使现有迭代器失效，erase 仅会失效被删除的迭代器。
- 对于 `std::unordered_map`：在 rehash 时会使所有迭代器失效。

---

## 高级：自定义 comparator / hash / node handle

- `std::map<T, U, Compare>`：Compare 必须为严格弱序。
- `std::unordered_map<Key, T, Hash, KeyEqual>`：自定义 `Hash` 与 `KeyEqual`。
- C++17 节点操作：`extract()`/`insert(node)` 可以在不同容器间高效移动节点而不重建对象。

示例（自定义 hash/eq）：

```cpp
#include <unordered_map>
#include <string>
#include <iostream>

struct MyHash {
    size_t operator()(const std::string &s) const noexcept { return std::hash<std::string>{}(s); }
};
struct MyEq { bool operator()(const std::string &a, const std::string &b) const noexcept { return a == b; } };

int main() {
    std::unordered_map<std::string, int, MyHash, MyEq> um;
    um.emplace("k", 42);
}
```

---

## 何时选择哪种 map

- 需要顺序遍历 / 有序区间处理：`std::map`（或 `std::multimap`）
- 需要最快平均查找速度：`std::unordered_map`（尽量 `reserve`）
- 需要稳定迭代器并发插入/删除：`std::map` 更安全（视实现）

---

## 常见陷阱

- `operator[]` 会在不存在 key 时创建默认值——此行为常被误用。
- 误用 `Hash` 或 `KeyEqual` 导致查找失败或 bug（哈希函数必须与相等比较一致）。
- 在性能关键路径中多次拷贝 value，优先使用 `emplace`/`try_emplace`。

---

## 参考

- cppreference: `std::map` / `std::unordered_map`



# std::map / std::unordered_map 使用指南

本文档介绍 C++ 标准库中常用的映射容器：`std::map`, `std::multimap`, `std::unordered_map`, `std::unordered_multimap`。
内容包括 API 列表、复杂度说明、示例代码、性能建议、以及常见陷阱。

---

## 概要

`std::map<Key, T, Compare>`：基于平衡二叉树（一般为红黑树）的有序关联容器；按 key 排序；key 唯一。

`std::multimap<Key, T, Compare>`：允许重复 key 的有序关联容器。

`std::unordered_map<Key, T, Hash, KeyEqual>`：基于哈希表的无序关联容器；平均 O(1) 查找/插入；key 唯一。

`std::unordered_multimap<Key, T, Hash, KeyEqual>`：多重键版本，允许重复 key。

选择基础：需要按 key 排序或范围查询请选择 `std::map`；需要最快的平均查找请选择 `std::unordered_map`（并注意内存和哈希成本）。

---

## 常用操作

构造：`map<Key,T> m;`、`map m = { {k1, v1}, {k2,v2}};`

访问：`operator[]`, `at(key)`, `find(key)`, `count(key)`（map 为 0/1）、`contains(key)`（C++20）

插入：`insert({k,v})`、`insert(hint, ...)`、`emplace(...)`、`try_emplace(key, args...)`（C++17）

替换/更新：`m[key] = new_value;` 或 `it->second = new_value;`

删除：`erase(key)`、`erase(it)`、`erase(range)`，返回值行为视 API 而定

查询范围与边界：`lower_bound`, `upper_bound`, `equal_range`（multimap）

示例：

```cpp
#include <map>
#include <iostream>

int main() {
    std::map<std::string, int> m;
    m["one"] = 1;                 // 插入或更新
    m.insert({"two", 2});         // 插入
    m.emplace("three", 3);        // 直接构造

    auto it = m.find("two");     // find 返回 iterator
    if (it != m.end()) std::cout << it->first << ' ' << it->second << '\n';

    // 遍历（map 按 key 排序）
    for (auto &p : m) std::cout << p.first << ':' << p.second << '\n';
}
```

---

## map 与 unordered_map 的复杂度与特性

`std::map`: 插入/查找/删除 都为 O(log n)，按 key 排序，迭代器稳定：插入不会使其他迭代器失效；erase 只失效被删除的迭代器。

`std::unordered_map`: 平均 O(1)，但最坏 O(n)（哈希冲突）；重哈希（rehash）会使所有迭代器失效；插入可能触发 rehash。

额外的哈希相关 API:

- `reserve(n)`：为至少存放 n 个元素做准备（有助于减少 rehash 次数）
- `rehash(b)`：调整桶数为至少 b
- `max_load_factor()` / `load_factor()`：控制和查询哈希表装载因子

示例：

```cpp
#include <unordered_map>
#include <iostream>

int main() {
    std::unordered_map<std::string, int> um;
    um.reserve(100);
    um["apple"] = 3;
    if (um.find("apple") != um.end()) std::cout << "found" << '\n';
}
```

---

## 插入 API 比较（insert / emplace / try_emplace / operator[]）

`insert(pair)`：若键已存在，插入失败且返回包含 map.end() 的 pair。

`emplace(args...)`：通过构造函数参数直接创建元素并插入，避免临时对象 (perfect-forwarding)。

`try_emplace(key, args...)`（C++17）：仅当键不存在时进行构造与插入，避免不必要的对象构造或拷贝。

`operator[]`：若键不存在，会默认构造值并插入（对 unordered_map、map 都可），因此仅用于 "读-写" 场景而不是只读查找。

`insert_or_assign`（C++17）：若键不存在插入，否则替换 value。

示例：

```cpp
#include <map>
#include <iostream>

int main() {
    std::map<int, std::string> m;
    m.insert({1, "a"});
    m.emplace(2, "b");
    m.try_emplace(3, "c");       // 仅当 3 不存在时插入
    m.insert_or_assign(2, "B");   // 将 2 的值更新为 "B"

    auto result = m.insert({3, "x"}); // 插入失败（已存在）
    if (!result.second) std::cout << "key 3 already exists\n";
}
```

---

## 多重映射（multimap）与范围查询

`std::multimap` 允许相同 key 重复。常用 API: `equal_range(key)` 返回 [lower_bound, upper_bound) 区间。

示例：

```cpp
#include <map>
#include <iostream>

int main() {
    std::multimap<int, std::string> mm;
    mm.emplace(1, "a"); mm.emplace(1, "b"); mm.emplace(2, "c");

    auto range = mm.equal_range(1);
    for (auto it = range.first; it != range.second; ++it) std::cout << it->second << '\n';
}
```

---

## 自定义 Key / 比较器 / hash

std::map: 通过第三个模板参数指定比较器：`std::map<Key, T, Compare>`，`Compare` 必须是严格弱序。

std::unordered_map: 通过第二和第三模板参数指定 `Hash` 与 `KeyEqual`，并确保哈希函数、相等比较一致。

示例（自定义 comparator）：

```cpp
#include <map>
#include <string>
#include <iostream>

struct LenCmp {
    bool operator()(const std::string &a, const std::string &b) const {
        return a.size() < b.size();
    }
};

int main() {
    std::map<std::string, int, LenCmp> m;
    m["ab"] = 1;
    m["abc"] = 2;
    // 迭代将按字符串长度排序
}
```

示例（unordered_map 自定义 hash/eq）：

```cpp
#include <unordered_map>
#include <string>
#include <iostream>

struct MyHash {
    size_t operator()(const std::string &s) const noexcept {
        // 简单示例：std::hash 的组合或其他自定义哈希
        return std::hash<std::string>{}(s);
    }
};

struct MyEq {
    bool operator()(const std::string &a, const std::string &b) const noexcept {
        return a == b;
    }
};

int main() {
    std::unordered_map<std::string, int, MyHash, MyEq> um;
    um["k"] = 42;
}
```

---

## 节点操作（C++17）

可以使用 `extract` 从 map 中提取节点（node handle），并用 `insert` 在同一或另一容器中插入而不重新分配/构造键和 value（转移内部节点），对高性能移动非常有用。

示例：

```cpp
#include <map>
#include <iostream>

int main() {
    std::map<int, std::string> m1{{1, "a"}, {2, "b"}};
    std::map<int, std::string> m2{{3, "c"}};

    auto node = m1.extract(1); // 提取键 1 的节点
    if (node) m2.insert(std::move(node));
    // 此操作不会重新构造 value
}
```

---

## 迭代器失效规则

`std::map`/`multimap`: 插入不使现有迭代器失效；erase 只使被删除的迭代器失效；范围 erase 使被移除的迭代器失效。

`std::unordered_map`/`unordered_multimap`: `rehash` 会使所有迭代器失效；插入在触发 rehash 时亦会失效；erase 只影响被删除元素的迭代器。

---

## 常见陷阱与建议

- 使用 `operator[]` 时会创建默认值：如果你只想查询是否存在请使用 `find()` 或 `contains()`（C++20）。
- 对于频繁大小变化或性能敏感场景，优先使用 `reserve()`（`unordered_map`）或考虑 `flat_map`（非标准，存储为排序的数组，适合小到中等规模且写操作较少的场景）。
- 当 key 类型为自定义类型时，确保实现合适的 `<` （用于 `map`）或 `hash` & `==`（用于 `unordered_map`）。
- `std::map` 的插入/删除稳定、按顺序遍历；`std::unordered_map` 在平均水平上更快，但不保证顺序。选择时权衡排序需求与性能需求。
- 可用 `try_emplace` 或 `emplace` 避免不必要的对象拷贝/构造。

---

## 示例汇总：有序/无序 map 常用场景

```cpp
#include <map>
#include <unordered_map>
#include <iostream>

int main() {
    std::map<int, std::string> ordered;
    ordered[2] = "two";
    ordered[1] = "one";
    for (auto &p : ordered) std::cout << p.first << ':' << p.second << '\n'; // 输出 1:one 2:two

    std::unordered_map<std::string, int> um;
    um.reserve(100);
    um.emplace("apple", 3);
    um.try_emplace("banana", 2);
    if (auto it = um.find("apple"); it != um.end()) std::cout << it->first << ':' << it->second << '\n';
}
```

---

## 参考

- cppreference: `std::map` / `std::unordered_map`



# std::map / std::unordered_map 使用指南

本文档介绍 C++ 标准库中常用的映射容器：`std::map`, `std::multimap`, `std::unordered_map`, `std::unordered_multimap`。
内容包括 API 列表、复杂度说明、示例代码、性能建议、以及常见陷阱。

---

## 概要

`std::map<Key, T, Compare>`：基于平衡二叉树（一般为红黑树）的有序关联容器；按 key 排序；key 唯一。

`std::multimap<Key, T, Compare>`：允许重复 key 的有序关联容器。

`std::unordered_map<Key, T, Hash, KeyEqual>`：基于哈希表的无序关联容器；平均 O(1) 查找/插入；key 唯一。

`std::unordered_multimap<Key, T, Hash, KeyEqual>`：多重键版本，允许重复 key。

选择基础：需要按 key 排序或范围查询请选择 `std::map`；需要最快的平均查找请选择 `std::unordered_map`（并注意内存和哈希成本）。

---

## 常用操作

构造：`map<Key,T> m;`、`map m = { {k1, v1}, {k2,v2}};`

访问：`operator[]`, `at(key)`, `find(key)`, `count(key)`（map 为 0/1）、`contains(key)`（C++20）

插入：`insert({k,v})`、`insert(hint, ...)`、`emplace(...)`、`try_emplace(key, args...)`（C++17）

替换/更新：`m[key] = new_value;` 或 `it->second = new_value;`

删除：`erase(key)`、`erase(it)`、`erase(range)`，返回值行为视 API 而定

查询范围与边界：`lower_bound`, `upper_bound`, `equal_range`（multimap）

示例：

```cpp
#include <map>
#include <iostream>

int main() {
    std::map<std::string, int> m;
    m["one"] = 1;                 // 插入或更新
    m.insert({"two", 2});         // 插入
    m.emplace("three", 3);        // 直接构造

    auto it = m.find("two");     // find 返回 iterator
    if (it != m.end()) std::cout << it->first << ' ' << it->second << '\n';


    ## 参考

    - cppreference: `std::map` / `std::unordered_map`
    m.insert({1, "a"});
    m.emplace(2, "b");
    m.try_emplace(3, "c");       // 仅当 3 不存在时插入
    m.insert_or_assign(2, "B");   // 将 2 的值更新为 "B"

    auto result = m.insert({3, "x"}); // 插入失败（已存在）
    if (!result.second) std::cout << "key 3 already exists\n";
}
```

---

## 多重映射（multimap）与范围查询

`std::multimap` 允许相同 key 重复。常用 API: `equal_range(key)` 返回 [lower_bound, upper_bound) 区间。

示例：

```cpp
#include <map>
#include <iostream>

int main() {
    std::multimap<int, std::string> mm;
    mm.emplace(1, "a"); mm.emplace(1, "b"); mm.emplace(2, "c");

    auto range = mm.equal_range(1);
    for (auto it = range.first; it != range.second; ++it) std::cout << it->second << '\n';
}
```

---

## 自定义 Key / 比较器 / hash

std::map: 通过第三个模板参数指定比较器：`std::map<Key, T, Compare>`，`Compare` 必须是严格弱序。

std::unordered_map: 通过第二和第三模板参数指定 `Hash` 与 `KeyEqual`，并确保哈希函数、相等比较一致。

示例（自定义 comparator）：

```cpp
#include <map>
#include <string>
#include <iostream>

struct LenCmp {
    bool operator()(const std::string &a, const std::string &b) const {
        return a.size() < b.size();
    }
};

int main() {
    std::map<std::string, int, LenCmp> m;
    m["ab"] = 1;
    m["abc"] = 2;
    // 迭代将按字符串长度排序
}
```

示例（unordered_map 自定义 hash/eq）：

```cpp
#include <unordered_map>
#include <string>
#include <iostream>

struct MyHash {
    size_t operator()(const std::string &s) const noexcept {
        // 简单示例：std::hash 的组合或其他自定义哈希
        return std::hash<std::string>{}(s);
    }
};

struct MyEq {
    bool operator()(const std::string &a, const std::string &b) const noexcept {
        return a == b;
    }
};

int main() {
    std::unordered_map<std::string, int, MyHash, MyEq> um;
    um["k"] = 42;
}
```

---

## 节点操作（C++17）

可以使用 `extract` 从 map 中提取节点（node handle），并用 `insert` 在同一或另一容器中插入而不重新分配/构造键和 value（转移内部节点），对高性能移动非常有用。

示例：

```cpp
#include <map>
#include <iostream>

int main() {
    std::map<int, std::string> m1{{1, "a"}, {2, "b"}};
    std::map<int, std::string> m2{{3, "c"}};

    auto node = m1.extract(1); // 提取键 1 的节点
    if (node) m2.insert(std::move(node));
    // 此操作不会重新构造 value
}
```

---

## 迭代器失效规则

`std::map`/`multimap`: 插入不使现有迭代器失效；erase 只使被删除的迭代器失效；范围 erase 使被移除的迭代器失效。

`std::unordered_map`/`unordered_multimap`: `rehash` 会使所有迭代器失效；插入在触发 rehash 时亦会失效；erase 只影响被删除元素的迭代器。

---

## 常见陷阱与建议

- 使用 `operator[]` 时会创建默认值：如果你只想查询是否存在请使用 `find()` 或 `contains()`（C++20）。
- 对于频繁大小变化或性能敏感场景，优先使用 `reserve()`（`unordered_map`）或考虑 `flat_map`（非标准，存储为排序的数组，适合小到中等规模且写操作较少的场景）。
- 当 key 类型为自定义类型时，确保实现合适的 `<` （用于 `map`）或 `hash` & `==`（用于 `unordered_map`）。
- `std::map` 的插入/删除稳定、按顺序遍历；`std::unordered_map` 在平均水平上更快，但不保证顺序。选择时权衡排序需求与性能需求。
- 可用 `try_emplace` 或 `emplace` 避免不必要的对象拷贝/构造。

---

## 示例汇总：有序/无序 map 常用场景

```cpp
#include <map>
#include <unordered_map>
#include <iostream>

int main() {
    std::map<int, std::string> ordered;
    ordered[2] = "two";
    ordered[1] = "one";
    for (auto &p : ordered) std::cout << p.first << ':' << p.second << '\n'; // 输出 1:one 2:two

    std::unordered_map<std::string, int> um;
    um.reserve(100);
    um.emplace("apple", 3);
    um.try_emplace("banana", 2);
    if (auto it = um.find("apple"); it != um.end()) std::cout << it->first << ':' << it->second << '\n';
}
```

---

## 参考

- cppreference: `std::map` / `std::unordered_map`



# std::map / std::unordered_map 使用指南

本文档介绍 C++ 标准库中常用的映射容器：`std::map`, `std::multimap`, `std::unordered_map`, `std::unordered_multimap`。
内容包括 API 列表、复杂度说明、示例代码、性能建议、以及常见陷阱。

---

## 概要

`std::map<Key, T, Compare>`：基于平衡二叉树（一般为红黑树）的有序关联容器；按 key 排序；key 唯一。

`std::multimap<Key, T, Compare>`：允许重复 key 的有序关联容器。

`std::unordered_map<Key, T, Hash, KeyEqual>`：基于哈希表的无序关联容器；平均 O(1) 查找/插入；key 唯一。

`std::unordered_multimap<Key, T, Hash, KeyEqual>`：多重键版本，允许重复 key。

选择基础：需要按 key 排序或范围查询请选择 `std::map`；需要最快的平均查找请选择 `std::unordered_map`（并注意内存和哈希成本）。

---

## 常用操作

构造：`map<Key,T> m;`、`map m = { {k1, v1}, {k2,v2}};`

访问：`operator[]`, `at(key)`, `find(key)`, `count(key)`（map 为 0/1）、`contains(key)`（C++20）

插入：`insert({k,v})`、`insert(hint, ...)`、`emplace(...)`、`try_emplace(key, args...)`（C++17）

替换/更新：`m[key] = new_value;` 或 `it->second = new_value;`

删除：`erase(key)`、`erase(it)`、`erase(range)`，返回值行为视 API 而定

查询范围与边界：`lower_bound`, `upper_bound`, `equal_range`（multimap）

示例：

```cpp
#include <map>
#include <iostream>

int main() {
    std::map<std::string, int> m;
    m["one"] = 1;                 // 插入或更新
    m.insert({"two", 2});         // 插入
    m.emplace("three", 3);        // 直接构造

    auto it = m.find("two");     // find 返回 iterator
    if (it != m.end()) std::cout << it->first << ' ' << it->second << '\n';

    // 遍历（map 按 key 排序）
    for (auto &p : m) std::cout << p.first << ':' << p.second << '\n';
}
```

---

## map 与 unordered_map 的复杂度与特性

`std::map`: 插入/查找/删除 都为 O(log n)，按 key 排序，迭代器稳定：插入不会使其他迭代器失效；erase 只失效被删除的迭代器。

`std::unordered_map`: 平均 O(1)，但最坏 O(n)（哈希冲突）；重哈希（rehash）会使所有迭代器失效；插入可能触发 rehash。

额外的哈希相关 API:

- `reserve(n)`：为至少存放 n 个元素做准备（有助于减少 rehash 次数）
- `rehash(b)`：调整桶数为至少 b
- `max_load_factor()` / `load_factor()`：控制和查询哈希表装载因子

示例：

```cpp
#include <unordered_map>
#include <iostream>

int main() {
    std::unordered_map<std::string, int> um;
    um.reserve(100);
    um["apple"] = 3;
    if (um.find("apple") != um.end()) std::cout << "found" << '\n';
}
```

---

## 插入 API 比较（insert / emplace / try_emplace / operator[]）

`insert(pair)`：若键已存在，插入失败且返回包含 map.end() 的 pair。

`emplace(args...)`：通过构造函数参数直接创建元素并插入，避免临时对象 (perfect-forwarding)。

`try_emplace(key, args...)`（C++17）：仅当键不存在时进行构造与插入，避免不必要的对象构造或拷贝。

`operator[]`：若键不存在，会默认构造值并插入（对 unordered_map、map 都可），因此仅用于 "读-写" 场景而不是只读查找。

`insert_or_assign`（C++17）：若键不存在插入，否则替换 value。

示例：

```cpp
#include <map>
#include <iostream>

int main() {
    std::map<int, std::string> m;
    m.insert({1, "a"});
    m.emplace(2, "b");
    m.try_emplace(3, "c");       // 仅当 3 不存在时插入
    m.insert_or_assign(2, "B");   // 将 2 的值更新为 "B"

    auto result = m.insert({3, "x"}); // 插入失败（已存在）
    if (!result.second) std::cout << "key 3 already exists\n";
}
```

---

## 多重映射（multimap）与范围查询

`std::multimap` 允许相同 key 重复。常用 API: `equal_range(key)` 返回 [lower_bound, upper_bound) 区间。

示例：

```cpp
#include <map>
#include <iostream>

int main() {
    std::multimap<int, std::string> mm;
    mm.emplace(1, "a"); mm.emplace(1, "b"); mm.emplace(2, "c");

    auto range = mm.equal_range(1);
    for (auto it = range.first; it != range.second; ++it) std::cout << it->second << '\n';
}
```

---

## 自定义 Key / 比较器 / hash

std::map: 通过第三个模板参数指定比较器：`std::map<Key, T, Compare>`，`Compare` 必须是严格弱序。

std::unordered_map: 通过第二和第三模板参数指定 `Hash` 与 `KeyEqual`，并确保哈希函数、相等比较一致。

示例（自定义 comparator）：

```cpp
#include <map>
#include <string>
#include <iostream>

struct LenCmp {
    bool operator()(const std::string &a, const std::string &b) const {
        return a.size() < b.size();
    }
};

int main() {
    std::map<std::string, int, LenCmp> m;
    m["ab"] = 1;
    m["abc"] = 2;
    // 迭代将按字符串长度排序
}
```

示例（unordered_map 自定义 hash/eq）：

```cpp
#include <unordered_map>
#include <string>
#include <iostream>

struct MyHash {
    size_t operator()(const std::string &s) const noexcept {
        // 简单示例：std::hash 的组合或其他自定义哈希
        return std::hash<std::string>{}(s);
    }
};

struct MyEq {
    bool operator()(const std::string &a, const std::string &b) const noexcept {
        return a == b;
    }
};

int main() {
    std::unordered_map<std::string, int, MyHash, MyEq> um;
    um["k"] = 42;
}
```

---

## 节点操作（C++17）

可以使用 `extract` 从 map 中提取节点（node handle），并用 `insert` 在同一或另一容器中插入而不重新分配/构造键和 value（转移内部节点），对高性能移动非常有用。

示例：

```cpp
#include <map>
#include <iostream>

int main() {
    std::map<int, std::string> m1{{1, "a"}, {2, "b"}};
    std::map<int, std::string> m2{{3, "c"}};

    auto node = m1.extract(1); // 提取键 1 的节点
    if (node) m2.insert(std::move(node));
    // 此操作不会重新构造 value
}
```

---

## 迭代器失效规则

`std::map`/`multimap`: 插入不使现有迭代器失效；erase 只使被删除的迭代器失效；范围 erase 使被移除的迭代器失效。

`std::unordered_map`/`unordered_multimap`: `rehash` 会使所有迭代器失效；插入在触发 rehash 时亦会失效；erase 只影响被删除元素的迭代器。

---

## 常见陷阱与建议

- 使用 `operator[]` 时会创建默认值：如果你只想查询是否存在请使用 `find()` 或 `contains()`（C++20）。
- 对于频繁大小变化或性能敏感场景，优先使用 `reserve()`（`unordered_map`）或考虑 `flat_map`（非标准，存储为排序的数组，适合小到中等规模且写操作较少的场景）。
- 当 key 类型为自定义类型时，确保实现合适的 `<` （用于 `map`）或 `hash` & `==`（用于 `unordered_map`）。
- `std::map` 的插入/删除稳定、按顺序遍历；`std::unordered_map` 在平均水平上更快，但不保证顺序。选择时权衡排序需求与性能需求。
- 可用 `try_emplace` 或 `emplace` 避免不必要的对象拷贝/构造。

---

## 示例汇总：有序/无序 map 常用场景

```cpp
#include <map>
#include <unordered_map>
#include <iostream>

int main() {
    std::map<int, std::string> ordered;
    ordered[2] = "two";
    ordered[1] = "one";
    for (auto &p : ordered) std::cout << p.first << ':' << p.second << '\n'; // 输出 1:one 2:two

    std::unordered_map<std::string, int> um;
    um.reserve(100);
    um.emplace("apple", 3);
    um.try_emplace("banana", 2);
    if (auto it = um.find("apple"); it != um.end()) std::cout << it->first << ':' << it->second << '\n';
}
```

---

## 参考

- cppreference: `std::map` / `std::unordered_map`


# std::map / std::unordered_map 使用指南

本文档介绍 C++ 标准库中常用的映射容器：`std::map`, `std::multimap`, `std::unordered_map`, `std::unordered_multimap`。
内容包括 API 列表、复杂度说明、示例代码、性能建议、以及常见陷阱。

- ## 概要

- `std::map<Key, T, Compare>`：基于平衡二叉树（一般为红黑树）的有序关联容器；按 key 排序；key 唯一。
- `std::multimap<Key, T, Compare>`：允许重复 key 的有序关联容器。
- `std::unordered_map<Key, T, Hash, KeyEqual>`：基于哈希表的无序关联容器；平均 O(1) 查找/插入；key 唯一。
- `std::unordered_multimap<Key, T, Hash, KeyEqual>`：多重键版本，允许重复 key。

选择基础：需要按 key 排序或范围查询请选择 `std::map`；需要最快的平均查找请选择 `std::unordered_map`（并注意内存和哈希成本）。

- ## 常用操作

- 构造：`map<Key,T> m;`、`map m = { {k1, v1}, {k2,v2}};`
- 访问：`operator[]`, `at(key)`, `find(key)`, `count(key)`（map 为 0/1）、`contains(key)`（C++20）
- 插入：`insert({k,v})`、`insert(hint, ...)`、`emplace(...)`、`try_emplace(key, args...)`（C++17）
- 替换/更新：`m[key] = new_value;` 或 `it->second = new_value;`
- 删除：`erase(key)`、`erase(it)`、`erase(range)`，返回值行为视 API 而定
- 查询范围与边界：`lower_bound`, `upper_bound`, `equal_range`（multimap）

示例：

```cpp
#include <map>
#include <iostream>

int main() {
	std::map<std::string, int> m;
	m["one"] = 1;                 // 插入或更新
	m.insert({"two", 2});         // 插入
	m.emplace("three", 3);        // 直接构造

	auto it = m.find("two");     // find 返回 iterator
	if (it != m.end()) std::cout << it->first << ' ' << it->second << '\n';

	// 遍历（map 按 key 排序）
	for (auto &p : m) std::cout << p.first << ':' << p.second << '\n';
}
```

## map 与 unordered_map 的复杂度与特性
- `std::map`: 插入/查找/删除 都为 O(log n)，按 key 排序，迭代器稳定：插入不会使其他迭代器失效；erase 只失效被删除的迭代器。
- `std::unordered_map`: 平均 O(1)，但最坏 O(n)（哈希冲突）；重哈希（rehash）会使所有迭代器失效；插入可能触发 rehash。

额外的哈希相关 API:
- `reserve(n)`：为至少存放 n 个元素做准备（有助于减少 rehash 次数）
- `rehash(b)`：调整桶数为至少 b
- `max_load_factor()` / `load_factor()`：控制和查询哈希表装载因子

示例：

```cpp
#include <unordered_map>
#include <iostream>

int main() {
	std::unordered_map<std::string, int> um;
	um.reserve(100);
	um["apple"] = 3;
	if (um.find("apple") != um.end()) std::cout << "found" << '\n';
}
```

## 插入 API 比较（insert / emplace / try_emplace / operator[]）
- `insert(pair)`：若键已存在，插入失败且返回包含 map.end() 的 pair。
- `emplace(args...)`：通过构造函数参数直接创建元素并插入，避免临时对象 (perfect-forwarding)。
- `try_emplace(key, args...)`（C++17）：仅当键不存在时进行构造与插入，避免不必要的对象构造或拷贝。
- `operator[]`：若键不存在，会默认构造值并插入（对 unordered_map、map 都可），因此仅用于 "读-写" 场景而不是只读查找。
- `insert_or_assign`（C++17）：若键不存在插入，否则替换 value。

示例：

```cpp
#include <map>
#include <iostream>

int main() {
	std::map<int, std::string> m;
	m.insert({1, "a"});
	m.emplace(2, "b");
	m.try_emplace(3, "c");       // 仅当 3 不存在时插入
	m.insert_or_assign(2, "B");   // 将 2 的值更新为 "B"

	auto result = m.insert({3, "x"}); // 插入失败（已存在）
	if (!result.second) std::cout << "key 3 already exists\n";
}
```

## 多重映射（multimap）与范围查询
- `std::multimap` 允许相同 key 重复。常用 API: `equal_range(key)` 返回 [lower_bound, upper_bound) 区间。

示例：

```cpp
#include <map>
#include <iostream>

int main() {
	std::multimap<int, std::string> mm;
	mm.emplace(1, "a"); mm.emplace(1, "b"); mm.emplace(2, "c");

	auto range = mm.equal_range(1);
	for (auto it = range.first; it != range.second; ++it) std::cout << it->second << '\n';
}
```

## 自定义 Key / 比较器 / hash
- std::map: 通过第三个模板参数指定比较器：`std::map<Key, T, Compare>`，`Compare` 必须是严格弱序。
- std::unordered_map: 通过第二和第三模板参数指定 `Hash` 与 `KeyEqual`，并确保哈希函数、相等比较一致。

示例（自定义 comparator）：

```cpp
#include <map>
#include <string>
#include <iostream>

struct LenCmp {
	bool operator()(const std::string &a, const std::string &b) const {
		return a.size() < b.size();
	}
};

int main() {
	std::map<std::string, int, LenCmp> m;
	m["ab"] = 1;
	m["abc"] = 2;
	// 迭代将按字符串长度排序
}
```

示例（unordered_map 自定义 hash/eq）：

```cpp
#include <unordered_map>
#include <string>
#include <iostream>

struct MyHash {
	size_t operator()(const std::string &s) const noexcept {
		// 简单示例：std::hash 的组合或其他自定义哈希
		return std::hash<std::string>{}(s);
	}
};

struct MyEq {
	bool operator()(const std::string &a, const std::string &b) const noexcept {
		return a == b;
	}
};

int main() {
	std::unordered_map<std::string, int, MyHash, MyEq> um;
	um["k"] = 42;
}
```

## 节点操作（C++17）
- 可以使用 `extract` 从 map 中提取节点（node handle），并用 `insert` 在同一或另一容器中插入而不重新分配/构造键和 value（转移内部节点），对高性能移动非常有用。

示例：

```cpp
#include <map>
#include <iostream>

int main() {
	std::map<int, std::string> m1{{1, "a"}, {2, "b"}};
	std::map<int, std::string> m2{{3, "c"}};

	auto node = m1.extract(1); // 提取键 1 的节点
	if (node) m2.insert(std::move(node));
	// 此操作不会重新构造 value
}
```

## 迭代器失效规则
- `std::map`/`multimap`: 插入不会使现有迭代器失效；erase 只使被删除的迭代器失效；范围 erase 使被移除的迭代器失效。
- `std::unordered_map`/`unordered_multimap`: `rehash` 会使所有迭代器失效；插入在触发 rehash 时亦会失效；erase 只影响被删除元素的迭代器。

## 常见陷阱与建议
- 使用 `operator[]` 时会创建默认值：如果你只想查询是否存在请使用 `find()` 或 `contains()`（C++20）。
- 对于频繁大小变化或性能敏感场景，优先使用 `reserve()`（`unordered_map`）或考虑 `flat_map`（非标准，存储为排序的数组，适合小到中等规模且写操作较少的场景）。
- 当 key 类型为自定义类型时，确保实现合适的 `<` （用于 `map`）或 `hash` & `==`（用于 `unordered_map`）。
- `std::map` 的插入/删除稳定、按顺序遍历；`std::unordered_map` 在平均水平上更快，但不保证顺序。选择时权衡排序需求与性能需求。
- 可用 `try_emplace` 或 `emplace` 避免不必要的对象拷贝/构造。

---

## 示例汇总：有序/无序 map 常用场景
```cpp
#include <map>
#include <unordered_map>
#include <iostream>

int main() {
	std::map<int, std::string> ordered;
	ordered[2] = "two";
	ordered[1] = "one";
	for (auto &p : ordered) std::cout << p.first << ':' << p.second << '\n'; // 输出 1:one 2:two

	std::unordered_map<std::string, int> um;
	um.reserve(100);
	um.emplace("apple", 3);
	um.try_emplace("banana", 2);
	if (auto it = um.find("apple"); it != um.end()) std::cout << it->first << ':' << it->second << '\n';
}
```

---

## 参考
- cppreference: `std::map` / `std::unordered_map`

