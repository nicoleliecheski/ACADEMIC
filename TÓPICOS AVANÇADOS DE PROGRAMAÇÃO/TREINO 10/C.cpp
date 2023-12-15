#include <bits/stdc++.h>
using namespace std;

int nR[110], nB[110];

int main(){
    int T, N[110];
    string R[110], B[110];

    cin >> T;

    for (int i = 0; i < T; i++)
    {
        cin >> N[i];
        cin >> R[i];
        cin >> B[i];
    }

    for (int i = 0; i < T; i++)
    {
        for (int j = 0; j < N[i]; j++)
        {
            //cout << j << "\n";
            //cout << R[i][j]-'0' << " " << B[i][j]-'0' << "\n";
            if((R[i][j]-'0') > (B[i][j]-'0')) 
                {nR[i]++;
                //cout << "r++\n";
                }
            else if((B[i][j]-'0') > (R[i][j]-'0'))
                {nB[i]++;
                //cout << "b++\n";
                }
            //else cout << "eq\n";
        }
        if(nR[i] > nB[i]) cout << "RED\n";
        else if( nB[i] > nR[i]) cout << "BLUE\n";
        else cout << "EQUAL\n";
    }
    
    return 0;
}
