#include <bits/stdc++.h>
using namespace std;

const int T = 1e5+100;

int freq[T];

int main(){
    int N, flag = 0;

    cin >> N;

    for (int i = 0; i < N-1; i++)
    {
        int a, b;
        cin >> a >> b;
        freq[a]++;
        freq[b]++;
    }

    for (int i = 1; i <= N; i++)
    {
        if(freq[i] == N-1) flag = 1;
    }
    
    if(flag == 1) cout << "Yes\n";
    else cout << "No\n";
    
    return 0;
}
