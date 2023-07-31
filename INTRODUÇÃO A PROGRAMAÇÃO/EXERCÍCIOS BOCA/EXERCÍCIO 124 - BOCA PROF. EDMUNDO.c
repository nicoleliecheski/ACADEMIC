#include <stdio.h>

int main()
{
    float a, b, c, d; //declaração das variáveis
    //este programa foi feito pela aluna: Nicole Liecheski
    //na data 31/07/2023

    //Faça um programa que leira uma matriz 2×2 e imprima o seu quadrado.
    //O quadrado de uma matriz A é dado pela operação: A² = AA.

    scanf("%f", &a);
    scanf("%f", &b);
    scanf("%f", &c);
    scanf("%f", &d);
    
    printf("%.3f %.3f\n%.3f %.3f\n", (a*a+b*c), (a*b+b*d), (c*a+c*d), (c*b+d*d));

    return 0;
}
