
### 内存释放
在Linux内核中，内存管理涉及多种不同的分配和释放机制。以下是kfree、vfree、free_pages和kobject_put的区别和使用场景：
1. kfree
    - 用途：释放通过kmalloc、kzalloc、kmalloc_array等函数分配的内存。
    - 原型：void kfree(const void *ptr);
    - 使用场景：用于释放小块内存，通常用于内核模块和驱动程序中。
    - 示例：
    ~~~cpp
    void *ptr = kmalloc(1024, GFP_KERNEL);
    if (ptr) {
        // 使用分配的内存
        kfree(ptr); // 释放内存
    }
    ~~~

2. vfree
    - 用途：释放通过vmalloc、vzalloc等函数分配的虚拟内存。
    - 原型：void vfree(const void *addr);
    - 使用场景：用于释放较大块的连续虚拟内存，适用于需要大块内存但不要求物理连续的情况。
    - 示例：
    ~~~ cpp
    void *ptr = vmalloc(1024);
    if (ptr) {
        // 使用分配的内存
        vfree(ptr); // 释放内存
    }
    ~~~

3. free_pages
    - 用途：释放通过alloc_pages或__get_free_pages分配的页框。
    - 原型：void free_pages(unsigned long addr, unsigned int order);
    - 使用场景：用于释放物理上连续的页框，适用于需要直接操作物理内存的情况。
    - 示例：
    ~~~ cpp
    unsigned long addr = __get_free_pages(GFP_KERNEL, 0);
    if (addr) {
        // 使用分配的页框
        free_pages(addr, 0); // 释放页框
    }
    ~~~
4. kobject_put
    - 用途：释放通过kobject_create分配的内核对象。
    - 原型：void kobject_put(struct kobject *kobj);
    - 使用场景：用于管理内核对象的生命周期，通常与内核对象模型（kobject）相关联。
    - 示例：
    ~~~ cpp
    struct kobject *kobj = kobject_create();
    if (kobj) {
        // 使用内核对象
        kobject_put(kobj); // 释放内核对象
    }
    ~~~

#### Linux进程内存释放路线：
- [void __noreturn do_exit(long code)](https://code.dragonos.org.cn/xref/linux-6.6.21/kernel/exit.c#809)：这是进程的退出的主要函数，负责处理进程退出的**所有**细节
- [static void exit_mm(void)](https://code.dragonos.org.cn/xref/linux-6.6.21/kernel/exit.c#exit_mm):do_exit 函数会调用 exit_mm 函数来释放与进程的内存管理相关的资源。
- [void mmput(struct mm_struct *mm)](https://code.dragonos.org.cn/xref/linux-6.6.21/kernel/fork.c?fi=mmput#mmput):,linux6.6.21是将其封装到__mmput里面了
- [void __mmdrop(struct mm_struct *mm)](https://code.dragonos.org.cn/xref/linux-6.6.21/kernel/fork.c?fi=mmput#__mmput)：mmput 函数会调用 __mmdrop 函数来实际释放内存描述符。
- [mm_free_pgd(mm)](https://code.dragonos.org.cn/xref/linux-6.6.21/kernel/fork.c#801):释放页全局目录PGD 和 [free_mm(mm)](https://code.dragonos.org.cn/xref/linux-6.6.21/kernel/fork.c?fi=__mmdrop#free_mm)释放mm_struct本身

#### Dragonos进程内存释放：
通过实现Drop这个trait，让Arc计数器为0的时候自动调用drop析构，从而避免因引用不当造成的内存泄漏
- PCB里有一个structure InnerAddressSpace，通过其析构函数调用LockVMA的析构函数，清除内存的
- 
- 总结
    - kfree：用于释放通过kmalloc等函数分配的小块内存。
    - vfree：用于释放通过vmalloc等函数分配的大块虚拟内存。
    - free_pages：用于释放通过alloc_pages或__get_free_pages分配的物理连续页框。
    - kobject_put：用于释放通过kobject_create分配的内核对象，管理内核对象的生命周期。