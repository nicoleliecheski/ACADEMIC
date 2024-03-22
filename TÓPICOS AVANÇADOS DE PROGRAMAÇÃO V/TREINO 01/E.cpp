#include <bits/stdc++.h>
using namespace std;

int bi, che, ba;

int main(){
    int n, res;

    cin >> n;

    for (int i = 1; i <= n; i++)
    {
        int a;
        cin >> a;
        if(i%3 == 1) che+= a;
        else if(i%3 == 2) bi+= a;
        else ba+= a;
    }
    
    if(bi > che && bi > ba) cout << "biceps\n";
    else if(che > bi && che > ba) cout << "chest\n";
    else cout << "back\n";

    return 0;
}
