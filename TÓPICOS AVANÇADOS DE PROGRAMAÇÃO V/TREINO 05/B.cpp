#include <bits/stdc++.h>
using namespace std;

set<int> v;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int N, res;

    cin >> N;

    for (int i = 0; i < N; i++)
    {
        int a;
        cin >> a;
        v.insert(a);
    }
    
    for (int i = 0; i < 2002; i++)
    {
        if(v.find(i) == v.end()){
            res = i;
            break;
        }
    }
    
    cout << res << "\n";
    
    return 0;
}
