#include <stdio.h>
#include <math.h>

int main() {
    int num, alg4; //declaração das variáveis
    //este programa foi feito pela aluna: Nicole Liecheski
    //na data 04/05/2023

   //Escreva um número inteiro de três algarismos.

    scanf("%d", &num);
    alg4 = (((num-num%100)/100)+((num%100-num%10)/10)*3+(num%10)*5)%7;
    printf("O NOVO NUMERO E = %.4d\n", num*10+alg4);

return 0;
}
