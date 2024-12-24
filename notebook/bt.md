1. 回溯函数模板返回值以及参数(一般是void)

2. 回溯函数终止条件（if (...) return）

3. 回溯搜索的遍历过程(一般是一个for循环，push bt pop )

// 版本二
class Solution {
public:
    int climbStairs(int n) {
        if (n <= 1) return n;
        int dp[3];
        dp[1] = 1;
        dp[2] = 2;
        for (int i = 3; i <= n; i++) {
            int sum = dp[1] + dp[2];
            dp[1] = dp[2];
            dp[2] = sum;
        }
        return dp[2];
    }
};
