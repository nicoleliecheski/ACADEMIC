#include <bits/stdc++.h>
using namespace std;

int main(){
    int L, R;
    string S, T;

    cin >> L >> R;

    cin >> S;

    L -= 1; R -= 1;

    for (int i = 0; i < L; i++)
    {
        T += S[i];
    }
    
    for (int i = R; i >= L; i--)
    {
        T += S[i];   
    }

    for (int i = R+1; i < S.size(); i++)
    {
        T += S[i];   
    }

    cout << T << "\n";
    
    return 0;
}
