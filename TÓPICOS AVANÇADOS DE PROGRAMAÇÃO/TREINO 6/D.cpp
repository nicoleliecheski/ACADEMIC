#include <bits/stdc++.h>
using namespace std;

const long long T = 2*1e5;
long long N, K, A[T], B[T];
long long DP[T];

long long PD(int Xi){
    long long ans = 0;
    if(Xi == 0){
        if(abs(A[Xi]-A[Xi+1]) <= K){
            cout << "/AA\n";
            DP[Xi] = A[Xi];
            ans = 1;
        } else if(abs(A[Xi]-B[Xi+1]) <= K){
            cout << "/AB\n";
            DP[Xi] = A[Xi];
            ans = 1;
        } else if(abs(B[Xi]-B[Xi+1]) <= K){
            cout << "/BB\n";
            DP[Xi] = A[Xi];
            ans = 1;
        } else if(abs(B[Xi]-A[Xi+1]) <= K){
            cout << "/BA\n";
            DP[Xi] = A[Xi];
            ans = 1;
        }
    }

    cout << DP[Xi] << "\n";

    if(abs(DP[Xi]-A[Xi+1]) <= K){
        cout << "/A1\n";
        DP[Xi+1] = A[Xi+1];
        ans = 1;
    } else if(abs(DP[Xi]-B[Xi+1]) <= K){
        cout << "/B1\n";
        DP[Xi+1] = B[Xi+1];
        ans = 1;
    }

    if(ans == 0) return 0;
    if(Xi < N) ans = PD(Xi+1);
    return ans;
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

    if(PD(0)) cout << "Yes\n";
    else cout << "No\n";

    return 0;
}
