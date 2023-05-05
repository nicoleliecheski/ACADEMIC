#include <stdio.h>

int main() {
    int conta;
    float consumo, valor;
    char tipo; //declaração das variáveis
    //este programa foi feito pela aluna: Nicole Liecheski
    //na data 04/05/2023

   //Escreva três números inteiros de um dígito.

    scanf("%d %f %c", &conta, &consumo, &tipo);
    if (tipo == "R") {
        valor = consumo*0.05+5;
    }
    if (tipo == "C") {
        if (consumo <= 80){
            valor = 500.0;
        } else {
            valor = (consumo-80)*0.25+500.0;
        }
    }
    if (tipo == "I") {
        if (consumo <= 100){
            valor = 800.0;
        } else {
            valor = (consumo-80)*0.04+800.0;
        }
    }
    printf("CONTA = %d\nVALOR DA CONTA = %.2f\n", conta, valor);

return 0;
}
