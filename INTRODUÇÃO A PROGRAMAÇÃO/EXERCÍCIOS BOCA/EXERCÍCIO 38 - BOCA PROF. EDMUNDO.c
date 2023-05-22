#include <stdio.h>

int main() {
    float en[4], sa[4];
    int n, p, i, v;//declaração das variáveis
    //este programa foi feito pela aluna: Nicole Liecheski
    //na data 22/05/2023

   //Escreva 4 números reais.

    for(v = 0; v < 4; ++v){
        sa[v] = 0.00;
    }

    scanf("%f\n%f\n%f\n%f", &en[0], &en[1], &en[2], &en[3]);
    for(i = 0; i < 4; ++i){
        for(n = 0; n < 4; ++n){
            if(en[i]>=sa[n]){
                sa[n+3] = sa[n+2];
                sa[n+2] = sa[n+1];
                sa[n+1] = sa[n];
                sa[n] = en[i];
                n = 4;
            }
        }
    }

    for(p = 3; p > 0; --p){
        printf("%.2f, ", sa[p]);
    }
    printf("%.2f", sa[0]);
    
return 0;
}
