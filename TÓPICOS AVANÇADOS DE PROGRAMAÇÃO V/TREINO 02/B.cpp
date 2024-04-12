#include <bits/stdc++.h>
using namespace std;

int main(){
    int t;

    cin >> t;

    for(int i = 0; i < t; i++){
        int n, m;
        cin >> n >> m;
        if((n >= 2 && m >= 3) || (n >= 3 && m >= 2)){ //tem pelo menos 1 que da certo
            if((n == 3 && m == 2) || (n == 2 && m == 3)) cout << "2 2\n";
            else if(n == 3 && m == 3) cout << "2 2\n";
            else cout << n << " "<< m << "\n";
        } else cout << n << " "<< m << "\n";
    }
    return 0;
}
