#include <bits/stdc++.h>
using namespace std;

vector<string> vertices;
vector<vector<int>> adj, filhos;

vector<int> color, time_in, time_out, pai, ci, mmx;
set<int> c;
int dfs_timer = 0;

void dfs(int v) {
    time_in[v] = dfs_timer++;
    color[v] = 1;
    for (int u : adj[v]){
        if (color[u] == 0){
            pai[u] = v;
            filhos[v].push_back(u);
            dfs(u);
        }
    }
    color[v] = 2;
    time_out[v] = dfs_timer++;
}

int main(){
    int n, maior = -1;

    cout << "Entrada:\n";

    cin >> n;

    adj.resize(n+2);
    filhos.resize(n+2);
    color.resize(n+2);
    time_in.resize(n+2);
    time_out.resize(n+2);
    pai.resize(n+2);
    ci.resize(n+2);
    mmx.resize(n+2);

    for (int i = 0; i < n; i++)
    {
        string a;
        cin >> a;
        vertices.push_back(a); //1, 0 ou alguma letra
    }
    
    for (int i = 0; i < n-1; i++)
    {
        int a, b;
        cin >> a >> b;
        adj[a].push_back(b);
        adj[b].push_back(a);
    }

    pai[0] = 0;

    dfs(0);

    int cont = 0;
    for (int i = 0; i < n; i++)
    {
        if(i != 0 && vertices[i] == "1" && vertices[pai[i]] == "0"){
            for(int j : filhos[i]){
                cont++;             
                ci[j] = cont;  
            }
            ci[filhos[i][0]] = ci[i];
        }
        else if(vertices[i] == "1"){         
            for(int j : filhos[i]){
                cont++;          
                ci[j] = cont;  
            }
        } else if (vertices[i] == "0")
        {            
            for(int j : filhos[i]){
                ci[j] = ci[i]; 
            }
        }
        
    }
    
    for(int i = 0; i < n; i++){
        if(vertices[i] != "1" && vertices[i] != "0"){
            int aux = ci[i];
            mmx[aux]++;
            if(mmx[aux] > maior) maior = mmx[aux];
            c.insert(aux);
        }
    }

    cout <<"\nSaida\nNumero da independencia: " <<maior << endl;

    return 0;
}
