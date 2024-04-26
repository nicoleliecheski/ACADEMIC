#include <bits/stdc++.h>
using namespace std;

int freq[4];

int main(){
    int flag = 0;

    for (int i = 0; i < 3; i++)
    {
        int a, b;
        cin >> a >> b;
        freq[a]++;
        freq[b]++;
    }

    for (int i = 1; i < 5; i++)
    {
        if(freq[i] > 2){
            flag = 1;
        }
    }
    
    if(flag == 0){
        cout << "YES\n";
    } else{
        cout << "NO\n";
    }
    
    return 0;
}
