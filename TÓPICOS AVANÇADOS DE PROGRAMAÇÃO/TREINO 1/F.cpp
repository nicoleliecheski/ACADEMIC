#include <bits/stdc++.h>
using namespace std;

const int N = 2*1e5+10;

vector<int> adj[N];
int n, vis[N], X, Y, pre[N], ans[N], cont;

void dfs(int x){
    /*if(vis[x] != 1){
        cout << "\n" << x << " ";
    }*/
    vis[x] = 1;
    int u;
    for (int i = 0; i < adj[x].size(); i++)
    {
        u = adj[x][i];
        //cout << adj[x][i] << " ";
        /*if(u == Y){
            break;
        }*/
        
        if(vis[u] == 0){
            pre[u] = x;
            dfs(u);
        }
    }
}

int main(){
    
    cin >> n >> X >> Y;

    for (int i = 0; i < n-1; i++)
    {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    
    dfs(X);

    int ant = Y;
    for (int i = 0; i < n; i++)
    {
        //cout << ant << " ";
        ans[i] = ant;
        cont++;
        if(ant == X){
            break;
        }
        ant = pre[ant];
    }

    for (int i = cont-1; i >= 0; i--)
    {
        cout << ans[i] << " ";
    }
    
    

    return 0;
}
