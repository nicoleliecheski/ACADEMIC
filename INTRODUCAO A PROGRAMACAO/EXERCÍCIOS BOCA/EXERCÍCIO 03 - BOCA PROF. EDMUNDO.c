#include <stdio.h>

int main() {
    float temp, chuva; //declaração das variáveis
    //este programa foi feito pela aluna: Nicole Liecheski
    //na data 27/04/2023

    //Escreva o valor da temperatura e da quantidade de chuva;

    scanf("%f\n%f", &temp, &chuva);
    printf("O VALOR EM CELSIUS = %.2f\nA QUANTIDADE DE CHUVA E = %.2f\n", (5*(temp-32))/9, chuva*25.4);

return 0;
} 
