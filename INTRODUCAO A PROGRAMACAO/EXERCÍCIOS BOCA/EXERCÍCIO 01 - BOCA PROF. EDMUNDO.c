#include <stdio.h>

int main() {
    int idade;
    float batida;//declaração das variáveis
    //este programa foi feito pela aluna: Nicole Liecheski
    //na data 02/05/2023

    //Escreva o valor da idade da pessoa.

    scanf("\n%d", &idade);
    batida = idade*365.25*24*60*60;
    printf("O CORACAO BATEU %.2f VEZES\n", batida);

return 0;
} 
