#include <stdio.h>

int main() {
    int x, un, dz, cn, ml; //declaração das variáveis
    //este programa foi feito pela aluna: Nicole Liecheski
    //na data 11/05/2023

   //Escreva um valor inteiro de até 4 dígitos.

    scanf("%d", &x);
    if ((x<=0)||(x>9999)){
        printf("Numero invalido!\n"); }
    else {
        ml = (x-x%1000)/1000;
        cn = (x%1000-x%100)/100;
        dz = (x%100-x%10)/10;
        un = x%10;
        if (ml>=1){
            printf("(quarta ordem) %d = ", x);}
        else {
            if (cn>=1){
                printf("(terceira ordem) %d = ", x);}
            else {
                if (dz>=1){
                    printf("(segunda ordem) %d = ", x);}
                else {
                    printf("(primeira ordem) %d = ", x);}
            }
        }
        if (ml>1){
            printf("%d unidades de milhar + ", ml);}
        if (ml==1) {
            printf("%d unidade de milhar + ", ml);}
        if (cn>1){
            printf("%d centenas + ", cn);}
        if (cn==1) {
            printf("%d centena + ", cn);}
        if (dz>1){
            printf("%d dezenas + ", dz);}
        if (dz==1) {
            printf("%d dezena + ", dz);}
        if (un>1){
            printf("%d unidades = ", un);}
        if (un==1) {
            printf("%d unidade = ", un);}
        if (ml>=1){
            printf("%d + ", ml*1000);}
        if (cn>=1){
            printf("%d + ", cn*100);}
        if (dz>=1){
            printf("%d + ", dz*10);}
        if (un>=1) {
            printf("%d\n", un);}
    }

return 0;
}
