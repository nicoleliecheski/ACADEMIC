#include <stdio.h>

int main()
{
    double a, b, c, d, A; //declaração das variáveis
    //este programa foi feito pela aluna: Nicole Liecheski
    //na data 24/07/2023

    //Escreva um programa em C que leia uma matrix de dimensão 2×2 e calcule o seu determinante.

    scanf("%lf", &a);
    scanf("%lf", &b);
    scanf("%lf", &c);
    scanf("%lf", &d);

    A = (a*d) - (b*c);

    printf("%.2lf", A);

    return 0;
}
