#include <bits/stdc++.h>
using namespace std;

int main(){
    string S, largest, small;

    cin >> S;

    largest = small = S;

    string aux = S;

    for (int i = 0; i < S.size(); i++)
    {
        //cout << aux << "1\n";
        aux.push_back(aux[0]);
        //cout << aux << "2\n";
        aux = aux.substr(1, aux.size());
        //cout << aux << "3\n";
        if(small > aux){
            small = aux;
        }
        if(largest < aux){
            largest = aux;
        }
    }

    cout << small << "\n" << largest << "\n";
    
    return 0;
}
