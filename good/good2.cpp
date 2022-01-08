#include <iostream>
#include <cmath>
#include <ctime>
using namespace std;

typedef __uint128_t Data;
const Data N=1e7;
Data mem[N];
Data f(Data n){ // n以内好数的个数
    if(n<=1) return 0;
    if(n<N&&mem[n]) return mem[n];
    Data ans=0;
    for(int k=2;;k++){
        Data t=pow(n,1.0/k);
        if(t<2) break;
        ans+=t-f(t)-1;
    }
    if(n<N) mem[n]=ans;
    return ans;
}
void print(Data x){
    char s[100], *t=s;
    do{
        *(++t)=char(x%10)+'0';
        x/=10;
    }while(x);
    for(;t!=s;t--) putchar(*t);
}
int main(){
    int x=clock();
    print(f(-1)); cout<<endl;
    cout<<double(clock()-x)/CLOCKS_PER_SEC<<endl;
    return 0;
}