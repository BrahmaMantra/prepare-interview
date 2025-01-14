小Q的父母要出差N天，走之前给小Q留下了M块巧克力。小Q决定每天吃的巧克力数量不少于前一天吃的一半，但是他又不想在父母回来之前的某一天没有巧克力吃，请问他第一天最多能吃多少块巧克力

输入描述:

每个输入包含一个测试用例。
每个测试用例的第一行包含两个正整数，表示父母出差的天数N(N<=50000)和巧克力的数量M(N<=M<=100000)。


输出描述:

输出一个数表示小Q第一天最多能吃多少块巧克力。
示例1
输入

3 7
输出

4
~~~ cpp
#include <iostream>
using namespace std;

int find(int now_num,int days,int cho_num){
    for(int i=0;i<days;i++){
        cho_num -= now_num;
        now_num = now_num%2 ==0?now_num/2:now_num/2+1;
        if(cho_num < 0) return 0;
    }
    if(cho_num ==0) return 2;
    return 1;
}
int main() {
    int N, M;
    while (cin >> N >> M) { // 注意 while 处理多个 case
        int left =0,right = M;
        while(left <right){
            int mid = (left+right) >> 1;
            if(find(mid,N,M) == 2){
                left = mid;
                break;
            }
            else if(find(mid,N,M) == 1){
                left = mid+1;
            }else {
            right = mid;
            }
        }
        if(find(left,N,M)){
        cout <<left<< endl;
        }else {
        cout <<left-1<< endl;
        }
    }

}
~~~ 