#include <stdio.h>

int main() {
    int n1, n2, n3; //declaração das variáveis
    //este programa foi feito pela aluna: Nicole Liecheski
    //na data 04/05/2023

   //Escreva três números inteiros de um dígito.

    scanf("%d\n%d\n%d", &n1, &n2, &n3);
    if ((n1 > 9)||(n2 > 9)||(n3 > 9)){
        printf("DIGITO INVALIDO\n");
    } else {
        printf("%d, %d\n", (n1*100)+(n2*10)+n3, ((n1*100)+(n2*10)+n3)*((n1*100)+(n2*10)+n3));
    }

return 0;
}
