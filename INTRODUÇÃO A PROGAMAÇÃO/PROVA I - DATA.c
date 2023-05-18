#include <stdio.h>

int main() {
    int dia, mes, ano, bi; //declaração das variáveis
    //este programa foi feito pela aluna: Nicole Liecheski
    //na data 18/05/2023

   //Escreva a data.

    scanf("%d %d %d", &dia, &mes, &ano);
    if((ano>2036)||(ano<1900)){
        printf("DATA FORA DO INTERVALO ESTIPULADO\n");}
    else{
        if((ano%4)!=0){
            bi = 0;}
        else{
            if((ano%100)!=0){
                bi = 1;}
            else{
                if((ano%400)==0){
                    bi = 1;}
                else{
                    bi = 0;
                }
            }
        }
        switch(mes){
            case 1:
                if(dia>31){
                    printf("DATA INVALIDA\n");}
                else {
                    printf("%d/%d/%d\n", dia, mes, ano);
                }
            break;
            case 2:
                if(dia>29){
                    printf("DATA INVALIDA\n");}
                else {
                    if((bi==0)&&(dia>28)){
                        printf("DATA INVALIDA\n");}
                    else {
                        printf("%d/%d/%d\n", dia, mes, ano);
                    }
                }
            break;
            case 3:
                if(dia>31){
                    printf("DATA INVALIDA\n");}
                else {
                    printf("%d/%d/%d\n", dia, mes, ano);
                }
            break;
            case 4:
                if(dia>30){
                    printf("DATA INVALIDA\n");}
                else {
                    printf("%d/%d/%d\n", dia, mes, ano);
                }
            break;
            case 5:
                if(dia>31){
                    printf("DATA INVALIDA\n");}
                else {
                    printf("%d/%d/%d\n", dia, mes, ano);
                }
            break;
            case 6:
                if(dia>30){
                    printf("DATA INVALIDA\n");}
                else {
                    printf("%d/%d/%d\n", dia, mes, ano);
                }
            break;
            case 7:
                if(dia>31){
                    printf("DATA INVALIDA\n");}
                else {
                    printf("%d/%d/%d\n", dia, mes, ano);
                }
            break;
            case 8:
                if(dia>31){
                    printf("DATA INVALIDA\n");}
                else {
                    printf("%d/%d/%d\n", dia, mes, ano);
                }
            break;
            case 9:
                if(dia>30){
                    printf("DATA INVALIDA\n");}
                else {
                    printf("%d/%d/%d\n", dia, mes, ano);
                }
            break;
            case 10:
                if(dia>31){
                    printf("DATA INVALIDA\n");}
                else {
                    printf("%d/%d/%d\n", dia, mes, ano);
                }
            break;
            case 11:
                if(dia>30){
                    printf("DATA INVALIDA\n");}
                else {
                    printf("%d/%d/%d\n", dia, mes, ano);
                }
            break;
            case 12:
                if(dia>31){
                    printf("DATA INVALIDA\n");}
                else {
                    printf("%d/%d/%d\n", dia, mes, ano);
                }
            break;
            default:
                printf("DATA INVALIDA\n");
            break;
        }
    }
   
return 0;
}
