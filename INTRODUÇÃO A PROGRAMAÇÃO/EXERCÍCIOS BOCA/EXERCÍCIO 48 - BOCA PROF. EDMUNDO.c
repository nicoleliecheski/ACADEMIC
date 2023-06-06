#include <stdio.h>

int main() {

    int V[10000], N, i, c, p, maior, cont = 0; //declaração das variáveis
    //este programa foi feito pela aluna: Nicole Liecheski
    //na data 25/05/2023

    //Faça um programa que receba vários vetores de números inteiros,
    //calcule o maior elemento (M) de cada vetor e apresente a frequência
    //dos valores menores ou iguais a i, onde i = 0,1,2,...,M.
    //O programa possui vários casos de testes.
    //A primeira linha de cada caso contém um inteiro 1 < N ≤
    //10000, representando o tamanho do vetor. A segunda linha
    //conterá N inteiros entre 0 e 10000, representando
    //os N elementos do vetor. A entrada termina quando N = 0.

    scanf("%d", &N);

    while(N != 0){
        maior = -1;
        for(i = 0; i < N; i++){
            if(i > (N-1)){
                scanf("%d ", &V[i]);
            } else{
                scanf("%d\n", &V[i]);
            }
            if(V[i] > maior){
                maior = V[i];
            }
        }
        for(c = 0; c <= maior; c++){
            for(p = 0; p < N; p++){
                if(V[p] <= c){
                    cont++;
                }
            }
            printf("(%d) %d\n", c, cont);
            cont = 0;
        }
        scanf("%d\n", &N);
    }

    return 0;
}
