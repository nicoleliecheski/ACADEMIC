#include <stdio.h>

int main() {

    int T, N, i, V[1000], j, c, d, menor = 9999; //declaração das variáveis
    //este programa foi feito pela aluna: Nicole Liecheski
    //na data 03/07/2023

    //Calcular a distância entre os dois elementos mais próximos em uma sequência de N números inteiros.
    //Na primeira linha há um inteiro T, 1 ≤ T ≤ 10, representando a quantidade de testes a serem realizados.
    //Para cada teste são esperados: (a) um número inteiro N, 2 ≤ N ≤ 1000, que é o tamanho do vetor, e (b)
    //uma sequência de N números inteiros que são os elementos do vetor. No vetor cada elemento deve estar no
    //intervalo [-1000 , 1000].

    scanf("%d", &T);

    for(i = 0; i < T; i++){
        scanf("%d", &N);
        for(j = 0; j < N; j++){
            if(j < N-1){
                scanf("%d ", &V[j]);
            } else{
                scanf("%d", &V[i]);
            }
        }
        for(j = 0; j < N-1; j++){
            for(c = j+1; c < N; c++){
                d = V[j] - V[c];
                if(d < 0){
                    d = -d;
                }
                if(menor > d){
                    menor = d;
                }
            }
        }
        printf("%d %d\n", menor, N*N);
    }
  
    return 0;
}
