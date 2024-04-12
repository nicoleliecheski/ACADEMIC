#include <bits/stdc++.h>
using namespace std;

int main(){
    int N, flag = 0;
    string W[105], res[6];

    res[0] = "and"; res[1] = "not";  res[2] = "that"; res[3] = "the"; res[4] = "you";

    cin >> N;

    for (int i = 0; i < N; i++)
    {
        cin >> W[i];
        if(W[i] == res[0] || W[i] == res[1] || W[i] == res[2] || W[i] == res[3] || W[i] == res[4]){
            flag++;
        }
    }

    if(flag > 0){
        cout << "Yes\n";
    } else{
        cout << "No\n";
    }
    
    

    return 0;
}
