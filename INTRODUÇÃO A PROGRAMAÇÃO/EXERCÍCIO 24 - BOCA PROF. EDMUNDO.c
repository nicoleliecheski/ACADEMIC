#include <stdio.h>

int main() {
    int horas; //declaração das variáveis
    //este programa foi feito pela aluna: Nicole Liecheski
    //na data 08/05/2023

   //Escreva o número de horas que o locatário utilizou a charrete.

    scanf("%d", &horas);
    printf("O VALOR A PAGAR E = %.2f\n", (horas/3)*10.00+(horas%3)*5.00);

return 0;
}
