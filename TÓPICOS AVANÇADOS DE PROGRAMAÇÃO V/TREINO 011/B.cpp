#include <bits/stdc++.h>
using namespace std;

multiset <char> v;

int main(){
    string s;

    cin >> s;

    for(int i = 0; i < s.size(); i++){
        v.insert(s[i]);
    }

    multiset<char>::iterator itr;
    for (itr = v.begin(); itr != v.end(); ++itr) {
        cout << *itr;
    }

    cout << "\n";

    return 0;
}
