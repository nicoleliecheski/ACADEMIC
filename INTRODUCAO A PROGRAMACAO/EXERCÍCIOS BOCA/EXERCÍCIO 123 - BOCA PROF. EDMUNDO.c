#include <stdio.h>

int main()
{
    int l, c, i, j; //declaração das variáveis
    //este programa foi feito pela aluna: Nicole Liecheski
    //na data 25/07/2023

    //Uma matriz xadrez é uma matriz (quadrada ou retangular) composta de 0s e 1s.
    //O padrão reticulado inicia em 1 (branco) e é alternado com 0 (preto).
    //Entrada: Dois números inteiros que representam o número de linhas e colunas da matriz xadrez.
    //Saída: Uma matriz xadrez de m linhas e n colunas.

    scanf("%d %d", &l, &c);
    
    for(i = 0; i < l; i++){
        for(j = 0; j < c; j++){
            if((i%2 == 0) && (j%2 == 0)){
                printf("1");
            } else if((i%2 == 0) && (j%2 != 0)){
                printf("0");}
            if((i%2 != 0) && (j%2 == 0)){
                printf("0");
            } else if((i%2 != 0) && (j%2 != 0)){
                printf("1");
            }
        }
        printf("\n");
    }

    return 0;
}
