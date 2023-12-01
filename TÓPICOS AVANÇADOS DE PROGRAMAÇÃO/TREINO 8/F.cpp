#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

int main(){
    ll N, K, res, x = 1;

    cin >> N;

    for(ll i = N-1; i >= N-11; i--){
        x *= i;
    }

    res = x/39916800;

    cout << res << "\n";

    return 0;
}
