#include <bits/stdc++.h>
using namespace std;

const int N = 5*1e5 + 500;

typedef long long ll;

ll v[N], soma[N];
ll * somap[N],  * somap1[N];

int main(){
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    ll n, q;

    cin >> n >> q;

    for (ll i = 0; i < n; i++)
    {
        cin >> v[i];
        soma[i] = soma[i-1] + v[i];
        somap1[i] = &soma[i-1];
        somap[i] = (somap1[i]+v[i]);
    }
    
    for (ll i = 0; i < q; i++)
    {
        ll a;
        cin >> a;
        if(a == 0){
            ll p, x;
            cin >> p >> x;
            v[p] += x;
            soma[p] += x;
            somap[p] += x;
            //cout << "novo valor de v[" << p << "] = " << v[p] << "\n";
        } else{
            ll l, r;
            cin >> l >> r;
            cout << soma[r-1] - soma[l-1] << "\n";
        }
    }
    
    
    return 0;
}
