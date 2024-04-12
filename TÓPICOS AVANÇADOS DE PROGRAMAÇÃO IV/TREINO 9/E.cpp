#include <bits/stdc++.h>
using namespace std;

int pares[10000], mod = 1e9+7;

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
    int N[10000], R[10000], T;

    cin >> T;

    for (int i = 0; i < T; i++)
    {
        cin >> N[i];
        for (int j = 0; j < N[i]; j++)
        {
            int a;
            cin >> a;
            if((a%2 == 0)&&(a != 0)) pares[i]++;
        }
        if(pares[i] == 0) R[i] = 1;
        else if(pares[i] == 1) R[i] = 2;
        else R[i] = 2 + (fat(pares[i]))%mod;
    }

    for (int i = 0; i < T; i++)
    {
        cout << R[i]%mod << "\n";
    }
    

    return 0;
}
