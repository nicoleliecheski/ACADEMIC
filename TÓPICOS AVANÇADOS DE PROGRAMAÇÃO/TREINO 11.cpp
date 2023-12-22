#include <bits/stdc++.h>
#include <string>
using namespace std;

int main(){
    int n1 = 0, n2 = 0, n3 = 0, n4 = 0, c = 0;
    string n, Sn2;

    cin >> n;

    while(n4 != 6174){
        c++;
        n4 = 0;
        n1 = stoi(n);
        Sn2 = n;
        sort(Sn2.begin(), Sn2.end(), greater<char>());

        n2 = stoi(Sn2);

        int a = n2;
        while (a > 0) { 
            n3 = n3 * 10 + a % 10; 
            a = a / 10; 
        }

        n4 = n2 - n3;
        //cout << n1 << " " << n2 << " " << Sn2 << " "<< n3 << " " << n4 << "\n";
        n1 = 0;
        n2 = 0; 
        n3 = 0; 
        n = to_string(n4);
    }

    cout << c << "\n";

    return 0;
}
