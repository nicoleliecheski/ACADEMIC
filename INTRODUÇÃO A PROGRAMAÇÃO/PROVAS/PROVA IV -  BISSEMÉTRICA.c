#include <stdio.h>

int main()
{
    int n, i, j, M[11][11], flag = 1; //declaração das variáveis
    //este programa foi feito pela aluna: Nicole Liecheski
    //na data 03/08/2023

    //Uma matriz bissimétrica é uma matriz quadrada que é simétrica em ambas diagonais.
    //Faça um programa que leia uma matriz quadrada (com números inteiros) de tamanho n × n,
    //onde 1 ≥ n ≥ 10, e verifique se a matriz é bissimétrica ou não.

    scanf("%d", &n);

    if((n < 1) || (n > 10)){
        printf("dimensao invalida\n");
    } else{
        for(i = 0; i < n; i++){
            for(j = 0; j < n; j++){
                scanf("%d", &M[i][j]);
            }
        }

        for(i = 0; i < n; i++){
            for(j = 0; j < n; j++){
                if(M[i][j] != M[j][i]){
                    flag = 0;
                }
                if(M[i][j] != M[n-j-1][n-i-1]){
                    flag = 0;
                }
            }
        }

        if(flag == 1){
            printf("bissimetrica\n");
        } else{
            printf("nao bissimetrica\n");
        }
    }

    return 0;
}
