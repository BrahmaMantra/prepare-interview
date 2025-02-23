## ACID
ACID — 数据库事务正确执行的四个基本要素,包含：原子性（Atomicity）、一致性（Consistency）、隔离性（Isolation）、持久性（Durability）。

一个支持事务（Transaction）中的数据库系统，必需要具有这四种特性，否则在事务过程（Transaction processing）当中无法保证数据的正确性，交易过程极可能达不到交易。

- 注意：当Connection遇到一个未处理的SQLException时，系统会非正常退出，事务也会自动回滚，但如果程序捕获到了异常，是需要在catch中显式回滚事务的(也就是不要commit)
### 脏读：一个事务读取到另外一个事务未commit的数据
例子：A向B转账，A执行了转账语句，但A还没有提交事务，B读取数据，发现自己账户钱变多了！B跟A说，我已经收到钱了。A回滚事务【rollback】，等B再查看账户的钱时，发现钱并没有多。

### 不可重复读：在一个transcatioon内多次读取同一个数据，但是前后两次读到的数据不一样

注：A查询数据库得到数据，B去修改数据库的数据，导致A多次查询数据库的结果都不一样【危害：A每次查询的结果都是受B的影响的，那么A查询出来的信息就没有意思了】

### 虚读(幻读)：是指在一个transaction内多次查询某条件的【记录数量】不一致
注：和不可重复读类似，但虚读(幻读)会读到其他事务的插入的数据，导致前后读取不一致

简单总结：脏读是不可容忍的，不可重复读和虚读在一定的情况下是可以的【做统计的肯定就不行】。
### 事务隔离级别
数据库定义了4个隔离级别：
1. Serializable【可避免脏读，不可重复读，幻读】
2. Repeatable read【可避免脏读，不可重复读】 （InodeDB默认）
3. Read committed【可避免脏读】
4. Read uncommitted【级别最低，什么都避免不了】

分别对应Connection类中的4个常量
1. TRANSACTION_READ_UNCOMMITTED
2. TRANSACTION_READ_COMMITTED
3. TRANSACTION_REPEATABLE_READ
4. TRANSACTION_SERIALIZABLE

### 关于repeatable read
Mysql在此隔离级别下，可以“很大程度”避免幻读的发生，解决办法有以下两种:
- 针对快照读（普通select语句），是通过MVCC方式解决了幻读，因为repeatable read隔离级别下，transcation执行过程中看到的数据，一直和事务启动时看到的数据是一致的，即使中途有别的事务插入也查询不到刚插入的数据。
- 针对当前读（select...for update），通过 next-key lock(记录锁+间隙锁)方式解决了幻读。因为当执行 select...for update 语句的时候，会加上 next-key lock ,如果有其它事务在此锁内插入了一条记录，那么这个插入语句就会阻塞，无法成功插入