#include <bits/stdc++.h>
using namespace std;

set<vector<int>> s;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int N, res;

    cin >> N;

    for (int i = 0; i < N; i++)
    {
        int L;
        vector<int> v;
        cin >> L;
        for (int i = 0; i < L; i++)
        {               
            int a;
            cin >> a;
            v.push_back(a);
        }
        s.insert(v);
        v.clear();
    }
    
    cout << s.size() << "\n";
    
    return 0;
}
