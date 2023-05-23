#include <stdio.h>

int main() {

    int V[100000], B[1000], N, M,  v, p, i, g, h, w, achei;//declaração das variáveis
    //este programa foi feito pela aluna: Nicole Liecheski
    //na data 22/05/2023

    //Na primeira linha do caso de teste há um número inteiro N, 1 ≤ N ≤ 100000,
    //representando o tamanho do vetor V. Na linha seguinte haverá N números inteiros
    //separados por um espaço em branco, que são nos N valores do vetor V. Na terceira linha será informado
    //um número inteiro M, 1 ≤ M ≤ 1000, representando a quantidade de buscas que serão efetuadas no vetor.
    //Logo em seguida haverá M linhas, cada uma com um número inteiro que deve ser buscado no vetor V.

    achei = 0;

    scanf("%d", &N);

    for(v = 0; v < N; ++v){
        V[v] = 0;
    }
    for(w = 0; w < M; ++w){
        B[w] = 0;
    }

    for(p = 0; p < N; ++p){
        scanf("%d ", &V[p]);
    }

    scanf("%d", &M);

    for(i = 0; i < M; ++i){
        scanf("%d", &B[i]);
    }

    for(g = 0; g < M; ++g){
        while (h < N && !achei) {
            if (V[h] == B[g]) {
                achei = 1;
            } else {
                h++;
            }
        }
        if (achei==1) {
            printf ("ACHEI\n");
        } else {
            printf ("NAO ACHEI\n");
        }
    }

    return 0;
}
