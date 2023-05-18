#include <stdio.h>

int main() {
    int A, B, C, maior, l1, l2; //declaração das variáveis
    //este programa foi feito pela aluna: Nicole Liecheski
    //na data 18/05/2023

   //Escreva o valor dos lados do triângulo.

    scanf("%d %d %d", &A, &B, &C);
    if((A<=0)||(B<=0)||(C<=0)){
        printf("triangulo invalido\n");}
    else{
        if((A>=B)&&(A>=C)){
            maior = A;
            l1 = B;
            l2 = C;}
        else {
            if((B>=A)&&(B>=C)){
                maior = B;
                l1 = A;
                l2 = C;}
            else{
                if((C>=A)&&(C>=B)){
                maior = C;
                l1 = A;
                l2 = B;}
            }
        }
        if((maior*maior)<(l1*l1+l2*l2)){
            printf("acutangulo\n");}
        else{
            if((maior*maior)==(l1*l1+l2*l2)){
                printf("retangulo\n");}
            else{
                if((maior*maior)>(l1*l1+l2*l2)){
                    printf("obtusangulo\n");}
            }
        }
    }
   
return 0;
}
