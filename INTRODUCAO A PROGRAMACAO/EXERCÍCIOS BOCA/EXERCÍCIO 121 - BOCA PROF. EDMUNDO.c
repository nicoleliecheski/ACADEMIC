#include <stdio.h>

int main()
{
    int N, i, j, M[1000][1000]; //declaração das variáveis
    //este programa foi feito pela aluna: Nicole Liecheski
    //na data 25/07/2023

    //Faça um programa que receba uma matriz quadrada e imprima os elementos de sua diagonal secundária.
    //A entrada contem apenas em caso de teste. A primeira linha ha um inteiro N, 1 < N ≤ 1000,
    //representando a dimensão das matrizes. A seguir haverá N linhas com N inteiros em cada linha
    //separados por um espaço em branco cada, representando os elementos da matriz A.

    scanf("%d", &N);

    for(i = 0; i < N; i++){
        for(j = 0; j < N; j++){
            scanf("%d", &M[i][j]);
        }
    }
    
    j = N-1;
    for(i = 0; i < N; i++){
        printf("%d\n", M[i][j]);
        j--;
    }

    /*for(i = 0; i < N; i++){
        for(j = 0; j < N; j++){
            printf("%d ", M[i][j]);
        }
        printf("\n");
    }*/

    return 0;
}
