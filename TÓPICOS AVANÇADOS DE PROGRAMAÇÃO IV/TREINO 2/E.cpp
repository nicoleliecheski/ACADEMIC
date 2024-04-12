#include <bits/stdc++.h>
using namespace std;

long long c[1010];

int main(){
    long long small = 999999;
    string S, T;

    cin >> S >> T;

    if(S == T){
        cout << "0\n";
    } else{
        for (int i = 0; i < S.size(); i++)
        {
            for (int j = 0; j < T.size(); j++)
            {
                //cout << char(S[i+j]) << " != " << T[j] << "?\n";
                if(char(S[i+j]) != T[j]){
                    c[i]++;
                }
            }
        }

        for (int i = 0; i < S.size(); i++)
        {
            if(c[i] != 0 && c[i] < small){
                small = c[i];
            }
        }
        
        cout << small << "\n";
    }
    
    return 0;
}
