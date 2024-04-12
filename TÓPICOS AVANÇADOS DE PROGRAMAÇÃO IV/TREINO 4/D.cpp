#include <bits/stdc++.h>
using namespace std;

bool isPrime(int n){
    if ((n < 2) || ((n > 2) && (n % 2 == 0))) return false;
    for (int i = 3; (i * i) <= n; i += 2){
        if (isPrime( i )) // <=== ???????
        {
            if (n % i == 0) return false;
        }
    }
    return true;
}

int main(){
    int T, ANS[25];

    cin >> T;
    
    for (int i = 0; i < T; i++)
    {
        int N;
        cin >> N;
        if(isPrime(N) == true){
            ANS[i] = 1;
        } else{
            ANS[i] = 0;
        }
    }

    for (int i = 0; i < T; i++)
    {
        if(ANS[i] == 1){
            cout << "yes\n";
        } else{
            cout << "no\n";
        }
    }

    return 0;
}
