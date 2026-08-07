#include <stdio.h>
#include <math.h>

int main() {
    float A, B, C;
    int n, p, i, v;//declaração das variáveis
    //este programa foi feito pela aluna: Nicole Liecheski
    //na data 22/05/2023

   //Escreva 3 números reais.


    scanf("%f %f %f", &A, &B, &C);
    if((fabs(B - C)<A)&&(A<(B + C))){
        if((fabs(A - C)<B)&&(B<(A + C))){
           if((fabs(A - B)<C)&&(C<(A + B))){
               printf("Perimetro = %.1f\n", A+B+C);
           }
           else{
               printf("Area = %.1f\n", ((A+B)*C)/2.0);
           }
        }
        else{
            printf("Area = %.1f\n", ((A+B)*C)/2.0);
        }
    }
    else{
        printf("Area = %.1f\n", ((A+B)*C)/2.0);
        }
    
    return 0;
}
