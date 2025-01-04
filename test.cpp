#include <iostream>
<<<<<<< HEAD
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

=======
using namespace std;
#include <memory>

struct A {
    int i;
};

struct B {
    char ch;
};

struct C {
    double d;
    virtual int fn() { return 0; }
};

struct D :public A, public B, public C {
    int i;
};


int main() {
    cout << "Size of struct A: " << sizeof(A) << endl;
    cout << "Size of struct B: " << sizeof(B) << endl;
    cout << "Size of struct C: " << sizeof(C) << endl;
    std::shared_ptr<int> ptr;
>>>>>>> 9486fa6d000dbd8a19db97d3b796612707e9e988
    return 0;
}