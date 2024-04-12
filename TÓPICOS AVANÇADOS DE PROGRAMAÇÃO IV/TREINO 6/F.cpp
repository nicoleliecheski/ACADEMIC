#include <bits/stdc++.h>
using namespace std;

const long long T = 1e5;
string S;
int DP[T][9];
long long mod = 1e9+7;

int PD(int i, int L){ 
    if(i == S.size()){
        if(L == 8) return 1;
        else return 0;
    }
    if(DP[i][L] != -1) return DP[i][L];

    long long ans = 0;
    ans = (ans + PD(i+1, L))%mod;
    if(S[i] == 'c' && L == 0) ans = (ans + PD(i+1, L+1))%mod;
    if(S[i] == 'h' && L == 1) ans = (ans + PD(i+1, L+1))%mod;
    if(S[i] == 'o' && L == 2) ans = (ans + PD(i+1, L+1))%mod;
    if(S[i] == 'k' && L == 3) ans = (ans + PD(i+1, L+1))%mod;
    if(S[i] == 'u' && L == 4) ans = (ans + PD(i+1, L+1))%mod;
    if(S[i] == 'd' && L == 5) ans = (ans + PD(i+1, L+1))%mod;
    if(S[i] == 'a' && L == 6) ans = (ans + PD(i+1, L+1))%mod; 
    if(S[i] == 'i' && L == 7) ans = (ans + PD(i+1, L+1))%mod; 

    return DP[i][L] = ans;
}

int main(){
    memset(DP, -1, sizeof(DP));

    cin >> S;

    cout << PD(0, 0);

    return 0;
}
