#include <bits/stdc++.h>
using namespace std;

int par, impar;

int main(){
    int x, n[105], res;

    cin >> x;

    for (int i = 0; i < x; i++)
    {
        cin >> n[i];
        if(n[i]%2 == 0) par++;
        else impar++;
    }
    
    if(par > 1){
        for (int i = 0; i < x; i++)
        {
            if(n[i]%2 != 0) res = i+1;
        }
    } else{
        for (int i = 0; i < x; i++)
        {
            if(n[i]%2 == 0) res = i+1;
        }
    }

    cout << res << "\n";

    return 0;
}
