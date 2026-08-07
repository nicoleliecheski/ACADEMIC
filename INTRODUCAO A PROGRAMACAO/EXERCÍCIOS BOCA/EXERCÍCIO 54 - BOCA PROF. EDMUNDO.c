#include <stdio.h>

int main() {

    int N, i, min, j, aux;
    double mediana;
    long int V[100000]; //declaração das variáveis
    //este programa foi feito pela aluna: Nicole Liecheski
    //na data 27/06/2023

    //A entrada consiste de um único caso de teste. Na primeira linha,
    //é informado um inteiro N, 0 < N ≤ 106, representando a quantidade
    //de elementos da amostra de dados. Nas N linhas seguintes é informado
    //um inteiro por linha, este valor varia de −2^32 a (2^32)−1.
    //A saída consiste da mediana dos dados informados. O valor da mediana
    //deve ser formatado com duas casas decimais.

    scanf("%d", &N);

    for(i = 0; i < N; i++){
        scanf("%ld", &V[i]);
    }

    for(i = 0; i < N; i++){
        for(j = i+1; j < N; j++){
            if(V[i] > V[j]){
                aux = V[i];
                V[i] = V[j];
                V[j] = aux;
            }
        }
    }
    
    if((N%2) != 0){
        mediana = V[N/2];
    } else{
        mediana = (V[N/2]+V[N/2-1])/2.00;
    }
    
    printf("%.2lf\n", mediana);
    
    return 0;
}

