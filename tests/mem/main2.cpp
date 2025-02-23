/*
栈上第一次分配int内存，会给四个页表的栈地址
堆上第一次分配int内存，会给16个页表的堆地址

这个和堆栈指针无关，堆栈指针也只能判断变量是否在已分配的内存中，不能判断变量是否被初始化
*/


#include <cstdint>
#include <cstdlib> // 包含 malloc_usable_size 的头文件
#include <fstream>
#include <iostream>
#include <malloc.h> // 包含 malloc_usable_size 的头文件
using namespace std;
void print_memory_map() {
    std::ifstream maps("/proc/self/maps");
    if (!maps.is_open()) {
        std::cerr << "Failed to open /proc/self/maps" << std::endl;
        return;
    }

    std::string line;
    while (std::getline(maps, line)) {
        std::cout << line << std::endl;
    }
}
void print_stack_pointer() {
  uintptr_t sp;
  asm("mov %%rsp, %0" : "=r"(sp));
  std::cout << "Stack pointer (RSP): " << std::hex << sp << std::endl;
}
void print_ptr(int *p) {
  cout << p << endl;
  cout << std::dec << *p << endl;
}
int main() {
  // print_memory_map();
  print_stack_pointer();

  int a=0;
  int *p = &a;
  int i=0;
// while(1){
//   cout<<i++<<endl;
//   print_ptr(p++);
// }
  int *b = new int(100); // 动态分配内存并初始化为 100
  p=b;
  // 获取分配的内存块的实际大小
while(1){
  cout<<i++<<endl;
  print_ptr(p++);
}
  // delete b; // 释放内存
  // print_ptr(b);
  int a1 = -11;
  cout<<(a1>>1)<<","<<(a1/2)<<endl;
  return 0;
}

/* 是的，变量 a 的内存已经被分配。
在 C++ 中，当你声明一个局部变量（例如 int
a;），编译器会在栈上为这个变量分配内存。因此，变量 a 的内存已经被分配，指针 p
也正确地指向了 a 的内存地址。

然而，虽然内存已经分配，但变量 a 没有被初始化。这意味着 a
的值是未定义的，可能包含任何垃圾值。因此，当你输出 *p 的值时，结果是不可预测的。
*/