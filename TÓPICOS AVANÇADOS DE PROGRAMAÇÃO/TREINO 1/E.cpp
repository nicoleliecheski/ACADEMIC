#include <bits/stdc++.h>
using namespace std;

int N = 1e5+100;

int main(){
    long long n, M, T, A[N], X[N], Y[N], somaA = 0, somaY = 0;

    cin >> n >> M >> T;

    for (int i = 1; i < n; i++)
    {
        cin >> A[i];
        somaA += A[i];
    }
    //cout << "//\n";
    for (int i = 1; i <= M; i++)
    {   
        //cout << "//3\n";
        cin >> X[i] >> Y[X[i]];
        somaY += Y[X[i]];
    }
    //cout << "2//\n";
    if(somaA < T+somaY){
        for (int i = 1; i < n; i++)
        {
            T = T - A[i] + Y[i];
            if(T <= 0){
                cout << "No\n";
                break;
            } else if(i == n-1){
                cout << "Yes\n";
            }
        }
        
    } else{
        cout << "No\n";
    }

    return 0;
}
