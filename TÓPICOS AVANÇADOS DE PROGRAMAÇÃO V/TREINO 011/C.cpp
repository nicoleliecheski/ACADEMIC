#include <bits/stdc++.h>
using namespace std;

int main(){
    int N, n, u, d, c, res;

    cin >> n;
    N = n;

    for(int i = n; i <= 919; i++){
        u = N%10;
        d = (N%100 - N%10)/10;
        c = (N%1000 - N%100)/100;
        //cout << N << " " << c << " " << d << " " << u << "\n";
        
        if(d*c == u){
            res = N;
            break;
        } else{
            N++;
        }
    }
    

    cout << res << "\n";

    return 0;
}
