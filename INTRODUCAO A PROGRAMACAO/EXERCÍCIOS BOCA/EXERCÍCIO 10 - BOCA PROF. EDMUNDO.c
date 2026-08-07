#include <stdio.h>

int main() {
    float valor, f;
    int n; //declaração das variáveis
    //este programa foi feito pela aluna: Nicole Liecheski
    //na data 27/04/2022

    //Escreva o valor a ser arredondado.

    //1 casa
    scanf("%f", &valor);
    n = valor*10+0.5;
    f = n/10.0;
    printf("%f\n", f);
    //2 casas
    n = valor*100+0.5;
    f = n/100.0;
    printf("%f\n", f);
    //3 casas
    n = valor*1000+0.5;
    f = n/1000.0;
    printf("%f\n", f);

return 0;
} 
