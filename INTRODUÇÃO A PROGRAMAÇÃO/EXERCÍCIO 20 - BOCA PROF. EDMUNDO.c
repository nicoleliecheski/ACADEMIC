#include <stdio.h>

int main() {
    float a, b, c, media; //declaração das variáveis
    //este programa foi feito pela aluna: Nicole Liecheski
    //na data 04/05/2023

   //Escreva os valores das três notas do aluno.

    scanf("%f %f %f", &a, &b, &c);
    media = (a+b+c)/3;
    printf("MEDIA = %.2f\n", media);
    if (media >= 6.0){
        printf("APROVADO\n");
    } else {
        printf("REPROVADO\n");
    }

return 0;
}
