#include <bits/stdc++.h>
using namespace std;

int main(){
    int n, r[110], flag = 0;

    cin >> n;

    for(int i = 0; i < n; i++){
        cin >> r[i];
        if(r[i] == 1){
            flag = 1;
        }
    }

    if(flag == 1){
        cout << "HARD\n";
    } else{
        cout << "EASY\n";
    }

    return 0;
}
