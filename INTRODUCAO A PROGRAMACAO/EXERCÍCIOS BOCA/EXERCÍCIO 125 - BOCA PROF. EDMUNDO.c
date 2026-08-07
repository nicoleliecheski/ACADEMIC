#include <stdio.h>

int main()
{
    int M[6][6], i, j, maior = -9999, soma; //declaração das variáveis
    //este programa foi feito pela aluna: Nicole Liecheski
    //na data 31/07/2023

    //O objetivo desse execício é identificar um conjunto de elementos 
    //cuja somatória seja o maior valor entre todos os conjuntos definidos
    //por um padrão na forma de ampulheta, em uma matriz de inteiros, quadrada,
    //e de ordem 6. Cada elemento da matriz está no intervalo [-9, 9].
    //O valor de uma ampulheta é a soma de todos os valores presentes nas respectivas posições.
    //Seu programa deve retornar o maior valor entre todos os valores de ampulheta possíveis na matriz.

    for(i = 0; i < 6; i++){
       for(j = 0; j < 6; j++){
            scanf("%d", &M[i][j]);
       }
    }

    /*for(i = 0; i < 6; i++){
       for(j = 0; j < 6; j++){
            printf("%d ", M[i][j]);
       }
       printf("\n");
    }*/

    for(i = 0; i < 4; i++){
       for (j = 0; j < 4; j++){
            soma = M[i][j] + M[i][j+1] + M[i][j+2] + M[i+1][j+1] + M[i+2][j] + M[i+2][j+1] + M[i+2][j+2];
            if(soma > maior){
                maior = soma;
            }
       }
    }

    printf("%d\n", maior);

    return 0;
}
