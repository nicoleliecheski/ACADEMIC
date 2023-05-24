#include <stdio.h>

int main() {

    int V[1000], K, N, i, res; //declaração das variáveis
    //este programa foi feito pela aluna: Nicole Liecheski
    //na data 24/05/2023

    //Dado um vetor V de tamanho N e um inteiro K, contabilize quantos elementos de V são maiores ou
    //iguais ao inteiro K.
    //O programa terá apenas um caso de teste. O programa deve ler, obrigatoriamente, um número N que
    //pertença ao intervalo 1 ≤ N ≤ 1000. Se N lido não for válido, o programa deve fazer uma nova leitura
    //de N. Caso N seja válido, N representa o tamanho do vetor V . Na próxima linha há N números inteiros
    //separados por um espaço em branco cada, representando cada elemento do vetor V . E finalmente, na última
    //linha há um inteiro K.

    for(i = 0; i < N; ++i){
        V[i] = 0;
    }

    N = 0;
    while((N<1)||(N>1000)){
        scanf("%d", &N);
    }

    for(i = 0; i < N; ++i){
        scanf("%d ", &V[i]);
    }

    scanf("%d", &K);

    res = 0;
    for(i = 0; i < N; ++i){
        if (V[i]>=K) {
            res++;
        }
    }

    printf("%d\n", res);
    return 0;
}
