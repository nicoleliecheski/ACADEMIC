#include <stdio.h>

int main() {
    float raio, alt, valor; //declaração das variáveis
    //este programa foi feito pela aluna: Nicole Liecheski
    //na data 27/04/2023

    //Escreva o valor do raio e da altura da lata.

    scanf("%f\n%f", &raio, &alt);
    valor = (2*(3.14159*raio*raio)+(2*3.14159*raio*alt))*100.00;
    printf("O VALOR DO CUSTO E = %.2f\n", valor);

return 0;
} 
