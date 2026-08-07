#include <stdio.h>

int main() {

    int N, i, c, V[1000], menor = 1001, menorIND; //declaração das variáveis
    //este programa foi feito pela aluna: Nicole Liecheski
    //na data 26/06/2023

    //Faça um programa para imprimir uma lista de inteiros em ordem crescente.
    //A entrada contém apenas um caso de teste. Na primeira linha há um inteiro
    //N, 1 < N ≤ 1000, representando a quantidade de inteiros que serão informados.
    //Em seguida haverá N linhas com um inteiro em cada linha.

    scanf("%d", &N);

    for(i = 0; i < N; i++){
        scanf("%d", &V[i]);
        if(V[i] < menor){
            menor = V[i];
            menorIND = i;
        }
    }

    printf("%d\n", V[menorIND]);
    menor++;

    for(i = 0; i <= N; i++){
        for(c = 0; c < N; c++){
            if(V[c] == menor){
                printf("%d\n", V[c]);
            }
        }
        menor++;

    }

    return 0;
}
