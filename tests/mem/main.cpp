
#include <iostream>
#include <unordered_map>
#include <shared_mutex>
#include <mutex>
#include <cstdlib> // 包含 malloc_usable_size 的头文件
#include <malloc.h> // 包含 malloc_usable_size 的头文件
using namespace std;
struct Node {
  int key;
  int value;
  Node *next;
  Node(int key, int value) : key(key), value(value), next(nullptr) {}
};

struct LRUCache {
  int _capacity;
  unordered_map<int, Node *> _cache;
  int _size = 0;
  Node *_head; // 虚拟头结点
  std::shared_mutex cache_rw;
  std::mutex node_mutex;
public:
  LRUCache(int capacity) : _capacity(capacity) {
    _head = new Node(-1, -1);
    _capacity = capacity;
    _size = 0;
    _cache.clear();
  }
  // 只在找到了key的时候才会调用
  void move_to_head(Node *node) {
    // Node *node = _cache[node->key];

    // 将node移动到头部
    Node *prev = _head;
    while (prev->next != node) {
      prev = prev->next;
    }
    prev->next = node->next;
    node->next = _head->next;
    _head->next = node;
  }
  void delete_end() {
    Node *last = _head;
    while (last->next->next != nullptr) {
      last = last->next;
    }
    Node *tmp = last->next;
    last->next = nullptr;
    _cache.erase(tmp->key);
    _size--;
    delete tmp;
  }
  void insert_node(Node *node) {
    node->next = _head->next;
    _head->next = node;
    _cache[node->key] = node;
    _size++;
  }
  int get(int key) {
     std::shared_lock<std::shared_mutex> lock(cache_rw);
    if (_cache.find(key) != _cache.end()) {// compiler_fence
      std::lock_guard<std::mutex> lock(node_mutex);
      move_to_head(_cache[key]);
      return _cache[key]->value;
    } else {
      return -1;
    }
  } 
  void put(int key, int value) {
    std::unique_lock<std::shared_mutex> lock(cache_rw);
    std::lock_guard<std::mutex> lock2(node_mutex);
    // 如果key存在，更新value，将key移动到头部
    if (_cache.find(key) != _cache.end()) {
      _cache[key]->value = value;
      move_to_head(_cache[key]);
    } else {
      // 如果key不存在
      if (_size == _capacity) {
        // cache满了
        delete_end();
        Node *node = new Node(key, value);
        insert_node(node);
      } else {
        Node *node = new Node(key, value);
        insert_node(node);
      }
    }
  }
};
struct sharedPtr
{
    
};


int main() {
  LRUCache *cache = new LRUCache(2 /* 缓存容量 */);
    // 获取分配的内存块的实际大小
    size_t allocated_size = malloc_usable_size(cache);

    std::cout << "Requested size: " << sizeof(LRUCache) << " bytes" << std::endl;
    std::cout << "Allocated size: " << allocated_size << " bytes" << std::endl;
  cache->put(1, 1);
  cache->put(2, 2);
  cout<<cache->get(1)<<endl;;    // 返回 1
  cache->put(3, 3); // 该操作会使得关键字 2 作废
  cout<<cache->get(2)<<endl;    // 返回 -1 (未找到)
  cache->put(4, 4); // 该操作会使得关键字 1 作废
  cout<<cache->get(1)<<endl;    // 返回 -1 (未找到)
  cout<<cache->get(3)<<endl;    // 返回 3
  cout<<cache->get(4)<<endl;    // 返回 4
}