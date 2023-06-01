#include <stdio.h>

int main() {
    int n1, n2, n3, n4, res; //declaração das variáveis
    //este programa foi feito pela aluna: Nicole Liecheski
    //na data 01/06/2023

   //Escreva quatro valores inteiros.

    scanf("%d\n%d\n%d\n%d", &n1, &n2, &n3, &n4);
    if((n1 >= n2) && (n1 >= n3) && (n1 >= n4)){
        res = n2+n3+n4;
    } else{
        if((n2 >= n1) && (n2 >= n3) && (n2 >= n4)){
            res = n1+n3+n4;
        } else{
            if((n3 >= n1) && (n3 >= n2) && (n3 >= n4)){
                res = n1+n2+n4;
            } else{
                res = n1+n2+n3;
            }
        }
    }

    printf("%d\n", res);


return 0;
}
