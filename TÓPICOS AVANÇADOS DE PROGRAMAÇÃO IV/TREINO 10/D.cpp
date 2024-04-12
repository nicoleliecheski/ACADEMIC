#include <bits/stdc++.h>
#include <math.h>
using namespace std;

int main(){
    int Z;
    double N, K, res = 0.000;

    cin >> N >> K;

    for (int i = 1; i <= N; i++)
    {
        for (int j = 0; j < 20; j++)
        {
            double X = i * pow(2.000, j); 
            if(X >= K){
                Z = j;
                break;
            }
        }
        
        res += (1.000/N) * pow((1.000/2.000), Z);
    }
    
    cout << fixed << setprecision(12);

    cout << res << "\n";

    return 0;
}
