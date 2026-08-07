#include <stdio.h>

int main() {
    int num, inv; //declaração das variáveis
    //este programa foi feito pela aluna: Nicole Liecheski
    //na data 04/05/2023

   //Escreva um número com três dígitos. Esse número é diferente de zero e não é múltiplo de 10 ou 100.

    scanf("%d", &num);
    inv = ((num-num%100)/100)+(num%100-num%10)+((num%10)*100);
    printf("%.d\n", inv);

return 0;
}
