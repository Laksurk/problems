#include <iostream>
#include <cmath>
#include <cstring>
#include <iomanip>
#include <vector>
#include <algorithm>
using namespace std;

typedef unsigned long long ull;
class BigUint {
    typedef const BigUint& CR;
    vector<unsigned> vec;
    void remove0() {
        while(vec.size() > 1 && *vec.rbegin() == 0)
            vec.pop_back();
    }
public:
    BigUint(unsigned x = 0) : vec({x}) {}
    size_t size() const { return vec.size(); }
    unsigned uint() const { return *vec.begin(); }
    unsigned& uint() { return *vec.begin(); }
    ull uintll() const { return vec[0] | (size() == 1 ? 0 : ull(vec[1]) << 32); }
    unsigned cntbit() const {
        return __lg(*vec.rbegin()) + (size() - 1) * 32;
    }
    friend ostream& operator<<(ostream& os, CR bu) {
        int n = bu.vec.size();
        os << hex << bu.vec[n - 1];
        for(int i = n - 2; i >= 0; --i)
            os << setw(8) << setfill('0') << bu.vec[i];
        return os;
    }
    friend BigUint operator+(CR bu1, CR bu2) {
        BigUint bu3;
        int n = bu1.size(), m = bu2.size(), p = max(n, m);
        bu3.vec.insert(bu3.vec.end(), p, 0);
        auto a = &*bu1.vec.begin(), b = &*bu2.vec.begin();
        auto c = &*bu3.vec.begin();
        bool flag = false;
        for(int i = 0; i <= p; i++) {
            ull x = ull((i < n) * a[i]) + (i < m) * b[i] + flag;
            flag = x >> 32;
            c[i] = x;
        }
        bu3.remove0();
        return bu3;
    }
    friend BigUint operator-(CR bu1, CR bu2) {
        BigUint bu3;
        int n = bu1.size(), m = bu2.size();
        bu3.vec.insert(bu3.vec.end(), n - 1, 0);
        auto a = &*bu1.vec.begin(), b = &*bu2.vec.begin();
        auto c = &*bu3.vec.begin();
        bool flag = false;
        for(int i = 0; i < n; i++) {
            ull x = ull(a[i]) - (i < m) * b[i] - flag;
            flag = x >> 32;
            c[i] = x;
        }
        bu3.remove0();
        return bu3;
    }
    friend BigUint operator*(CR bu1, CR bu2) {
        BigUint bu3;
        int n = bu1.size(), m = bu2.size(), p = n + m;
        bu3.vec.insert(bu3.vec.end(), p - 1, 0);
        auto a = &*bu1.vec.begin(), b = &*bu2.vec.begin();
        auto c = &*bu3.vec.begin();
        for(int i = 0; i < n; i++) {
            ull flag = 0;
            for(int j = 0; j < m; j++) {
                flag += c[i + j] + ull(a[i]) * b[j];
                c[i + j] = flag;
                flag >>= 32;
            }
            c[i + m] += flag;
        }
        bu3.remove0();
        return bu3;
    }
    BigUint pow(unsigned k) {
        BigUint res(1), a(*this);
        for(; k; k /= 2) {
            if(k % 2) res *= a;
            a *= a;
        }
        return res;
    }
    void left(unsigned k) {
        vec.insert(vec.begin(), k, 0);
    }
    friend BigUint operator>>(CR bu, unsigned k) {
        BigUint bu2;
        int n = bu.size(), p = k / 32, q = k % 32, r = 32 - q;
        bu2.vec.insert(bu2.vec.end(), n - p - 1, 0);
        auto a = &*bu.vec.begin();
        auto b = &*bu2.vec.begin();
        for(int i = 0; i < n - p - 1; i++)
            b[i] = (a[i + p] >> q) | ((a[i + p + 1] & ((1u << q) - 1)) << r);
        b[n - p - 1] = a[n - 1] >> q;
        bu2.remove0();
        return bu2;
    }
    friend BigUint operator<<(BigUint bu, unsigned k) {
        if(k % 32 == 0) bu.left(k / 32);
        else {
            bu.left(k / 32 + 1);
            bu >>= 32 - k % 32;
        }
        return bu;
    }
    friend BigUint operator|(CR bu1, CR bu2) {
        BigUint bu3;
        int n = max(bu1.size(), bu2.size()), m = min(bu1.size(), bu2.size());
        bu3.vec.insert(bu3.vec.end(), n - 1, 0);
        auto a = &*bu1.vec.begin(), b = &*bu2.vec.begin();
        auto c = &*bu3.vec.begin();
        if(n != int(bu1.size())) swap(a, b);
        for(int i = 0; i < m; i++)
            c[i] = a[i] | b[i];
        memcpy(c + m, a + m, (n - m) * 4);
        return bu3;
    }
    BigUint sqrt() const {
        BigUint r, res;
        int n = size();
        auto a = &*vec.begin();
        for(int i = n - 1; i >= 0; i--) {
            for(int j = 30; j >= 0; j -= 2) {
                (r <<= 2).uint() |= (a[i] >> j) & 0b11;
                res <<= 1;
                BigUint d = res << 1;
                d.uint() |= 1;
                if(d <= r) {
                    r -= d;
                    res.uint() |= 1;
                }
            }
        }
        return res;
    }
    BigUint root(unsigned k) const {
        if(k == 2) return sqrt();
        int n = size();
        BigUint l(1), r(1);
        l.left((n - 1) / k);
        r.left((n - 1) / k + 1);
        while(l != r) {
            BigUint mid = (l + r + 1) >> 1;
            if(mid.pow(k) <= *this) l = mid;
            else r = mid - 1;
        }
        return l;
    }
    friend int comp(CR bu1, CR bu2) {
        int n = bu1.size(), m = bu2.size();
        if(n < m) return -1;
        if(n > m) return 1;
        auto a = &*bu1.vec.begin(), b = &*bu2.vec.begin();
        for(int i = n - 1; i >= 0; i--) {
            if(a[i] < b[i]) return -1;
            if(a[i] > b[i]) return 1;
        }
        return 0;
    }
    friend bool operator==(CR bu1, CR bu2) { return bu1.vec == bu2.vec; }
    friend bool operator!=(CR bu1, CR bu2) { return bu1.vec != bu2.vec; }
    friend bool operator< (CR bu1, CR bu2) { return comp(bu1, bu2) == -1; }
    friend bool operator<=(CR bu1, CR bu2) { return !(bu2 < bu1); }
    friend bool operator> (CR bu1, CR bu2) { return bu2 < bu1; }
    friend bool operator>=(CR bu1, CR bu2) { return bu2 <= bu1; }
    BigUint& operator+=(CR bu) { return *this = *this + bu; }
    BigUint& operator-=(CR bu) { return *this = *this - bu; }
    BigUint& operator*=(CR bu) { return *this = *this * bu; }
    BigUint& operator<<=(unsigned k) { return *this = *this << k; }
    BigUint& operator>>=(unsigned k) { return *this = *this >> k; }
    BigUint& operator|=(CR bu) { return *this = *this | bu; }
};

const int N = 1e7, M = 10000;
bool fmem[N];
ull mem[N];
int prm[M], cntp = 0;
int maxd[M];
BigUint roots[M];
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
BigUint f2(BigUint n) {
    if(n.size() <= 2) return f1(n.uintll());
    BigUint ans = 0, root;
    int maxk = n.cntbit();
    for(int k = 2; k <= maxk; k++) {
        // if(!maxd[k]) root = n.root(k);
        root = n.root(k);
        // else root = roots[maxd[k]].root(k / maxd[k]);
        if(root.size() == 1 && root.uint() <= 2) {
            ans += maxk - k + 1;
            break;
        }
        ans += root - f2(root) - 1;
    }
    return ans;
}
int main() {
    const int np = 100;
    const BigUint n = BigUint(10).pow(np);
    cout << "n = 10**" << np << endl;
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
    return 0;
}