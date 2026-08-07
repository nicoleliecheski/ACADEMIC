#include <stdio.h>

int main() {

    int V[1000], N, i, res; //declaração das variáveis
    //este programa foi feito pela aluna: Nicole Liecheski
    //na data 24/05/2023

    //Faça um programa que receba um vetor V de N inteiros e imprima os elementos pares do vetor V.
    //A entrada contém apenas um caso de teste com 2 linhas. Na primeira linha há um inteiro N,
    //1 < N ≤ 1000, representando o tamanho do vetor V. Na segunda linha há N valores inteiros 
    //separados por um espaço em branco cada, que são os valores do vetor V.


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

    res = 0;
    for(i = 0; i < N; ++i){
        if ((V[i]%2)==0) {
            res++;
            printf("%d ", V[i]);
        }
    }

    printf("%d\n", res);
    return 0;
}
