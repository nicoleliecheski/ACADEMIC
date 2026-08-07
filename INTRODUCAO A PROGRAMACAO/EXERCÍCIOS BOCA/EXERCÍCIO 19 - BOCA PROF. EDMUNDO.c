#include <stdio.h>

int main() {
    float a, b, c, d, e, f, x, y; //declaração das variáveis
    //este programa foi feito pela aluna: Nicole Liecheski
    //na data 04/05/2023

   //Escreva os valores dos coeficientes: a,b, c,d, e & f.

    scanf("%f\n%f\n%f\n%f\n%f\n%f", &a, &b, &c, &d, &e, &f);
    x = (c*e-b*f)/(a*e-b*d);
    y = (a*f-c*d)/(a*e-b*d);
    printf("O VALOR DE X E = %.2f\nO VALOR DE Y E = %.2f\n", x, y);

return 0;
}
