#include <bits/stdc++.h>
using namespace std;

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
