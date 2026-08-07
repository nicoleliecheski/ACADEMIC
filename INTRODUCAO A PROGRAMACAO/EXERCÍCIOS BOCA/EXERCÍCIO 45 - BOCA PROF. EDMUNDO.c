#include <stdio.h>

int main() {

    int V[5000], N, i; //declaração das variáveis
    //este programa foi feito pela aluna: Nicole Liecheski
    //na data 25/05/2023

    //Escreva um programa em C para ler n elementos inteiros em um vetor, e depois imprimi-los.
    //A entrada contém duas linhas. A primeira, contém um valor inteiro n < 5000 que corresponde ao
    //número de elementos que aparecem na segunda linha. A segunda linha contém n valores inteiros, separados
    //entre si por um espaço.


    scanf("%d\n", &N);

    for(i = 0; i < N; ++i){
        V[i] = 0;
    }

    for(i = 0; i < N; i++){
        if(i<N-1){
            scanf("%d ", &V[i]);
        }
        else{
            scanf("%d", &V[i]);
        }
    }

    for(i = 0; i < N; i++){
        printf("%d ", V[i]);
    }

    return 0;
}
