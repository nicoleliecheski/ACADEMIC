#include <stdio.h>

int main() {
    int num, n100, n50, n10, m1; //declaração das variáveis
    //este programa foi feito pela aluna: Nicole Liecheski
    //na data 04/05/2023

   //Escreva um valor inteiro em reais.

    scanf("%d", &num);
    n100 = num/100;
    n50 = (num - n100*100)/50;
    n10 = (num - n100*100 - n50*50)/10;
    m1 = (num - n100*100 - n50*50 - n10*10);
    printf("NOTAS DE 100 = %d\nNOTAS DE 50 = %d\nNOTAS DE 10 = %d\nMOEDAS DE 1 = %d\n", n100, n50, n10, m1);

return 0;
}
