#include <bits/stdc++.h>
using namespace std;

//const int t = 1e9;

int main(){
    double n, m, d, res;

    cin >> n >> m >> d;

    if(d != 0) res = ((2.0*(n-d))/(n*n))*(m-1.0);
    else res = ((n*m)-n)/(n*n);

    cout << fixed << setprecision(10);
    
    cout << res << "\n";

    return 0;
}
