#include <ctime>
#include "bigint.h"

const int N = 20000; // log2(10**3000) = 9965
int prm[N], cntp = 0;
int d[N], mu[N];
BigUint roots[N];
int main() {
    // freopen("1.txt", "w", stdout);
    int np; cin >> np;
    BigUint& n = roots[1];
    n = BigUint(10).pow(np);
    cout << "n = 10**" << np << endl;
    auto x = clock();

    const int maxi = n.cntbit();
    for(int i = 2; i <= maxi; i++) {
        if(!d[i]) { prm[cntp++] = i; d[i] = 1; mu[i] = -1; }
        for(int j = 0; j < cntp; j++) {
            int t = i * prm[j];
            if(t > maxi) break;
            d[t] = i;
            if(i % prm[j] == 0) { mu[t] = 0; break; }
            mu[t] = -mu[i];
        }
    }

    BigUint posi, neg;
    for(int i = 2; i <= maxi; i++) {
        if(!mu[i]) continue;
        // auto x = clock();
        roots[i] = roots[d[i]].root(i / d[i]);
        // auto y = clock();
        // cout << setw(3) << i << " " << setw(3) << y - x << endl;
        if(roots[i].size() == 1 && roots[i].uint() == 2) {
            int ans = 0;
            for(; i <= maxi; i++) ans += mu[i];
            posi -= ans;
            break;
        }
        if(mu[i] == -1) posi += roots[i] - 1;
        else neg += roots[i] - 1;
    }
    cout << posi - neg << endl;

    cout << double(clock() - x) / CLOCKS_PER_SEC << " seconds" << endl;
    return 0;
}