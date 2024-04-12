#include <bits/stdc++.h>
using namespace std;

int main(){
    int cont = 0;
    string S;

    cin >> S;

    for(int i = 0; i < S.size(); i++){
        if(S[i] == 'v'){
            cont++;
        } else if(S[i] == 'w'){
            cont += 2;
        }
    }

    cout << cont << "\n";

    return 0;
}
