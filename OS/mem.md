
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

## 场景题
给你一个场景，在c++中，声明了一个int*p但是没有初始化，此时他的值是未定义的，但是你要执行`cout<<*p<<endl;` 的话，假设访问了一块未知空间0x????????，触发了一个page fault到内核，然后内核怎么判断这个空间是不是这个程序或线程可以访问的呢？（比如恰好落在自己的栈空间，其它线程的数据段亦或者哪里）

### 回答：
在C++中解引用未初始化的指针int* p时，访问的地址是随机的，可能触发页错误（Page Fault）。内核处理此错误的流程如下：
1. 触发页错误
当CPU访问虚拟地址0x????????时，MMU（极大概率）发现该地址未映射到物理内存或权限不足，触发页错误中断，CPU将控制权交给内核。
2. 内核检查地址有效性
- 内核通过以下步骤验证地址是否合法：
    - 确定地址是否属于用户空间：在大多数系统（如Linux）中，用户进程的虚拟地址空间分为：

        - 用户空间：例如，在x86-64中为0x0000000000000000到0x00007fffffffffff。

        - 内核空间：高位地址（如0xffff800000000000及以上）。

    - 若地址位于内核空间，用户态程序无权访问，直接判定为非法。

- 查找进程的虚拟内存区域（VMA）

    - 内核维护进程的虚拟内存描述结构（如Linux的vm_area_struct链表/红黑树），记录所有合法内存区域（代码段、堆、栈、内存映射等）及其权限（读/写/执行）。

    - 遍历VMA：检查0x????????是否落在任一VMA的范围内。若不在任何VMA中：地址无效，触发SIGSEGV（段错误），终止进程。若在VMA中：继续检查访问权限。

3. 检查访问权限

    - 若地址属于某个VMA，但当前操作（读/写/执行）不符合VMA记录的权限：
        - 例如，尝试写入只读的代码段，或执行非可执行栈。内核判定为权限错误，触发SIGSEGV。

4. 特殊情况的处理
- 若地址有效且权限匹配，内核可能进一步处理：

    - 未分配的物理页：分配物理内存并建立映射（如访问未初始化的堆内存）。

    - 写时复制（Copy-on-Write）：若页面标记为COW（如fork后的父子进程共享内存），内核复制页面并更新映射。

    - 按需分页（Demand Paging）：加载文件内容到内存（如动态库的懒加载）。