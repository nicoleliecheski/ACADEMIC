#include <bits/stdc++.h>
#include <math.h>
using namespace std;

int freq[30], mod = 1e9+7;
map<char, int> mapa;

long long binpow(long long a, long long b, long long m) {
    a %= m;
    long long res = 1;
    while (b > 0) {
        if (b & 1)
            res = res * a % m;
        a = a * a % m;
        b >>= 1;
    }
    return res;
}

long long fat(long long x){
    long long u = 1;

    if(x == 0) return 1;

    for (long long i = x; i > 0; i--)
    {
        u = u * i % mod;
    }

    return u;   
}

int main(){
    long long N, a=1, b = 1, r;
    string p;

    cin >> p;

    for (int i = 0; i < p.size(); i++)
    {
        freq[p[i]-'a']++;
        //mapa[p[i]]++;
    }

    N = p.size();

    a = fat(N);

    //cout << "N = " << N << "\n";
    //cout << "a = " << a << "\n";
    
    for (int i = 0; i <= 29; i++)
    {
        if(freq[i] > 1) b = b * fat(freq[i]) % mod; 
    }

    // for(auto it : mapa){
    //     for(int i=1; i<=it.second; i++){
    //         b = b * i % mod;
    //     }
    //     // b = b * fat(it.second) % mod;
    // }

    //cout << "b = " << b << "\n";

    //cout << binpow(b,mod-2) << "\n";
    
    r = a * binpow(b,mod-2, mod) % mod;
    
    cout << r << "\n";

    return 0;
}
