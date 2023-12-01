#include <bits/stdc++.h>
using namespace std;

long long fat(long long x){
    long long r = 1;

    if(x == 0) return 1;

    for(int i = x; i > 0; i--){
        r *= i;
    }
    return r;
}

int main(){
    long long M, N, res = 0;

    cin >> N >> M;

    if(N > 1){
        res += (N*(N-1))/2;
    }
    if(M > 1){
        res += (M*(M-1))/2;
    }

    cout << res << "\n";

    return 0;
}
