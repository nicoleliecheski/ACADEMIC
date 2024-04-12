#include <bits/stdc++.h>
using namespace std;

const long long T = 1e4+50;
string S;
int DP[55][T], A[55], B[55], N, X, i;

int PD(int p, int K){
    if(K == 0) return 1;
    if(p == N) return 0;

    int &ans = DP[p][K];
    if(ans != -1) return ans;
    ans = 0;
    for (int i = 0; i <= B[p]; i++)
    {   
        if(K < A[p]*i) break;
        ans |= PD(p+1, K - A[p]*i);
    }
    
    return ans;
}

int main(){
    memset(DP, -1, sizeof(DP));

    cin >> N >> X;

    for (int i = 0; i < N; i++)
    {
        cin >> A[i] >> B[i];
    }

    if(PD(0, X)) cout << "Yes\n";
    else cout << "No\n";

    return 0;
}
