#include <stdio.h>
#include <math.h>

int main() {
    int N, i; //declaração das variáveis
    //este programa foi feito pela aluna: Nicole Liecheski
    //na data 29/05/2023
  
    //Escreva um programa para ler um valor inteiro N e que gere o quadrado de cada um dos valores pares,
    //de 1 até N, inclusive N, se for o caso.
    //A entrada conterá uma linha com um valor inteiro N, 5 < N < 2000.

    scanf("%d", &N);

    for(i = 2; i<=N; i=i+2){
        if((i%2)==0){
        printf("%d^2 = %.0lf\n", i, pow(i, 2));}
    }

    return 0;
}
