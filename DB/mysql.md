## Mysql单行不要超过2000w靠谱吗？
- 假设：
    - 非叶子节点内指向其他页的数量为 x
    - 叶子节点内能容纳的数据行数为 y
    - B+ 数的层数为 z
    - Total =x^(z-1) *y 也就是说总数会等于 x 的 z-1 次方 与 Y 的乘积。
  
- X=? 索引也也不例外，都会有 File Header (38 byte)、Page Header (56索引也也不例外，都会有 File Header (38 byte)、Page Header (56Byte)、Infimum + Supermum（26 byte）、File Trailer（8byte）, 再加上页目录，大概 1k 左右。

