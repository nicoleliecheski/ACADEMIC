#include <stdio.h>

int main() {
    int l1, l2, l3, l4; //declaração das variáveis
    //este programa foi feito pela aluna: Nicole Liecheski
    //na data 18/05/2023

   //Escreva a velocidade do grupo de quatro lesmas.

    scanf("%d %d %d %d", &l1, &l2, &l3, &l4);
    if((l1<=0)||(l2<=0)||(l3<=0)||(l4<=0)){
        printf("Grupo invalido\n");}
    else{
        if((l1>=20)||(l2>=20)||(l3>=20)||(l4>=20)){
            printf("Nivel 3\n");}
        else {
            if((l1>=10)||(l2>=10)||(l3>=10)||(l4>=10)){
                printf("Nivel 2\n");}
            else{
                printf("Nivel 1\n");
            }
        }
    }
   
return 0;
}
