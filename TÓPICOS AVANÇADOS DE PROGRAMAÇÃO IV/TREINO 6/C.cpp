#include <bits/stdc++.h>
using namespace std;

long long N, M, ruins[100050];
long long DP[100050];
const long long mod = 1e9 + 7;
long long PD(int degrau){
    if(degrau > N) return 0;
    if(ruins[degrau] == 1) return 0;
    if(degrau == N) return 1;
    if(DP[degrau] != -1) return DP[degrau];

    long long ans = 0;
    ans = (ans + PD(degrau+1))%mod;
    ans = (ans + PD(degrau+2))%mod;
    DP[degrau] = ans;
    return ans;
}

int main(){
    memset(DP, -1, sizeof(DP));

    cin >> N >> M;

    for (int i = 0; i < M; i++)
    {
        int c;
        cin >> c;
        ruins[c]++;
    }
    

    cout << PD(0) << "\n";


    return 0;
}
