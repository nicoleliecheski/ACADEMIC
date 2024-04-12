#include <bits/stdc++.h>
using namespace std;

int DP[3010][3010];
string s, t;

int solve(int i, int j){
    if(i == s.size() || j == t.size()) return 0;
    int &ans = DP[i][j];
    if(ans != -1) return ans;
    ans = max(solve(i+1, j), solve(i, j+1));
    if(s[i] == t[j]) ans = max(ans, 1 + solve(i+1, j+1));

    return ans;
}

void walk(int i, int j){
    if(i == s.size() || j == t.size()){
        cout << "\n"; return;}
    int &ans = DP[i][j];
    if(ans == solve(i+1, j)) walk (i+1, j);
    else if(ans == solve(i, j+1)) walk (i, j+1);
    else{
        cout << s[i];
        walk(i+1, j+1);
    } 
}

int main(){
    memset(DP, -1, sizeof(DP));

    cin >> s >> t;

    solve(0,0);
    walk(0,0);

    return 0;
}
