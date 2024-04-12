#include <bits/stdc++.h>
using namespace std;

vector <pair<string,int>> v;

bool sortbysec(const pair<string,int> &a, const pair<string,int> &b)
{
    return (a.second > b.second);
}

int main(){
    int n;

    cin >> n;

    for(int i = 0; i < n; i++){
        int t;
        string s;
        cin >> s >> t;
        v.push_back(make_pair(s, t));
    }

    sort(v.begin(), v.end(), sortbysec);

    cout << v[1].first << "\n";

    return 0;
}
