#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

const int T = 1e5 + 100;

ll N, H[T], maior = -1, a = 0;


int main(){    
    cin >> N;

    for (int i = 0; i < N; i++)
    {
        cin >> H[i];
    }

    for (int i = 0; i < N; i++)
    {
        //cout << "i = " << i << "\n";
        if(i != N-1 && H[i] >= H[i+1]) a++;
        else{
            //cout << "else\n";
            if(a > maior) maior = a;
            a = 0;
        } 

        if(i == N-1 && a > maior) maior = a;
    }
    
    cout << maior << "\n";
    

    return 0;
}
