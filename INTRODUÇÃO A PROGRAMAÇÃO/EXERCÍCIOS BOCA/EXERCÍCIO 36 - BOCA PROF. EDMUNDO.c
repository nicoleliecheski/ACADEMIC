#include <stdio.h>

int main() {
    int num, alg[5]; //declaração das variáveis
    //este programa foi feito pela aluna: Nicole Liecheski
    //na data 22/05/2023

   //Escreva um número inteiro de até 5 digítos.

    scanf("%d", &num);
    alg[0] = (num - num%10000)/10000;
    alg[1] = (num%10000 - num%1000)/1000;
    alg[2] = (num%1000 - num%100)/100;
    alg[3] = (num%100 - num%10)/10;
    alg[4] = num%10;
    
    if (num>99999){
        printf("NUMERO INVALIDO\n");}
    else {
        if(num>9999){
            if((alg[0]==alg[4])&&(alg[1]==alg[3])){
                printf("PALINDROMO\n");}
            else{
                printf("NAO PALINDROMO\n");}
        }
        else{
            if(num>999){
                if((alg[1]==alg[4])&&(alg[2]==alg[3])){
                    printf("PALINDROMO\n");}
                else{
                    printf("NAO PALINDROMO\n");}
            }
            else{
                if(num>99){
                    if(alg[2]==alg[4]){
                        printf("PALINDROMO\n");}
                    else{
                        printf("NAO PALINDROMO\n");}
                }
                else{
                    if(num>9){
                        if(alg[3]==alg[4]){
                            printf("PALINDROMO\n");}
                        else{
                            printf("NAO PALINDROMO\n");}
                    }
                    else{
                        printf("PALINDROMO\n");
                    }
                }
            }
        }
        
    }
    
    
return 0;
}
