#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

map<ll, ll> freq;

int main(){
    ll N, res = 0, rep = 0;

    cin >> N;

    for (int i = 0; i < N; i++)
    {
        int a;
        cin >> a;
        freq[a]++;
    }
    

    for(auto & [a,b]: freq){
        if(b > 1){
            rep += (b*(b-1))/2;
        }
    }

    res = (N*(N-1))/2 - rep;

    cout << res << "\n";

    return 0;
}
