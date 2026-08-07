#include <stdio.h>

int main() {
    int n1, n2, n3, i = 2, c = 0, r = 1, d = 0; //declaração das variáveis
    //este programa foi feito pela aluna: Nicole Liecheski
    //na data 01/06/2023

   //Escreva três valores inteiros.

    scanf("%d %d %d", &n1, &n2, &n3);

    while((n1 > 1) || (n2 > 1) || (n3 > 1)){
        if(((n1%i) != 0) && ((n2%i) != 0) && ((n3%i) != 0)){
            i++;
            } else{
                printf("%d %d %d ", n1, n2, n3);
                if((n1%i) == 0){
                    n1 = n1/i;}
                if((n2%i) == 0){
                    n2 = n2/i;}
                if((n3%i) == 0){
                    n3 = n3/i;}
                printf(":%d\n", i);
                r = r*i;
            }
    }

    printf("MMC: %d\n", r);


return 0;
}
