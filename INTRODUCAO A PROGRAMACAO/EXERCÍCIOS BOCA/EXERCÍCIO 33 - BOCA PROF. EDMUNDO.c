#include <stdio.h>
#include <string.h>

int main() {
    int dia, mes, ano, data;
    char mesEX[10];//declaração das variáveis
    //este programa foi feito pela aluna: Nicole Liecheski
    //na data 15/05/2023

   //Escreva a data em formato inteiro.

    scanf("%d", &data);
    dia = (data-data%1000000)/1000000;
    mes = (data%1000000-data%10000)/10000;
    ano = data%10000;
    if (dia>31){
        printf("Data invalida!\n"); }
    else {
        if(mes>12){
            printf("Data invalida!\n"); }
        else{
            switch (mes){
                case 1:
                    strcpy(mesEX, "janeiro");
                    break;
                case 2:
                    if (dia>28){
                        printf("Data invalida!\n"); 
                        return 0;}
                    else {
                        strcpy(mesEX, "fevereiro");}
                    break;
                case 3:
                    strcpy(mesEX, "março");
                    break;
                case 4:
                    if (dia>30){
                        printf("Data invalida!\n");
                        return 0;}
                    else {
                        strcpy(mesEX, "abril");}
                    break;
                case 5:
                    strcpy(mesEX, "maio");
                    break;
                case 6:
                    if (dia>30){
                        printf("Data invalida!\n");
                        return 0;}
                    else {
                        strcpy(mesEX, "junho");}
                    break;
                case 7:
                    strcpy(mesEX, "julho");
                    break;
                case 8:
                    strcpy(mesEX, "agosto");
                    break;
                case 9:
                    if (dia>30){
                        printf("Data invalida!\n");
                        return 0;}
                    else {
                        strcpy(mesEX, "setembro");}
                    break;
                case 10:
                    strcpy(mesEX, "outubro");
                    break;
                case 11:
                    if (dia>30){
                        printf("Data invalida!\n");
                        return 0;}
                    else {
                        strcpy(mesEX, "novembro");}
                    break;
                case 12:
                    strcpy(mesEX, "dezembro");
                    break;
            }
            
            printf("%d de %s de %d\n", dia, mesEX, ano);
        }
    }
    
return 0;
}
