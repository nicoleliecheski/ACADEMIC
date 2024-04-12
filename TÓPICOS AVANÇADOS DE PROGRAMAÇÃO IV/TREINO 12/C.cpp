#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

ll k, a = 0, i = 0, b = 19;

vector<ll> n;

int main(){   
    string s; 
    cin >> k;

    n.push_back(19);

    while(i<k) {
        a = 0;
        b += 9;
        s = to_string(b);
        for (int j = 0; j < s.size(); j++)
        {
            a += (s[j] - '0');
        }
        if(a == 10){
            n.push_back(b);
            i++;
        } 
    }
    
    cout << n.at(k-1) << "\n";

    return 0;
}
