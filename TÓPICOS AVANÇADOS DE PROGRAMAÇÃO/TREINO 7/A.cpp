#include <bits/stdc++.h>
using namespace std;

const int N = 1e5+50;
long long DP[N], n, V[N], maior = -1, ans;

long long solve(int i){
    if(i > maior) return 0;
    if(DP[i] != -1) return DP[i];

    ans = max(V[i] + solve(i+2), solve(i+1));
    //cout << "max(" << V[i] + solve(i+2) << ", " << solve(i+1) << ") = " << max(V[i] + solve(i+2), solve(i+1)) << "\n";
    
    return DP[i] = ans;
}

int main(){
    memset(DP, -1, sizeof(DP));

    cin >> n;

    for (int i = 0; i < n; i++)
    {   
        int a;
        cin >> a;
        V[a] += a;
        if(a > maior){
            maior = a;}
    }

    /*for (int i = 1; i <= maior; i++)
    {
        cout << "V" << i << " = " << V[i] << "\n";
    }*/
    

    cout << solve(1) << "\n";
    cout << "meu amigo Rafael eh muinto lindo ave maria uwu";
    return 0;
}
