// Programa que calcula a quantidade de conjuntos LD (Localizadores Dominantes) possiveis de um grafo G qualquer
// Utiliza funcoes dos cabecalhos personalizados "subset.h" e "showg.h"
// Para compilar utilize o comando "g++ -o QtdConjLD QtdConjLD.cpp subset.cpp graphio.c"

#include <iostream>
#include <vector>
#include <algorithm> 
#include <stdio.h>
#include "subset.h"
#include "showg.h"
using namespace std;

int main(int argc, char *argv[]) {
  // Quantidade de conjuntos LDs possiveis
  int LDs = 0;

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

  // Calcula todos os subconjuntos de vertices S possíveis de G
  set< set<int> > res = subsets(n);
  
  vector<unsigned long> g((size_t)n * m);
  stringtograph(s, g.data(), m);

  // Passa por cada conjunto S
  for (auto& x : res) {
    
    int eh_LD = 1; // Inicialmente eh true

    if(!x.empty()){
      // Passa por cada vertice de G
      for (int i = 0; i < n; i++) { 
        
        // Verifica as condicoes para ser dominante: cada vertice de G tem que estar em S ou ter um vizinho em S
        if(x.find(i) == x.end()){
          // Passa por cada elemento y do S atual (representado por x)
          for (auto& y : x) {  
            if(sao_vizinhos(g.data(), y, i, m)){
              eh_LD = 1;
              break;
            }
            eh_LD = 0;
          }
        }
      }
  
      LDs += eh_LD;
    } else {
        eh_LD = 0;
    }
  }

  cout << "Quantidade de conjuntos LD: " << LDs << endl;

  fclose(f);
  return 0;
}
