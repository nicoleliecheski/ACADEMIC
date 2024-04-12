#include <bits/stdc++.h>
using namespace std;

const long long T = 2*1e5;
long long N, K, A[T], B[T];
int DP[T][2];

int PD(int Xi, bool ant){ // 0 = A - 1 = B
    if(Xi == N) return 1;
    if(DP[Xi][ant] != -1) return DP[Xi][ant];
    DP[Xi][ant] = 0;
    if(ant == 0){
        if(abs(A[Xi] - A[Xi-1]) <= K) DP[Xi][ant] |= PD(Xi+1, 0);
        if(abs(B[Xi] - A[Xi-1]) <= K) DP[Xi][ant] |= PD(Xi+1, 1);
    } else{
        if(abs(A[Xi] - B[Xi-1]) <= K) DP[Xi][ant] |= PD(Xi+1, 0);
        if(abs(B[Xi] - B[Xi-1]) <= K) DP[Xi][ant] |= PD(Xi+1, 1);
    }
    return DP[Xi][ant];
}

int main(){
    memset(DP, -1, sizeof(DP));

    cin >> N >> K;

    for (int i = 0; i < N; i++)
    {
        cin >> A[i]; 
    }

    for (int i = 0; i < N; i++)
    {
        cin >> B[i]; 
    }

    //cout << PD(1, 0) << " " << PD(1, 1) << "\n";
    if(PD(1, 0)|PD(1, 1)) cout << "Yes\n";
    else cout << "No\n";

    return 0;
}
