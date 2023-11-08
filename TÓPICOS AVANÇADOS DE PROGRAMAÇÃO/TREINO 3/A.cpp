#include <bits/stdc++.h>
using namespace std;

struct comp {
    bool operator()(int const &i, int const &j) const {
        return i > j;
    }
};

int main(){
    int A, B, K, maior, v[110], c = 1;

    cin >> A >> B >> K;

    maior = max(A,B);

    for(int i = 1; i <= maior; i++){
        if(A%i == 0 && B%i == 0){
            v[c] = i;
            c++;
        }
    }

    // for (int i = 1; i <= c; i++)
    // {
    //     cout << v[i] << " ";
    // }

    // cout << "\n";

    //int n = sizeof(v) / sizeof(v[1]);

    sort(v+1, v + c, comp());

    // for (int i = 1; i <= c; i++)
    // {
    //     cout << v[i] << " ";
    // }
    
    // cout << "\n";

    cout << v[K] << "\n";

    //cout << maior << "\n";

    return 0;
}
