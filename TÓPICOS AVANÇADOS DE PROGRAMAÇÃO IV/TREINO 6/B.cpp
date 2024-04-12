#include <bits/stdc++.h>
using namespace std;

long long N, X, Y;
long long DP[15][3];

long long PD(int  nivel, int tipo){
    if(nivel == 1){
        if(tipo == 1) return 1;
        return 0;
    } 
    long long &resposta = DP[nivel][tipo];
    if(resposta != -1) return resposta;
    resposta = 0;
    if(tipo == 0){
        resposta = resposta + PD(nivel - 1, 0);
        resposta = resposta + X * PD(nivel, 1);
    }
    else{
        resposta = resposta + PD(nivel - 1, 0);
        resposta = resposta + Y * PD(nivel - 1, 1);
    }
    return resposta;
}

int main(){
    memset(DP, -1, sizeof(DP));
    cin >> N >> X >> Y;

    cout << PD(N, 0) << "\n";


    return 0;
}
