#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <bitset>
using namespace std;

typedef unsigned long long ull;
class BigUint {
    typedef const BigUint& CR;
    vector<unsigned> vec;
    void remove0() {
        while(vec.size() > 1 && *vec.rbegin() == 0)
            vec.pop_back();
    }
    const static int N = 20;
    static ull C[N + 1][N + 1];
    static BigUint pw[N];
public:
    BigUint(unsigned x = 0) : vec({x}) {}
    size_t size() const { return vec.size(); }
    unsigned uint() const { return *vec.begin(); }
    unsigned& uint() { return *vec.begin(); }
    bool getbit(int i) const {
        return (i / 32 < int(size())) * vec[i / 32] & (1 << i % 32);
    }
    unsigned cntbit() const {
        return __lg(*vec.rbegin()) + (size() - 1) * 32;
    }
    friend ostream& operator<<(ostream& os, CR bu) {
        int n = bu.size();
        os << hex << bu.vec[n - 1];
        for(int i = n - 2; i >= 0; --i)
            os << setw(8) << setfill('0') << bu.vec[i];
        return os;
    }
    void print() const {
        int n = size();
        for(int i = n - 1; i >= 0; --i) 
            cout << bitset<32>(vec[i]);
    }
    BigUint& operator+=(CR a) {
        int n = size(), m = a.size();
        if(n < m) vec.insert(vec.end(), m - n, 0);
        vec.push_back(0);
        n = size();
        bool flag = false;
        for(int i = 0; i < m; i++) {
            ull x = ull(vec[i]) + a.vec[i] + flag;
            vec[i] = x;
            flag = x >> 32;
        }
        for(int i = m; i < n && flag; i++) {
            ++vec[i];
            flag = !vec[i];
        }
        remove0();
        return *this;
    }
    BigUint& operator-=(CR a) {
        int n = size(), m = a.size();
        bool flag = false;
        for(int i = 0; i < m; i++) {
            ull x = ull(vec[i]) - a.vec[i] - flag;
            vec[i] = x;
            flag = x >> 32;
        }
        for(int i = m; i < n && flag; i++) {
            --vec[i];
            flag = !~vec[i];
        }
        remove0();
        return *this;
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
    BigUint& operator>>=(unsigned k) {
        int n = size(), p = k / 32, q = k % 32, r = 32 - q;
        for(int i = 0; i < n - p - 1; i++)
            vec[i] = (vec[i + p] >> q) | ((vec[i + p + 1] & ((1u << q) - 1)) << r);
        vec[n - p - 1] = vec[n - 1] >> q;
        vec.erase(vec.begin() + (n - p), vec.end());
        remove0();
        return *this;
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
    BigUint& operator<<=(unsigned k) {
        if(k % 32 == 0) left(k / 32);
        else {
            left(k / 32 + 1);
            *this >>= 32 - k % 32;
        }
        return *this;
    }
    friend BigUint operator<<(BigUint bu, unsigned k) {
        if(k % 32 == 0) bu.left(k / 32);
        else {
            bu.left(k / 32 + 1);
            bu >>= 32 - k % 32;
        }
        return bu;
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
    BigUint cbrt() const {
        BigUint r, res, sq;
        int n = cntbit() / 3 + 1;
        for(int i = n - 1; i >= 0; i--) {
            int thr = (getbit(i * 3 + 2) << 2) | (getbit(i * 3 + 1) << 1) | getbit(i * 3);
            (r <<= 3).uint() |= thr;
            res <<= 1; sq <<= 2;
            BigUint d = (sq + res) * 3;
            d.uint() |= 1;
            if(d <= r) {
                r -= d;
                (sq += res << 1).uint() |= 1;
                res.uint() |= 1; 
            }
        }
        return res;
    }
    BigUint rt5() const {
        BigUint r, res, sq, cb, pw4;
        int n = cntbit() / 5 + 1;
        for(int i = n - 1; i >= 0; i--) {
            int fiv = (getbit(i * 5 + 4) << 4) | (getbit(i * 5 + 3) << 3) |
                      (getbit(i * 5 + 2) << 2) | (getbit(i * 5 + 1) << 1) | getbit(i * 5);
            (r <<= 5).uint() |= fiv;
            res <<= 1; sq <<= 2; cb <<= 3; pw4 <<= 4;
            BigUint d = (((cb + sq) << 1) + (pw4 + res)) * 5;
            d.uint() |= 1;
            if(d <= r) {
                r -= d;
                (pw4 += ((cb + res) << 2) + sq * 6).uint() |= 1;
                (cb += (sq + res) * 3).uint() |= 1;
                (sq += res << 1).uint() |= 1;
                res.uint() |= 1; 
            }
        }
        return res;
    }
    static void initC() {
        for(int i = 0; i <= N; i++) {
            C[i][0] = C[i][i] = 1;
            for(int j = 1; j < i; j++)
                C[i][j] = C[i - 1][j - 1] + C[i - 1][j];
        }
        pw[0] = 1;
    }
    BigUint rt(int k) const {
        BigUint r, &res = pw[1];
        for(int i = 1; i < k; i++) pw[i] = 0;
        for(int i = cntbit() / k; i >= 0; i--) {
            for(int j = k - 1; j >= 0; j--)
                (r <<= 1).uint() |= getbit(i * k + j);
            for(int j = 1; j < k; j++)
                pw[j] <<= j;
            BigUint d;
            for(int j = 0; j < k; j++)
                d += pw[j] * C[k][j];
            if(d <= r) {
                r -= d;
                for(int j = k - 1; j >= 1; j--)
                    for(int t = 0; t < j; t++)
                        pw[j] += pw[t] * C[j][t];
            }
        }
        return res;
    }
    BigUint root(unsigned k) const {
        if(k == 2) return sqrt();
        if(k == 3) return cbrt();
        if(k == 5) return rt5();
        if(k < N) return rt(k);
        unsigned c = cntbit() / k;
        BigUint l(1);
        l <<= c;
        BigUint r(l << 1);
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
    friend BigUint operator+(CR a, CR b) { BigUint c(a); c += b; return c; }
    friend BigUint operator-(CR a, CR b) { BigUint c(a); c -= b; return c; }
    BigUint& operator*=(CR bu) { return *this = *this * bu; }
};
ull BigUint::C[N + 1][N + 1];
BigUint BigUint::pw[N];
struct _init { _init() { BigUint::initC(); } } _init0;