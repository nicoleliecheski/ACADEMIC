#include <stdio.h>

int main() {
    float en[3], sa[3];
    int i, n, v, p;//declaração das variáveis
    //este programa foi feito pela aluna: Nicole Liecheski
    //na data 22/05/2023

   //Escreva 3 números reais.

    scanf("%d\n%f\n%f\n%f", &p, &en[0], &en[1], &en[2]);
    
    for(v = 0; v < 4; ++v){
        sa[v] = 0.00;
    }

    if((en[0]>=en[1])&&(en[0]>=en[2])){
        sa[0] = en[0];
        if(en[1]>=en[2]){
            sa[1] = en[1];
            sa[2] = en[2];
        }
        else{
            sa[1] = en[2];
            sa[2] = en[1];
        }
    }
    else{
        if((en[1]>=en[0])&&(en[1]>=en[2])){
            sa[0] = en[1];
            if(en[0]>=en[2]){
                sa[1] = en[0];
                sa[2] = en[2];
            }
            else{
                sa[1] = en[2];
                sa[2] = en[0];
            }
        }
        else{
            if((en[2]>=en[0])&&(en[2]>=en[1])){
                sa[0] = en[2];
                if(en[1]>=en[0]){
                    sa[1] = en[1];
                    sa[2] = en[0];
                }
                else{
                    sa[1] = en[0];
                    sa[2] = en[1];
                }
            }
        }
    }
     
    if(p == 1){
        printf("%.2f %.2f %.2f\n", sa[2], sa[1], sa[0]);
    }
    else{
        if(p==2){
            printf("%.2f %.2f %.2f\n", sa[0], sa[1], sa[2]);
        }
        else{
            printf("%.2f %.2f %.2f\n", sa[1], sa[0], sa[2]);
        }
    }
    
    
    return 0;
}
