#include <bits/stdc++.h>
using namespace std;

int main(){
    int n, k, l, c, d, p, nl, np, T1, T2, T3, res;

    cin >> n >> k >> l >> c >> d >> p >> nl >> np;

    T1 = (l * k)/nl;
    T2 = c * d;
    T3 = p/np;

    res = (min(T1, min(T2, T3)))/n;

    cout << res << "\n";

    return 0;
}
