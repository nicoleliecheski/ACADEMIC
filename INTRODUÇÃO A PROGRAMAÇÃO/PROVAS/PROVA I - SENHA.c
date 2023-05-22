#include <stdio.h>

int main() {
    int senha;
    char catg; //declaração das variáveis
    //este programa foi feito pela aluna: Nicole Liecheski
    //na data 18/05/2023

   //Escreva a senha e a categoria do usuário.

    scanf("%d %c", &senha, &catg);
    if(senha<100000){
        printf("SENHA INVALIDA: %d - MINIMO 6 DIGITOS\n", senha);}
    else{
        if((catg == 'A')||(catg == 'G')||(catg == 'U')){
            if(catg == 'A'){
                if (senha == 321456){
                    printf("ACESSO PERMITIDO A CATEGORIA %c\n", catg);}
                else{
                    printf("ACESSO NEGADO\n");
                }
            }
            if(catg == 'G'){
                if (senha == 9510753){
                    printf("ACESSO PERMITIDO A CATEGORIA %c\n", catg);}
                else{
                    printf("ACESSO NEGADO\n");
                }
            }
            if(catg == 'U'){
                if (senha == 78955632){
                    printf("ACESSO PERMITIDO A CATEGORIA %c\n", catg);}
                else{
                    printf("ACESSO NEGADO\n");
                }
            }
        } else{
            printf("CATEGORIA INVALIDA: %c\n", catg);
        }
    }
   
return 0;
}
