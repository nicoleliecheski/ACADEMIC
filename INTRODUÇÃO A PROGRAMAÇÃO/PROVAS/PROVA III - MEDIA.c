#include <stdio.h>

int main() {
    int N, i, V[1000], p = 0, s = 0;
    float media, soma; //declaração das variáveis
    //este programa foi feito pela aluna: Nicole Liecheski
    //na data 29/06/2023

    scanf("%d", &N);

    for(i = 0; i < N; i++){
        if(i < N-1){
            scanf("%d ", &V[i]);
        } else{
            scanf("%d", &V[i]);
        }
    }

    if((N < 5)||(N > 1000)){
        printf("QUANTIDADE DE ELEMENTOS INVALIDOS!\n");
    } else{
        for(i = 0; i < N; i++){
            soma =  soma + V[i];
        }
        media = soma/N;
        for(i = 0; i < (N/2); i++){
            if(V[i] > media){
                p++;
            }
        }
        for(i = (N/2)+1; i < N; i++){
            if(V[i] > media){
                s++;
            }
        }
        printf("%.2f %d %d ", media, p, s);
        if(s == p){
            printf("EMPATE\n");
        } else{
            if(p > s){
                printf("PRIMEIRA METADE\n");
            } else{
                printf("SEGUNDA METADE\n");
            }
        }
    }

return 0;
}
