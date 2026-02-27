// Programa para calcular todos os subconjuntos de vertices possiveis de um grafo dado seu tamanho n
// Fonte: https://www.geeksforgeeks.org/dsa/backtracking-to-find-all-subsets/

#include <iostream>
#include <set>
#include <algorithm> 
#include "subset.h"
using namespace std;

set< set<int> > subsets(int n) {
  set< set<int> > res;
  
  // Loop through all possible subsets
  for (int i = 0; i < (1 << n); i++) {
    set<int> subset;

    // Loop through all elements of the input array
    for (int j = 0; j < n; j++) {
      // Check if the jth bit is set 
      if ((i & (1 << j)) != 0) {
          subset.insert(j);
      }
    }
    
    // Push the subset into result
    res.insert(subset);
  }
  
  return res;
}
