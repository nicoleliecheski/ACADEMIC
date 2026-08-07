#include <stdio.h>

int main() {
    int CC, DR, RT, grau; //declaração das variáveis
    //este programa foi feito pela aluna: Nicole Liecheski
    //na data 15/05/2023

   //Escreva a data em formato inteiro.

    scanf("%d\n%d\n%d", &CC, &DR, &RT);
    if (CC<7){
        grau = 8;
        if(DR>50){
            grau = 9;
            if (RT>80000){
                grau = 10;
            }
        }
    }
    else {
        grau = 7;
    }
    printf("ACO DE GRAU = %d\n", grau);
    
return 0;
}
