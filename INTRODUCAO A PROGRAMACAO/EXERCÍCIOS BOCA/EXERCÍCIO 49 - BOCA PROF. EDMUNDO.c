#include <stdio.h>

int main() {

    int N, i, c;
    char bin[35]; //declaração das variáveis
    //este programa foi feito pela aluna: Nicole Liecheski
    //na data 12/06/2023

    //Escreva um programa que leia vários números inteiros positivos
    //na base decimal e escreva os valores correspondentes desses números
    //na base binária. A entrada contém várias linhas, cada uma contendo um
    //valor inteiro N na base decimal tal que seu número binário equivalente
    //possui no máximo 32 bits. A saída termina por fim de arquivo.

    while(scanf("%d", &N) != EOF){
        i = 0;
        if(N == 0){
            printf("0\n");
        } else{
            while(N != 0){
                bin[i+1] = bin[i];
                bin[i] = N%2 + '0';
                N = N/2;
                i++;
            }
            for(c = i-1; c > -1; c--){
                printf("%c", bin[c]);
            }
            printf("\n");
        }
    }

    return 0;
}
