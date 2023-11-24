#include <bits/stdc++.h>
using namespace std;

int DP[110][110], H, W;
string s[150];

int main(){
    memset(DP, -1, sizeof(DP));

    cin >> H >> W;

    for (int i = 0; i < H; i++)
    {
        cin >> s[i];
    }
    DP[0][0] = 1;
    int rs = 1;
    for (int i = 0; i < H; i++)
    {
        for (int j = 0; j < W; j++)
        {
            if(s[i][j] == '#') continue;
            int mx = -1;
            if(j != 0) mx = max(mx, DP[i][j-1]);
            if(i != 0) mx = max(mx, DP[i-1][j]);
            if(mx != -1){
                DP[i][j] = mx+1;
                rs = max(rs, mx+1);
            }
        }
        
    }
    
    cout << rs << "\n";

    return 0;
}
