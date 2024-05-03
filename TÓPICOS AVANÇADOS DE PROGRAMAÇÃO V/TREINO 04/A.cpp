#include <bits/stdc++.h>
using namespace std;

const int N = 2*1e5 + 500;

typedef long long ll;

ll soma[N];

int main(){
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    ll n, q;

    cin >> n >> q;

    for (ll i = 1; i <= n; i++)
    {
        ll a;
        cin >> a;
        soma[i] = soma[i-1] + a;
    }
    
    for (ll i = 0; i < q; i++)
    {
        ll a, b, r;
        cin >> a >> b;
        r = soma[b] - soma[a-1];
        cout << r << "\n";
    }
    
    
    return 0;
}
