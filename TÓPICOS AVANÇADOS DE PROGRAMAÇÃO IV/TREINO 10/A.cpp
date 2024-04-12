#include <bits/stdc++.h>
using namespace std;

int main(){
    int X, Y, Z, K = 6;

    cin >> X >> Y;

    Z = max(X, Y);

    Z = 7 - Z;

    if(Z%2 == 0){
        Z = Z/2;
        K = K/2;}
    if(Z%3 == 0){
        Z = Z/3;
        K = K/3;}
    if(Z%6 == 0){
        Z = Z/6;
        K = K/6;}
    
    cout << Z << "/" << K << "\n";

    return 0;
}
