#include <stdio.h>

int main() {
    float a, b, c, d; //declaração das variáveis
    //este programa foi feito pela aluna: Nicole Liecheski
    //na data 27/04/2023

    //Escreva o valor dos coenficientes A, B e C.

    scanf("%f\n%f\n%f\n%f", &a, &b, &c, &d);
    printf("O VALOR DO DETERMINANTE E = %.2f\n", a*d - b*c);

return 0;
} 
