#include <bits/stdc++.h>
using namespace std;

vector<pair<int, int>> v;

int bb(int i, int n, int k){
    int l = -1, r = n, ans = -1;
    while (r - l > 1) {
        int m = (l + r) / 2;
        if (k <= v[m].first) {
            //cout << "1 " << i << " "<< v[m].first << "\n";
            r = m;
        } else {
            if(v[m].first+i == k){
                //cout << "2 " << i << " " << v[m].first << "\n";
                ans = v[m].second;
                break;
            } else if(v[m].first+i > k){
                //cout << "3 " << i << " " << v[m].first << "\n";
                r = m;
            } else{
                //cout << "4 " << i << " " << v[m].first << "\n";
                l = m;
            }
        }
    }
    
    return ans;
}

int main(){
    int n, x;
    pair<int, int> ans = {-1, -1};
    
    cin >> n >> x;
    
    for(int i = 1; i <= n; i++){
        int a;
        cin >> a;
        v.push_back(make_pair(a, i));
    }
    
    sort(v.begin(), v.end());
    
    //for(int i = 0; i < n; i++){
      //  cout << v[i].first << " ";
    //}
    
    //cout << "\n";
    
    for(int i = 0; i < n; i++){
        //cout << v[i].first << " " << v[i].second << "\n";
        ans = {v[i].second, bb(v[i].first, n, x)};
        if(ans.second != -1) break;
    }
    
    if(ans.second != -1) cout << ans.first << " " << ans.second << "\n";
    else cout << "IMPOSSIBLE\n";

    return 0;
}
