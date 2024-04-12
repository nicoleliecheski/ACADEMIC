#include <bits/stdc++.h>
#include <math.h>
using namespace std;

int main(){
    int N, par = 0;

    cin >> N;

    for (int i = 0; i < N; i++)
    {
        int a;
        cin >> a;
        if(a%2 == 0) par++; 
    }
    
    cout << pow(3,N) - pow(2,par) << "\n";

    return 0;
}
