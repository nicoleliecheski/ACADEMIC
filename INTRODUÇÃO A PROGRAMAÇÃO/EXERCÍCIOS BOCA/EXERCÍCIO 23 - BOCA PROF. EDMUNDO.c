#include <stdio.h>

int main() {
    int num; //declaração das variáveis
    //este programa foi feito pela aluna: Nicole Liecheski
    //na data 08/05/2023

   //Escreva um número inteiro.

    scanf("%d", &num);
    if (((num%3)==0)&&((num%5)==0)) {
        printf("O NUMERO E DIVISIVEL\n");
    } else {
        printf("O NUMERO NAO E DIVISIVEL\n");
    }

return 0;
}
