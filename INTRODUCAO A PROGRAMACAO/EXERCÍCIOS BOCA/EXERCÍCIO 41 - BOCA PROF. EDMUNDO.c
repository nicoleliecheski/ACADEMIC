#include <stdio.h>

int main() {

    int V[100000], B[1000], N, M, i, h, achei;//declaração das variáveis
    //este programa foi feito pela aluna: Nicole Liecheski
    //na data 22/05/2023

    //Na primeira linha do caso de teste há um número inteiro N, 1 ≤ N ≤ 100000,
    //representando o tamanho do vetor V. Na linha seguinte haverá N números inteiros
    //separados por um espaço em branco, que são nos N valores do vetor V. Na terceira linha será informado
    //um número inteiro M, 1 ≤ M ≤ 1000, representando a quantidade de buscas que serão efetuadas no vetor.
    //Logo em seguida haverá M linhas, cada uma com um número inteiro que deve ser buscado no vetor V.

    achei = 0;

    scanf("%d", &N);

    for(i = 0; i < N; ++i){
        V[i] = 0;
    }
    i=0;

    for(i = 0; i < M; ++i){
        B[i] = 0;
    }
    i=0;

    for(i = 0; i < N; ++i){
        scanf("%d ", &V[i]);
    }
    i=0;

    scanf("%d", &M);

    for(i = 0; i < M; ++i){
        scanf("%d", &B[i]);
    }
    i=0;

    for(i = 0; i < M; ++i){
        h = 0;
        while ((h < N)&&(achei == 0)) {
            if (V[h] != B[i]) {
                h++;
            } else {
                achei = 1;
            }
        }
        if (achei!=0) {
            printf ("ACHEI\n");
            achei = 0;
        } else {
            printf ("NAO ACHEI\n");
        }
    }

    return 0;
}
