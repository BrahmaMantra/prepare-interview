#include <iostream>

class Base {
public:
    virtual void print() {
        std::cout << "Base::print() at address " << (void*)&Base::print << std::endl;
    }
};

class Derived : public Base {
public:
    virtual void print() override {
        std::cout << "Derived::print() at address " << (void*)&Derived::print << std::endl;
    }
};

int main() {
    // 创建一个 Base 对象，用来打印父类的虚表地址
    Base baseObj;
    // 创建一个 Derived 对象
    Derived obj;
    
    // 获取父类对象的虚指针
    void** baseVtable = *(void***)&baseObj;
    // 获取子类对象的虚指针
    void** derivedVtable = *(void***)&obj;


    // 比较父类和子类的虚表地址
    if (baseVtable == derivedVtable) {
        std::cout << "The vtables are the same!" << std::endl;
    } else {
        std::cout << "The vtables are different!" << std::endl;
    }

     // 输出父类虚函数表的地址。
    std::cout << "Vtable for Base: " << baseVtable << std::endl;
    // 输出子类虚函数表的地址
    std::cout << "Vtable for Derived: " << derivedVtable << std::endl;


    // 输出父类虚函数表的第一个函数指针的地址。
    std::cout << "First Func in Vtable for Base: " << *baseVtable << std::endl;
    // 输出子类虚函数表第一个函数指针的地址。
    std::cout << "First Func in Vtable for Derived: " << *derivedVtable << std::endl;

    // 都转换为 Base* 类型，输出虚函数表的地址
    std::cout<<"*(reinterpret_cast<Base**>(baseVtable)) is:"<<*(reinterpret_cast<Base**>(baseVtable))<<std::endl;
    std::cout<<"*(reinterpret_cast<Base**>(derivedVtable)) is:"<<*(reinterpret_cast<Base**>(derivedVtable))<<std::endl;

    // 强行修改虚指针的地址，让它指向父类的虚函数表第一个函数指针的地址
    // 在这一步出现了Segmentation fault (core dumped)错误
    *(reinterpret_cast<Base**>(derivedVtable)) = *(reinterpret_cast<Base**>(baseVtable));
    std::cout << "First Func in Vtable for Derived: " << *derivedVtable << std::endl;
    obj.print();

    return 0;
}
