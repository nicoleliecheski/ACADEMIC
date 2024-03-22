#include <bits/stdc++.h>
using namespace std;

int sf, fs;

int main(){
    int n;
    char ant;

    cin >> n;

    for (int i = 0; i < n; i++)
    {
        char a;
        cin >> a;
        if(i > 0){
            if(ant == 'S' && a == 'F') sf++;
            else if(ant == 'F' && a == 'S') fs++;
        }
        ant = a;
    }

    if(sf > fs) cout << "YES\n";
    else cout << "NO\n";
    
    return 0;
}
