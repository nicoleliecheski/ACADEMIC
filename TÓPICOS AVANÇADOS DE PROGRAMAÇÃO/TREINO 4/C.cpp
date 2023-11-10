#include <bits/stdc++.h>
using namespace std;

int main(){
    int N, r = 0;

    cin >> N;

    for (int i = 1; i <= N; i++)
    {
        if(i%2 != 0){
            int c = 0;
            for (int j = 1; j <= i; j++)
            {
                if(i%j == 0){
                    c++;
                }
            }
            //cout << c << " ";
            if(c == 8){
                r++;
            }
        }
    }

    //cout << "\n";
    
    cout << r << "\n";

    return 0;
}
