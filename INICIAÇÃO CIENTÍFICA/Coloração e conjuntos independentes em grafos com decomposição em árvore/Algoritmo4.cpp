#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef pair<int, int> pii;
typedef pair<ll, ll> pll;
#define pb push_back
#define ft first
#define sd second
const ll inf = 1e15;


vector<int> lexbfs(vector<set<int>>adj, int N){
    vector<pair<string, int>> label(N); //first é a label, second é o vertice
    vector<bool> vis(N, false);
    vector<int> res(N);
    srand (time(NULL));
    int v = rand() % (N); //numero aleatorio entre 0 e N

    label[v] = {to_string(N-1), v};
    for (int i = 0; i < N; i++)
    {
        if(i != v) label[i] = {"", i};
    }
    
    set<pair<string, int>> pq;
    vis[v] = true;
    pq.insert(label[v]);

    
    int i = N-1;
    while(!pq.empty()){
        pair<string, int> aux = *(pq.rbegin());
        pq.erase(aux);
        res[i] = aux.sd;
        vis[aux.sd] = true;
        
        
        for(auto x : adj[aux.sd]){
            if(!vis[x]){
                auto it = pq.find({label[x].ft, x});
                if(it != pq.end()) pq.erase(it);
                adj[x].erase(aux.sd);
                label[x].ft += to_string(i);
                pq.insert({label[x].ft, x});
            }
        }

        i--;
    }

    return res;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    //vector<set<int>> adj = {{1,4},{0,2,4},{1,3,4},{2,4},{0,1,2,3,5},{4,6,9},{5,7,9},{6,8,9},{7,9},{5,6,7,8}};
    vector<set<int>> adj = {{1,3},{0,2},{1,3},{0,2}};
    
    int N = adj.size();
    
    vector<int> res = lexbfs(adj, N);
    
    for(auto x : res){
        cout << x << " ";
    }
    cout << "\n";

    reverse(res.begin(), res.end());
    
    int f = 0;
    vector<bool> vis(N, false);
    for (int i = 0; i < N; i++)
    {
        set<int> early_neighbors_V, early_neighbors_W;
        int v = res[i];
        int w = -1;

        vis[v] = true;
        for (int j = i-1; j >= 0; j--)
        {
            if(adj[v].find(res[j]) != adj[v].end()){
                w = res[j];
                break;
            }
        }
        //cout << "\nv = " << v << " w = " << w;
        
        if(w == -1) continue;
        else {
            //cout << "\nvizinhos V\n";
            for(auto x : adj[v]){
                if(vis[x] && x != w){
                    early_neighbors_V.insert(x);
                    //cout << x << " ";
                }
            }
            //cout << "\nvizinhos W\n";
            for(auto x : adj[w]){
                if(vis[x]) {
                    early_neighbors_W.insert(x);
                    //cout << x << " ";
                }
            }

            for(auto x : early_neighbors_V){
                if(early_neighbors_W.find(x) == early_neighbors_W.end()){
                    cout << "nao eh cordal\n";
                    return 0;
                }
            } 
        }
    }

    cout << "eh cordal\n";

    return 0;
}
