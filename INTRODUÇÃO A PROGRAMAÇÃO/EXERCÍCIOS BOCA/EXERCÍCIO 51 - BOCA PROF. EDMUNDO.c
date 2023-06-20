#include <stdio.h>

int main() {

    int N, i, nts[10000], ultima, maior = -1, indice = -1, v = 0; //declaração das variáveis
    //este programa foi feito pela aluna: Nicole Liecheski
    //na data 19/06/2023

    //Dada uma sequência de N notas entre 0 e 10, escreva um programa que exiba o valor da última nota
    //informada e quantas vezes ela apareceu no conjunto. O programa deve exibir
    //ainda a maior nota informada e a posição (índice do vetor) da sua primeira ocorrência.
    //Na primeira linha há um inteiro N, sendo 1 ≤ N ≤ 10000 representando a
    //quantidade de notas da sequência. Não é necessário validar o valor de N na entrada.
    //Nas N linhas seguintes haverá um número inteiro entre 0 e 10, inclusive, em cada linha.

    scanf("%d", &N);

    for(i = 0; i < N; i++){
        scanf("%d", &nts[i]);
        if(i == N-1){
            ultima = nts[i];
        }
        if(nts[i] > maior){
            maior = nts[i];
        }
    }

    for(i = 0; i < N; i++){
        if((nts[i] == maior)&&(indice == -1)){
            indice = i;
        }
        if(nts[i] == ultima){
            v++;
        }
    }

    printf("Nota %d, %d vezes\nNota %d, indice %d", ultima, v, maior, indice);

    return 0;
}
