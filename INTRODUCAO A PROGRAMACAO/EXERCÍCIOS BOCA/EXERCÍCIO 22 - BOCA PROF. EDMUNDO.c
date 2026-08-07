#include <stdio.h>

int main() {
    int conta;
    float consumo, valor;
    char tipo; //declaração das variáveis
    //este programa foi feito pela aluna: Nicole Liecheski
    //na data 08/05/2023

   //Escreva a conta do cliente (um número inteiro), o consumo de água por metros cúbicos (float) e o tipo do consumidor (um caractere: ‘C’ - COMERCIAL, ‘I’ - INDUSTRIAL ou ‘R’ - RESIDENCIAL).

    scanf("%d %f %c", &conta, &consumo, &tipo);
    if (tipo == 'R') {
        valor = consumo*0.05+5;
    }
    if (tipo == 'C') {
        if (consumo <= 80.00){
            valor = 500.00;
        } else {
            valor = (consumo-80.00)*0.25+500.00;
        }
    }
    if (tipo == 'I') {
        if (consumo <= 100.00){
            valor = 800.00;
        } else {
            valor = (consumo-100.00)*0.04+800.00;
        }
    }
    printf("CONTA = %d\nVALOR DA CONTA = %.2f\n", conta, valor);

return 0;
}
