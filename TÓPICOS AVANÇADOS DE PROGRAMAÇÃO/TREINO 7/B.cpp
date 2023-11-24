#include <bits/stdc++.h>
using namespace std;

const int T = 1e6+50;
string n;
long long DP[T], N, ans, maior, c;

long long solve(int N){
    if(N == 0) return 0;
    if(DP[N] != -1) return DP[N];

    maior = -1;
    for (int i = 0; i < n.size(); i++)
    {
        if(n[i]-48 > maior) maior = n[i]-48;
    }
    
    N -= maior;
    n = to_string(N);
    //cout << maior << " m\n";
    c++;

    ans = solve(N);
    
    return DP[N] = ans;
}

int main(){
    memset(DP, -1, sizeof(DP));

    cin >> N;

    n = to_string(N);

    //cout << N << " " << n << "\n";

    solve(N);
    
    cout << c << "\n";

    return 0;
}
