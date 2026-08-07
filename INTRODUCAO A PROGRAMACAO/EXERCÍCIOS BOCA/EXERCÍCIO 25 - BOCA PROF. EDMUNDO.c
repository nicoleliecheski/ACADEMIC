#include <stdio.h>

int main() {
    float nota;
    char conceito;//declaração das variáveis
    //este programa foi feito pela aluna: Nicole Liecheski
    //na data 08/05/2023

   //Escreva a nota do aluno em valor real.

    scanf("%f", &nota);
    if (nota<6.0){
        conceito = 'D';
    } else {
        if (nota<7.5){
            conceito = 'C';
        } else {
            if (nota<9.0){
                conceito = 'B';
            } else {
                conceito = 'A';
            }
        }
    }
    
    printf("NOTA = %.1f CONCEITO = %c\n", nota, conceito);

return 0;
}
