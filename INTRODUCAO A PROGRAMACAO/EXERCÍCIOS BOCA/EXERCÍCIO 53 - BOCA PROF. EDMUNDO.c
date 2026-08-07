#include <stdio.h>

int main() {

    int N, i, c, V[1000000], P[1000000] = {0}, maior = 0, freq = -1; //declaração das variáveis
    //este programa foi feito pela aluna: Nicole Liecheski
    //na data 26/06/2023

    //Faça um programa para imprimir uma lista de inteiros em ordem crescente.
    //A entrada contém apenas um caso de teste. Na primeira linha há um inteiro
    //N, 1 < N ≤ 1000, representando a quantidade de inteiros que serão informados.
    //Em seguida haverá N linhas com um inteiro em cada linha.

    scanf("%d", &N);

    for(i = 0; i < N; i++){
        scanf("%d", &V[i]);
        P[V[i]]++;
    }

    for(i = 0; i < N; i++){
        if(P[i] > freq){
            maior = i;
            freq = P[i];
        } else{
            if(P[i] == freq){
                if(V[i] < maior){
                    maior = i;
                    freq = P[i];
                }
            }
        }
    }

    printf("%d\n%d\n", maior, freq);

    return 0;
}

