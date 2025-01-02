#include <iostream>
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
    return 0;
}