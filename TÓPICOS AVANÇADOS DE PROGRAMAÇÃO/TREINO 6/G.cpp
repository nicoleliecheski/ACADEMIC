#include <bits/stdc++.h>
using namespace std;

const long long T = 1e5;
string S;
int DP[55][55], A[55], B[55], N, X, i;
long long mod = 1e9+7;

int PD(int i, int qt, int C){ 
    if(i == N) return 0;
    if(qt > B[i]) PD(i+1, 1, C);
    if(DP[i][qt] != -1) return DP[i][qt];

    C += A[i+1]*qt;

    long long ans = 0;

    cout << C << "\n";

    if(C == X) return 1;
    if(C < X) ans += PD(i, qt+1, A[i]);
    if(C > X) return 0;

    return DP[i][qt] = ans;
}

int main(){
    memset(DP, -1, sizeof(DP));

    cin >> N >> X;

    for (int i = 0; i < N; i++)
    {
        cin >> A[i] >> B[i];
    }

    if(PD(0, 1, 0)) cout << "Yes\n";
    else cout << "No\n";

    return 0;
}
