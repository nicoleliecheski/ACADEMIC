/* 
Programa que dado varios grafos cubicos, calcula a quantidade de grafos com
conjuntos RED-OLD (Redudantes Abertos Localizadores Dominantes) possiveis
e registra a RED-OLD(G) de cada
*/
// Os grafos estao armazenados em arquivos do tipo .g6
// Utiliza funcoes dos cabecalhos personalizados "subset.h" e "graphio.h"
// Para compilar:
// 1. gcc -c graphio.c -o graphio.o  
// 2. g++ -o Conjuntos_RED_OLD Conjuntos_RED_OLD.cpp subset.cpp graphio.o
// Para executar:
// ./Conjuntos_RED_OLD <nome do arquivo>.g6

#include "graphio.h"
#include "subset.h"
#include <iostream>
#include <set>
#include <stdio.h>
#include <utility>
#include <vector>
#include <climits>

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
  // Abrindo o arquivo de entrada do tipo .g6
  // Um arquivo so pode conter varios grafos mas todos possuem o mesmo tamanho de V(G) 
  if (argc < 2) {
    fprintf(stderr, "Usando: %s <input.g6>\n", argv[0]);
    return -1;
  }
  FILE *f = fopen(argv[1], "r");
  if (f == NULL) {
    fprintf(stderr, "Erro: Nao foi possivel abrir o arquivo '%s'\n", argv[1]);
    return -1;
  }

  // Quantidade de grafos do arquivo de entrada que possuem um conjunto RED-OLD possivel
  int Qtd_Grafos_com_RED_OLD = 0;

  // Valores (tamanhos) possiveis dos conjuntos RED-OLD dos grafos de entrada
  set<int> Vlrs_Possiveis_RED_OLD;

  // Tamanho de V(G)
  int n = 0;

  // Verificacao para saber se os grafos do arquivo possuem cintura maior ou igual a 5
  int g5 = 0;
  cout << "g(G) >= 5? (1/0)\n";
  cin >> g5;

  char *s;
  // Indice do grafo atual
  int graph_num = 0;
  // Loop para pegar todos os grafos no arquivo .g6
  while ((s = showg_getline(f)) != NULL) { 
    // Quantidade de conjuntos RED-OLD que o grafo atual possui
    int Qtd_RED_OLDs = 0;
    // RED-OLD(G), cardinalidade minima de um subconjunto RED-OLD em G
    int RED_OLD_G = INT_MAX;
    graph_num++;

    // Otimizando: o n eh sempre igual nos arquivos
    if(n == 0) n = graphsize(s); 

    // Gerando todos os pares de vertices distintos possiveis do grafo
    vector<pair<int, int> > pares_distintos;
    for (int i = 0; i < n; i++) {
      for (int j = i + 1; j < n; j++) {
        pair<int, int> par(i, j);
        pares_distintos.push_back(par);
      }
    }

    // Tratamento dos dados do arquivo para poderem ser manipulados
    int m = graph_row_words(n);
    vector<unsigned long> g((size_t)n * m);
    stringtograph(s, g.data(), m);
    
    // Gerando a vizinhanca de cada vertice
    vector<set<int> > vizinhancas;
    for (int i = 0; i < n; i++) {
      set<int> vizi = vizinhos(g.data(), i, m);
      vizinhancas.push_back(vizi);
    }
    
    // Todos os subconjuntos S possiveis de vertices desse grafo
    set<set<int> > conjuntos_S = subsets(n); 

    // Loop passando por cada conjunto S
    for (auto &S : conjuntos_S) {  
      int tamanho_de_S = S.size();   

      // Otimizando: se o conjunto atual eh vazio passa pro proximo conjunto
      if(S.empty()) continue; 
      
      // Otimizando: Estamos a procura de valores possiveis de RED-OLD(G), logo se o S atual eh maior que isso, ja nao vale a pena testa-lo
      if(tamanho_de_S >= RED_OLD_G) continue;
      
      // Otimizando: os limites de RED-OLD(G) sao (2/3)*n <= RED-OLD(G) <= n
      if(((2/3) * n) > tamanho_de_S) continue; 
      
      // Otimizando: se o grafo possui g(G) (cintura) >= 5, entao V(G) é RED-OLD
      if(g5 && tamanho_de_S == n){ 
        Qtd_RED_OLDs++;
        if(tamanho_de_S < RED_OLD_G){
          RED_OLD_G = tamanho_de_S;
        }
        continue;
      }  

      // Flag pra indicar se o conjunto S atual eh RED-OLD ou nao
      int eh_RED_OLD = 1; 

      // Loop passando por cada par de vertice distinto u e v do grafo
      for (auto &pp : pares_distintos) { 
        int flag_u = 0;
        int flag_v = 0;

        // Loop passando por cada elemento do conjunto S atual
        for (auto &s : S) { 

          auto tem_u = vizinhancas[s].find(pp.first);
          auto tem_v = vizinhancas[s].find(pp.second);
          auto s_end = vizinhancas[s].end();

          // Conferindo se a vizinhanca do elemento de S atual tem APENAS o u
          if(tem_u != s_end && tem_v == s_end) { 
            flag_u = 1;
            continue;
          } 
          // Conferindo se a vizinhanca do elemento de S atual tem APENAS o v
          else if(tem_v != s_end && tem_u == s_end) { 
            flag_v = 1;
            continue;
          } 
        }

        // Se alguma dessas flag ainda eh false entao o conjunto S atual nao cobriu esse par logo S nao eh RED-OLD
        if(!flag_u || !flag_v){ 
          eh_RED_OLD = 0;
          break;
        }
      }

        // Se essa flag ainda eh true entao esse conjunto S eh RED-OLD
      if(eh_RED_OLD){ 
        Qtd_RED_OLDs++;

        // Guardando o tamanho de S como um valor possivel de RED-OLD(G)
        if(tamanho_de_S < RED_OLD_G){
          RED_OLD_G = tamanho_de_S;
        }
      }
    }

    // Se algum conjunto S eh RED-OLD entao significa que esse grafo tem um conjunto RED-OLD possivel
    if(Qtd_RED_OLDs > 0){ 
      Vlrs_Possiveis_RED_OLD.insert(RED_OLD_G);
      Qtd_Grafos_com_RED_OLD++;
    }
  }

  // Verificando se o arquivo nao esta vazio
  if (graph_num == 0) {
    fprintf(stderr, "Erro: Nao foi lido nenhuma linha de grafo (arquivo vazio?)\n");
    fclose(f);
    return -1;
  } else {
    cout << "Quantidade de grafos = " << graph_num << endl;
    cout << "Quantidade de grafos com RED-OLD = " << Qtd_Grafos_com_RED_OLD << endl;

    cout << "Valores possiveis para RED-OLD(G) = ";
    for(auto &r : Vlrs_Possiveis_RED_OLD){
      cout << r << " ";
    }
    cout << "\n";
  }

  fclose(f);
  return 0;
}
