#include <bits/stdc++.h>
using namespace std;

int main(){
    double res, A, B, C, X;

    cin >> A >> B >> C >> X;

    res = C/(B-A);

    //cout << res << "\n";

    if(X <= A) {
        res = 1.000;
        //cout << "1\n";
        }
    else if(X <= B)
    {   res = C/(B-A); 
        //cout << "2\n";
        }
    else 
    {   res = 0.000;
        //cout << "3\n";
        }

    cout << fixed << setprecision(12);

    cout << res << "\n";

    return 0;
}
