// Programa que calcula a quantidade de conjuntos LD (Localizadores Dominantes) possiveis de um grafo G qualquer
// O grafos esta armazenado em arquivo do tipo .g6
// Quando ha mais de um grafo em um arquivo ele interage apenas com o primeiro
// Utiliza funcoes dos cabecalhos personalizados "subset.h" e "graphio.h"
// Para compilar utilize o comando "g++ -o Conjuntos_RED_LD Conjuntos_RED_LD.cpp subset.cpp graphio.o"
// Para executar utilize o comando "./Conjuntos_RED_LD <nome do arquivo>.g6"

#include <iostream>
#include <vector>
#include <algorithm> 
#include <stdio.h>
#include "subset.h"
#include "graphio.h"
using namespace std;

// Funcao que retorna um set com os vizinhos de um vertice v
set<int> vizinhos(unsigned long *g, int v, int m) { 
  int j1, j2;
  unsigned long *set1;
  set1 = g + v * m;
  set<int> viz;

  j1 = -1;
  while ((j1 = nextelement((unsigned long *)set1, m, j1)) >= 0) {
    viz.insert(j1);
  }

  return viz;
}

int main(int argc, char *argv[]) {
  // Tem RED-LD possivel?
  int RED_LD_possivel = 0;

  // RED-LD(G) (RED-LD com cardinalidade minima)
  int RED_LD_G = INT_MAX;


  // Abrindo um arquivo .g6
  if (argc < 2) {
    fprintf(stderr, "Usage: %s <input.g6>\n", argv[0]);
    return -1;
  }
  FILE* f = fopen(argv[1], "r");
  if (f == NULL) {
    fprintf(stderr, "Error: Unable to open file '%s'\n", argv[1]);
    return -1;
  }
  char *s = showg_getline(f);
  if (s == NULL) {
    fprintf(stderr, "Error: No graph line read (empty file?)\n");
    fclose(f);
    return -1;
  }

  // Calcula a quantidade de vertices de G
  int n = graphsize(s);
  int m = graph_row_words(n);

  // Tratamento dos dados do arquivo para poderem ser manipulados
  vector<unsigned long> g((size_t)n * m);
  stringtograph(s, g.data(), m);

  set<int> V_G;

  // Gerando a vizinhanca de cada vertice
  vector<set<int> > vizinhancas;
  for (int i = 0; i < n; i++) {
    V_G.insert(i); // aproveitando o loop para add os vertices em V(G)
    set<int> vizi = vizinhos(g.data(), i, m);
    vizinhancas.push_back(vizi);
    cout << "N(" << i << ") = ";
    for (auto &v : vizi){
      cout << v << ", ";
    }
    cout << "\n";
  }

  // Calcula todos os subconjuntos de vertices S possíveis de G
  set< set<int> > res = subsets(n);
  
  // Passa por cada conjunto S
  int ty = 1;
  for (auto& x : res) {
    //cout << "cheguei " << ty << endl;
    ty++;
    
    int eh_RED_LD = 1; // Inicialmente eh true
    
    if(!x.empty()){
      set<int> V_G_menos_S;
      V_G_menos_S = V_G;
      for(auto& v : x){
        V_G_menos_S.erase(v);
      }

      vector<set<int> > vizinhancas_em_S(n);

      for (int i = 0; i < n; i++){
        for(auto& vizi : vizinhancas[i]){
          if(x.find(vizi) != x.end()) vizinhancas_em_S[i].insert(vizi); 
        }
      }

      // 1 condicao
      for (int i = 0; i < n; i++) {
        int cnt = 0;
        if(x.find(i) != x.end()) cnt++;
        cnt += vizinhancas_em_S[i].size();
        if(cnt < 2){
          eh_RED_LD = 0;
          break;
        }
      }
      //cout << "c1" << endl;

      // 2 condicao
      for(auto& v : x){
        if(eh_RED_LD != 1) break;

        for(auto& u : V_G_menos_S){
          set<int> dif_simetrica = vizinhancas_em_S[u];

          for(auto& z : vizinhancas_em_S[v]){
            dif_simetrica.insert(z);
          }

          set<int> aux = dif_simetrica;

          for(auto& z : aux){
            if((vizinhancas_em_S[u].find(z) != vizinhancas_em_S[u].end()) && (vizinhancas_em_S[v].find(z) != vizinhancas_em_S[v].end()) && (dif_simetrica.find(z) != dif_simetrica.end())) dif_simetrica.erase(z);
          }

          if(dif_simetrica.find(v) != dif_simetrica.end()) dif_simetrica.erase(v);

          if(dif_simetrica.size() < 1){
            eh_RED_LD = 0;
            break;
          }
        }
      }
      //cout << "c2" << endl;

      // 3 condicao
      for(auto& u : V_G_menos_S){
        if(eh_RED_LD != 1) break;

        for(auto& v : V_G_menos_S){
          if(u != v){
            set<int> dif_simetrica = vizinhancas_em_S[u];

            for(auto& z : vizinhancas_em_S[v]){
              dif_simetrica.insert(z);
            }

            set<int> aux = dif_simetrica;

            for(auto& z : aux){
              if((vizinhancas_em_S[u].find(z) != vizinhancas_em_S[u].end()) && (vizinhancas_em_S[v].find(z) != vizinhancas_em_S[v].end()) && (dif_simetrica.find(z) != dif_simetrica.end())) dif_simetrica.erase(z);
            }

            if(dif_simetrica.size() < 2){
              eh_RED_LD = 0;
              break;
            }
          }
        }
      }
      //cout << "c3" << endl;

      if(eh_RED_LD == 0) continue; // conjunto S atual nao eh RED-LD, passa pro proximo
      else{
        RED_LD_possivel = 1;

        if(x.size() < RED_LD_G) RED_LD_G = x.size();
      }
    } else {
      continue;
    }
  }

  if(RED_LD_possivel == 1) cout << "Tem RED-LD possivel\nRED-LD(G) = " << RED_LD_G << endl;
  else cout << "Nao tem RED-LD possivel\n" << endl;

  fclose(f);
  return 0;
}
