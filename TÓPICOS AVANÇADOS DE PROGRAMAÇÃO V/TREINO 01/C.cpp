#include <bits/stdc++.h>
using namespace std;

int at, bt, ct;

int main(){
    int x;

    cin >> x;

    for (int i = 0; i < x; i++)
    {
        int a, b, c;
        cin >> a >> b >> c;
        at += a;
        bt += b;
        ct += c;
    }
    
    if(at == 0 && bt == 0 && ct == 0) cout << "YES\n";
    else cout << "NO\n";

    return 0;
}
