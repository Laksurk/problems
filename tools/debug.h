#define array_idx(name, _l, _r)                  \
    struct _array_idx {                          \
        int l, r;                                \
        _array_idx(int l, int r) : l(l), r(r) {} \
        auto operator[](int i) const {           \
            if (i < l || i > r) {                \
                cerr << i << endl;               \
                throw 1;                         \
            }                                    \
            return _##name[i];                   \
        }                                        \
        auto& operator[](int i) {                \
            if (i < l || i > r) {                \
                cerr << i << endl;               \
                throw 1;                         \
            }                                    \
            return _##name[i];                   \
        }                                        \
    } name(_l, _r);
