#include "bigint.h"

int main() {
    BigUint x(8);
    cout << "n: "; x.print(); cout << endl;
    BigUint y = x.rt(2);
    cout << "ans: "; y.print(); cout << endl;
    BigUint z = x.rt(2);
    cout << "ans: "; y.print(); cout << endl;
    BigUint y1 = x.rt(2);
    cout << "ans: "; y.print(); cout << endl;
    BigUint y2 = x.rt(2);
    cout << "ans: "; y.print(); cout << endl;
    BigUint y3 = x.rt(2);
    cout << "ans: "; y.print(); cout << endl;
    BigUint y4 = x.rt(2);
    cout << "ans: "; y.print(); cout << endl;
    
    return 0;
}