#include <bits/stdc++.h>
#include <string>
using namespace std;

string s, t;

vector<int> kmp(string s) {
    int n = (int)s.length();
    vector<int> pi(n);
    for (int i = 1; i < n; i++)
    {
        int j = pi[i-1];
        while(j > 0 && s[i] != s[j])
            j = pi[j-1];
        if(s[i] == s[j])
            j++;
        pi[i] = j;
    }
    return pi;
}

int main(){

    cin >> s >> t;

    vector<int>pi;
    pi = kmp(s);

    string conc = t+'#'+s;
    pi = kmp(conc);
    int ans = 0;

    for (int i = t.size(); i < conc.size(); i++)
    {
        if(pi[i] == t.size()) ans++;
    }
    

    if(ans > 0){
        cout << "Yes\n";
    } else{
        cout << "No\n";
    }
    

    return 0;
}
