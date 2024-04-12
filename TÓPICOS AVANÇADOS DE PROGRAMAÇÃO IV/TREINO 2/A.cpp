#include <bits/stdc++.h>
using namespace std;

int main(){
    string S, T;

    cin >> S;

    for (int i = 0; i < S.size(); i++)
    {
        T += char(S[i] - 32);
        //cout << char(S[i] - 32) << " ";
    }
    
    cout << T << "\n";

    return 0;
}
