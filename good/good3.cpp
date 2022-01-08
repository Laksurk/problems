#include <iostream>
#include <cmath>
#include <ctime>
#include <cstring>
#include "bigint.h"
using namespace std;

const int N = 1e7, M = 10000;
bool fmem[N];
ull mem[N];
int prm[M], cntp = 0;
int maxd[M];
ull f1(ull n) {
    if(n < N && fmem[n]) return mem[n];
    ull ans = 0;
    for(int k = 2;; k++) {
        ull t = pow(n, 1.0 / k) + 1e-6;
        if(t < 2) break;
        ans += t - f1(t) - 1;
    }
    if(n < N) { fmem[n] = true; mem[n] = ans; }
    return ans;
}
BigUint roots[M];
BigUint f2(BigUint n) {
    if(n.size() <= 2) return f1(n.uint());
    BigUint ans = 0, root;
    int maxk = n.cntbit();
    for(int k = 2; k <= maxk; k++) {
        if(!maxd[k]) root = n.root(k);
        else root = roots[maxd[k]].root(k / maxd[k]);
        if(root.size() == 1 && root.uint() <= 2) {
            ans += maxk - k + 1;
            break;
        }
        ans += root - f2(root) - 1;
        roots[k] = root;
    }
    return ans;
}
int main() {
    const int np = 100;
    const BigUint n = BigUint(10).pow(np);
    cout << "n = 10**" << np << endl;
    int x = clock();

    const int maxi = n.cntbit();
    for(int i = 2; i <= maxi; i++) {
        if(!maxd[i]) prm[cntp++] = i;
        for(int j = 0; j < cntp && i * prm[j] <= maxi; j++) {
            maxd[i * prm[j]] = i;
            if(i % prm[j] == 0) break;
        }
    }
    fmem[0] = fmem[1] = true;
    cout << f2(n) << endl;

    cout << double(clock() - x) / CLOCKS_PER_SEC << " seconds" << endl;
    return 0;
}