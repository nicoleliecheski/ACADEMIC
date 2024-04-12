#include <bits/stdc++.h>
using namespace std;

int main(){
    int H, W, flag = 0;
    string s[55];

    cin >> H >> W;
    for (int i = 0; i < H; i++)
    {
        cin >> s[i];
    }

    /* cout << "\nIMPRESSAO\n";

    for (int i = 0; i < H; i++)
    {
        cout << s[i] << "\n";
    } */

    for (int i = 0; i < H; i++)
    {
        for (int j = 0; j < W; j++)
        {   
            if(s[i][j] == '#'){
                if(i == 0 && j == 0){
                    if(s[i][j+1] != '#' && s[i+1][j] != '#') flag = 1;
                } else if(i == H-1 && j == 0){
                    if(s[i-1][j] != '#' && s[i][j+1] != '#') flag = 1;
                } else if(i == 0 && j == W-1){
                    if(s[i][j-1] != '#' && s[i+1][j] != '#') flag = 1;
                } else if(i == H-1 && j == W-1){
                    if(s[i][j-1] != '#' && s[i-1][j] != '#') flag = 1;
                } else if(i == 0){
                    if(s[i][j-1] != '#' && s[i][j+1] != '#' && s[i+1][j] != '#') flag = 1;
                } else if(j == 0){
                    if(s[i-1][j] != '#' && s[i][j+1] != '#' && s[i+1][j] != '#') flag = 1;
                } else if(i == H-1){
                    if(s[i-1][j] != '#' && s[i][j-1] != '#' && s[i][j+1] != '#') flag = 1;
                } else if(j == W-1){
                    if(s[i-1][j] != '#' && s[i][j-1] != '#' && s[i+1][j] != '#') flag = 1;
                } else if(s[i-1][j] != '#' && s[i][j-1] != '#' && s[i][j+1] != '#' && s[i+1][j] != '#') flag = 1;
            }
        }
    }

    if(flag == 0) cout << "Yes\n";
    else cout << "No\n";
    
    return 0;
}
