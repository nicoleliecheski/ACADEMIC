#include <stdio.h>

int main() {
    int i, N, d; //declaração das variáveis
    //este programa foi feito pela aluna: Nicole Liecheski
    //na data 30/05/2023

    //Faça um programa que leia um número N e informa se o número é primo ou não.
  
    scanf("%d", &N);

    d = 0;
    if(N<0){
        printf("Numero invalido!\n");}
    else{
        if(N<2){
            printf("NAO PRIMO\n");}
        else{
            for(i=2; i<N; i++){
                if((N%i)==0){
                    d = 1;}
            }
            if(d!=0){
                printf("NAO PRIMO\n");}
            else{
                printf("PRIMO\n");
            }
        }
    }


return 0;
}
