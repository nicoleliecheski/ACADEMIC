#include <stdio.h>

int main() {
    int num, bin, n = 1; //declaração das variáveis
    //este programa foi feito pela aluna: Nicole Liecheski
    //na data 04/05/2023

   //Escreva um número inteiro de 0 até 255.

    scanf("%d", &num);
    if ((num < 0)||(num > 255)) {
        printf("Numero invalido!");
    } else {
        while (num > 0) {
            bin = bin+((num%2)*n);
            num = num/2;
            n = n*10;
        }
        printf("%.8d", bin);
    }

return 0;
}
