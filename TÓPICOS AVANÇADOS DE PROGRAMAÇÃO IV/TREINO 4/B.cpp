#include <bits/stdc++.h>
using namespace std;

int main(){
    int N, K, res, c = 0;

    cin >> N >> K;

    res = N;

    while(res != 0){
        res = res/K;
        c++;
    }
    
    cout << c << "\n";

    return 0;
}
