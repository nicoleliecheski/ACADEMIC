#include <bits/stdc++.h>
using namespace std;

set<int> v;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int N, flag = 0;

    cin >> N;

    for (int i = 0; i < N; i++)
    {
        int a;
        cin >> a;
        if(v.find(a) != v.end()) flag = 1;
        else v.insert(a);
    }
    
    if(flag == 0) cout << "YES\n";
    else cout << "NO\n";
    
    
    return 0;
}
