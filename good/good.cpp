#include <iostream>
#include <cmath>
#include <vector>
using namespace std;

const int N=1e7;
long long ans[N];
vector<long long> vec;
long long f(long long n){
    vec.push_back(n);
    if(n<N&&ans[n]) return ans[n];
    if(n<=1) return 1;
    long long res=1;
    for(long long k=2;;k++){
        long long t=pow(n,1.0/k);
        if(t<2) break;
        res+=f(t);
    }
    if(n<N) ans[n]=res;
    return res;
}
int main(){
    cout<<f(1e8)<<endl;
    for(auto x:vec){
        cout<<"f("<<x<<") = "<<f(x)<<endl;
    }
    return 0;
}