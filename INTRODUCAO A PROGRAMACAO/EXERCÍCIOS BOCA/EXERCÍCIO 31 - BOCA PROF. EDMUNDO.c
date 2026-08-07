#include <stdio.h>
#include <math.h>

int main() {
    float a, b, c, delta, x1, x2; //declaração das variáveis
    //este programa foi feito pela aluna: Nicole Liecheski
    //na data 11/05/2023

   //Escreva o valor dos coeficientes a, b, c;

    scanf("%f %f %f", &a, &b, &c);
    delta = b*b - 4*a*c;
    if (delta<0){
        printf("RAIZES IMAGINARIAS\n");}
    else{
        if (delta==0){
            x1 = (-b)/(2*a);
            printf("RAIZ UNICA\nX1 = %.2f", x1);}
        else {
            x1 = ((-b)+sqrt(delta))/(2*a);
            x2 = ((-b)-sqrt(delta))/(2*a);
            if (x2>x1){
                printf("RAIZES DISTINTAS\nX1 = %.2f\nX2 = %.2f\n", x1, x2);}
            if (x1>x2){
                printf("RAIZES DISTINTAS\nX1 = %.2f\nX2 = %.2f\n", x2, x1);
            }   
        }
    }
return 0;
}
