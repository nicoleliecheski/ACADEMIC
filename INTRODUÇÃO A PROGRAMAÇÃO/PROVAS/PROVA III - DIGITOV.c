#include <stdio.h>

int main() {
    int UF, i, dv1, dv2, digUF[2] = {0}, somaUF;
    long int numSeq, dig[8] = {0}, soma; //declaração das variáveis
    //este programa foi feito pela aluna: Nicole Liecheski
    //na data 29/06/2023

    scanf("%ld %d", &numSeq, &UF);
    if((UF > 28)||(UF < 1)){
        printf("CODIGO DA UF INVALIDO!\n");
    } else{
        dig[0] = (numSeq - numSeq%10000000)/100000000;
        dig[1] = (numSeq%10000000 - numSeq%1000000)/10000000;
        dig[2] = (numSeq%1000000 - numSeq%100000)/1000000;
        dig[3] = (numSeq%100000 - numSeq%10000)/100000;
        dig[4] = (numSeq%10000 - numSeq%1000)/10000;
        dig[5] = (numSeq%1000 - numSeq%100)/1000;
        dig[6] = (numSeq%100 - numSeq%10)/100;
        dig[7] = numSeq%10;
        for(i = 0; i < 8; i++){
            printf("%ld ", dig[i]);
        }
        printf("\n");
        soma = (dig[0]*2)+(dig[1]*3)+(dig[2]*4)+(dig[3]*5)+(dig[4]*6)+(dig[5]*7)+(dig[6]*8)+(dig[7]*9);
        dv1 = soma%11;
        if(dv1 == 10){
            dv1 = 0;
        }
        digUF[0] = (UF - UF%10)/10;
        digUF[1] = UF%10;
        printf("%d %d", digUF[0], digUF[1]);
        printf("\n");
        somaUF = (digUF[0]*7)+(digUF[1]*8);
        dv2 = somaUF%11;
        if(dv2 == 10){
            dv2 = 0;
        }
        printf("%ld%.2d/%d%d", numSeq, UF, dv1, dv2);
    }

return 0;
}
