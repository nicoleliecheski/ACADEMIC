#include <bits/stdc++.h>
using namespace std;

long long DP[110][110], T, N, a[110][110], ans;

long long solve(int i, int j){
    if(i == N) return 0;
    if(DP[i][j] != -1) return DP[i][j];

    ans = max(a[i][j] + solve(i+1, j), a[i][j] + solve(i+1, j+1));
    //cout << "max(" << V[i] + solve(i+2) << ", " << solve(i+1) << ") = " << max(V[i] + solve(i+2), solve(i+1)) << "\n";
    
    return DP[i][j] = ans;
}

int main(){
    cin.tie(0);
    ios_base::sync_with_stdio(0);

    cin >> T;

    for (int i = 0; i < T; i++)
    {   
        cin >> N;
        memset(DP, -1, sizeof(DP));
        memset(a, 0, sizeof(a));
        for (int j = 0; j < N; j++)
        {
            for (int k = 0; k < j+1; k++)
            {
                cin >> a[j][k];
            }
        }

        cout << solve(0 , 0) << "\n";
        
    }

    

    return 0;
}
