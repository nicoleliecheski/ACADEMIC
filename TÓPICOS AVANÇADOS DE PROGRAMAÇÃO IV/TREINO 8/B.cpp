#include <bits/stdc++.h>
using namespace std;
#include <math.h> 

int main(){
    unsigned long long N, res = 0;

    cin >> N;

    if(N > 1){
        res = pow(2, N) - N - 1;
    }

    cout << res << "\n";

    return 0;
}
