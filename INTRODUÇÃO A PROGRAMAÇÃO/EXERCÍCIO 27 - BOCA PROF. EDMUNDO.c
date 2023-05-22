#include <stdio.h>

int main() {
    int x, y; //declaração das variáveis
    //este programa foi feito pela aluna: Nicole Liecheski
    //na data 11/05/2023

   //Escreva o valor de x.

    scanf("%d", &x);
    if (x<1){
        y = x;
    }
    if (x==1){
        y = 0;
    }
    if (x>1){
        y = x*x;
    }
    
    printf("Y = %d\n", y);

return 0;
}
