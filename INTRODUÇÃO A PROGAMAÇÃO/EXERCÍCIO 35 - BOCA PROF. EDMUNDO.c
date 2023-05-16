#include <stdio.h>

int main() {
    int matricula, dependentes;
    float salaMIN, salaFUN, taxa, impB, impL, res; //declaração das variáveis
    //este programa foi feito pela aluna: Nicole Liecheski
    //na data 16/05/2023

   //Escreva a matrícula, o valor do salário mínimo, o número de dependentes, o salário do funcionário e a taxa de imposto.

    scanf("%d %f %d %f %f", &matricula, &salaMIN, &dependentes, &salaFUN, &taxa);
    if (salaFUN>(12*salaMIN)){
        impB = salaFUN*0.20;}
    else {
        if (salaFUN>(5*salaMIN)){
            impB = salaFUN*0.08;}
        else {
            impB = 0.00;
        }
    }
    
    impL = impB - 300.00*dependentes;
    res = impL - (salaFUN*(taxa/100.00));
    
    printf("MATRICULA = %d\nIMPOSTO BRUTO = %.2f\nIMPOSTO LIQUIDO = %.2f\nRESULTADO = %.2f\n", matricula, impB, impL, res);
    if (res>0){
        printf("IMPOSTO A PAGAR\n");}
    else {
        if (res<0){
            printf("IMPOSTO A RECEBER\n");}
        else{
            printf("IMPOSTO QUITADO\n");
        }
    }
    
return 0;
}
