#include <bits/stdc++.h>
#include <string>
using namespace std;

int main(){
    string S[20];
    int N;

    cin >> N;

    S[1] = "1";

    for (int i = 2; i <= N; i++)
    {
        S[i] = S[i-1] + " " + to_string(i) + " " +S[i-1];
    }
    
    cout << S[N] << "\n";

    return 0;
}
