#include <bits/stdc++.h>
#include <math.h>
using namespace std;

typedef long long ll;

int main(){
    ll N, K, res;

    cin >> N >> K;

    res = K*pow(K-1, N-1);

    cout << res << "\n";

    return 0;
}
