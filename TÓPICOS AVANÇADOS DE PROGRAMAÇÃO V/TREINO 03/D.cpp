#include <bits/stdc++.h>
using namespace std;

const int v = 1e5+100;
int n, m, r;

void bfs(vector<vector<int>>& graph, int S, vector<int>& par, vector<int>& dist)
{
    queue<int> q;
    dist[S] = 0;
    q.push(S);

    while (!q.empty()) {
        int node = q.front();
        q.pop();
        //cout << node << ": ";

        for (int neighbour : graph[node]) {
            //cout << neighbour << "(d: " << dist[neighbour] << ") ";
            if (dist[neighbour] > n) {               
                par[neighbour] = node;
                dist[neighbour] = dist[node] + 1;
                q.push(neighbour);
            }
        }
        //cout << "\n";
    }
    //cout << "\n";
}

void printShortestDistance(vector<vector<int>>& graph, int S, int D, int V)
{
    vector<int> par(V+1, -1);

    vector<int> dist(V+1, 1e9);

    bfs(graph, S, par, dist);

    if (dist[D] == 1e9) {
        cout << "IMPOSSIBLE\n";
        return;
    }

    vector<int> path;
    int currentNode = D;
    path.push_back(D);
    while (par[currentNode] != -1) {
        path.push_back(par[currentNode]);
        currentNode = par[currentNode];
    }

    /* for (int i = par.size() - 1; i >= 0; i--)
        cout << par[i] << " ";

    cout << "\n"; */
    r = path.size();
    cout << r << "\n";
    for (int i = path.size() - 1; i >= 0; i--){
        cout << path[i] << " ";
    }
}

int main(){
    vector<vector<int>> graph(v);

    cin >> n >> m;

    for (int i = 0; i < m; i++)
    {
        int a, b;
        cin >> a >> b;
        graph[a].push_back(b);
        graph[b].push_back(a);
    }

    printShortestDistance(graph, 1, n, n);
    return 0;
}
