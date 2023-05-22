#include <stdio.h>
#include <math.h>

int main() {
    float l1, l2, l3, t; //declaração das variáveis
    //este programa foi feito pela aluna: Nicole Liecheski
    //na data 02/05/2023

    //Escreva o valor dos lados do triangulo

    scanf("\n%f\n%f\n%f", &l1, &l2, &l3);
    t = (l1+l2+l3)/2;
    printf("A AREA DO TRIANGULO E = %.2f\n", sqrtf(t*(t-l1)*(t-l2)*(t-l3)));

return 0;
}
