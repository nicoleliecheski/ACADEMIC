#include <stdio.h>

int main()
{
    int m = 0, n = 0, num, i, j, c = 0, col[1000]; //declaração das variáveis
    //este programa foi feito pela aluna: Nicole Liecheski
    //na data 25/07/2023

    //Escreva um programa em C que armazene elementos inseridos pelo usuário em uma matriz A
    //de dimensão máxima 10x10, e em seguida os imprima na forma matricial. O programa deve ler
    //dois números inteiros válidos referentes às quantidades de linhas e colunas da matriz.
    //Uma sequência de números inteiros com dois números válidos, m-linhas e n-colunas, para a
    //definição da dimensão da matriz. Em seguida o programa deve ler m×n números inteiros.

    while((m == 0) || (n == 0)){
        scanf("%d", &num);
        if ((m == 0) && (num > 0) && (num < 11)){
            m = num;
        } else if ((n == 0) && (num > 0) && (num < 11)){
            n = num;
        }
    }

    for(i = 0; i < (m*n); i++){
        scanf("%d", &col[i]);
    }
    
    for(i = 0; i < m; i++){
        printf("linha %d: ", i+1);
        for(j = 0; j < n; j++){
            if(j < n-1){
                printf("%d, ", col[c]);
            } else{
                printf("%d\n", col[c]);
            }
            c++;
        }
    }

    return 0;
}
