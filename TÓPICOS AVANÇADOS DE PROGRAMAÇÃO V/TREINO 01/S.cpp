#include <bits/stdc++.h>
using namespace std;

int main(){
    int a, b, res, l = 0;
    vector<int> c; 

    cin >> a >> b;

    res = a*b;

    for (int i = 0; i < a; i++)
    {
        for (int j = 0; j < b; j++)
        {
            char a;
            cin >> a;
            if(a == 'S'){
                if((find(c.begin(), c.end(), j)) == c.end()){
                    res--;
                    //cout << "/\n";
                }
                if(l == 0){
                    res--;
                    //cout << "//\n";
                }
                l = 1;
                c.push_back(j);
            }
        }
        l = 0;
    }
    
    cout << res << "\n";

    return 0;
}
