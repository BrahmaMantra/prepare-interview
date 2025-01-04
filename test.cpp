#include <iostream>
#include <cstring>

void print_size(char* ptr) {
    std::cout << "Size of char* ptr: " << sizeof(ptr) << std::endl;
}

void print_size(char (*ptr)[1024]) {
    std::cout << "Size of char (*ptr)[1024]: " << sizeof(*ptr) << std::endl;
}

int main() {
    char buf[1024];
    bzero(buf, sizeof(buf));

    print_size(buf);   // 传递 char* 类型
    print_size(&buf);  // 传递 char (*)[1024] 类型

    return 0;
}