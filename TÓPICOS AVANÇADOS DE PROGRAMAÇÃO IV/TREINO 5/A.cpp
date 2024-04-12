#include <bits/stdc++.h>
using namespace std;

const int T = 2*1e5+100;

int modpot( int x, int n, int m ) {
    if (n == 0) return 1;
    if (x == 0) return 0;

    long long u = modpot( x, n/2, m );
    u = (u * u) % m;

    if (n % 2)
        u = (u * x) % m;

    return u;
}

int main(){
    int n, a, b;
    long long ans[T];

    cin >> n;

    for(int i = 0; i < n; i++){
        cin >> a >> b;
        ans[i] = modpot(a,b,1e9+7);
    }
    
    for(int i = 0; i < n; i++){
        cout << ans[i] << "\n";
    }

    return 0;
}
