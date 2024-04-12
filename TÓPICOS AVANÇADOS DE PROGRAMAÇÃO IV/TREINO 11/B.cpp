#include <bits/stdc++.h>
using namespace std;

double v1, v2, maxM, mel, aux;
long long cam = 0;


int main(){
    cin >> v1 >> v2;

    if(v2<v1){
        cout << "0\n";
    } else{
        maxM = v2/v1;

        mel = 0.00;

        while(1){
            cam++;
            aux += cam;
            mel += aux;
            //cout << mel << "\n";
            if(mel > maxM){
                cam --;
                break;
            } 
        }

        cout << cam << "\n";
    }

    return 0;
}
