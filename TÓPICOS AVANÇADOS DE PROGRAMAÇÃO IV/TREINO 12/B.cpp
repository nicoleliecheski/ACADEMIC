#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

const int T = 120;

ll t, N[T];

int main(){    
    cin >> t;

    for (int i = 0; i < t; i++)
    {
        cin >> N[i];
    }

    for (int i = 0; i < t; i++)
    {
        if(N[i]%2 == 0){
            for (int j = N[i]; j > 0; j--){
                cout << j << " ";
            }
            cout << "\n";
        } else{
            for (int j = N[i]; j > 0; j--){
                if(j == N[i]/2) cout << N[i]/2 + 1 << " "; 
                else if(j == (N[i]/2)+1) cout << N[i]/2 << " "; 
                else cout << j << " ";
            }
            cout << "\n";
        }
    }
    

    return 0;
}
